#include "R_Workspace_Vulkan_Viewport.h"

R_Workspace_Vulkan_Viewport::R_Workspace_Vulkan_Viewport(QT_Text_Stream* P_Log) : QT_Linear_Contents("_Container", true) {
	Log = P_Log;

	QVulkanInstance* inst = new QVulkanInstance();
	inst->setLayers({ "VK_LAYER_KHRONOS_validation" });

	if (!inst->create())
		qFatal("Failed to create Vulkan instance: %d", inst->errorCode());

	R_Vulkan_Viewport_Window* vulkanWindow = new R_Vulkan_Viewport_Window();
	vulkanWindow->setVulkanInstance(inst);

	Vulkan_Widget = QWidget::createWindowContainer(vulkanWindow);
	Log->append("<p style = color:rgb(250,140,25);>Vulkan Viewport Initialized</p>");
}


R_Vulkan_Viewport_Widget::R_Vulkan_Viewport_Widget(R_Vulkan_Viewport_Window* w) : Wrapped_Window(w) {
	Window_Wrapper = QWidget::createWindowContainer(w);

	//QPushButton *grabButton = new QPushButton(tr("&Grab"));
	//connect(grabButton, &QPushButton::clicked, this, &R_VulkanWidget::onGrabRequested);
}

void R_Vulkan_Viewport_Widget::onGrabRequested() {

	QImage img = Wrapped_Window->grab();

	// startNextFrame() is synchronous so img is ready to be used here.

	QFileDialog fd(this);
	fd.setAcceptMode(QFileDialog::AcceptSave);
	fd.setDefaultSuffix("png");
	fd.selectFile("test.png");
	if (fd.exec() == QDialog::Accepted)
		img.save(fd.selectedFiles().first());
}

QVulkanWindowRenderer* R_Vulkan_Viewport_Window::createRenderer() {
	return new R_Vulkan_Viewport_Renderer(this);
}

R_Vulkan_Viewport_Renderer::R_Vulkan_Viewport_Renderer(R_Vulkan_Viewport_Window* w) : R_Vulkan_Window_Viewport_Renderer(w, true) {
}

void R_Vulkan_Viewport_Renderer::initResources() {
	R_Vulkan_Window_Viewport_Renderer::initResources();
}

void R_Vulkan_Viewport_Renderer::startNextFrame() {
	R_Vulkan_Window_Viewport_Renderer::startNextFrame();
	emit static_cast<R_Vulkan_Viewport_Window*>(Wrapped_Window)->frameQueued(int(m_rotation) % 360);
}

static float vertexData[] = { // Y up, front = CCW
	 0.0f,   0.5f,   1.0f, 0.0f, 0.0f,
	-0.5f,  -0.5f,   0.0f, 1.0f, 0.0f,
	 0.5f,  -0.5f,   0.0f, 0.0f, 1.0f
};

static const int UNIFORM_DATA_SIZE = 16 * sizeof(float);

static inline VkDeviceSize aligned(VkDeviceSize v, VkDeviceSize byteAlign) {
	return (v + byteAlign - 1) & ~(byteAlign - 1);
}

R_Vulkan_Window_Viewport_Renderer::R_Vulkan_Window_Viewport_Renderer(QVulkanWindow* w, bool msaa) : Wrapped_Window(w) {
	if (msaa) {
		const QList<int> counts = w->supportedSampleCounts();
		qDebug() << "Supported sample counts:" << counts;
		for (int s = 16; s >= 4; s /= 2) {
			if (counts.contains(s)) {
				qDebug("Requesting sample count %d", s);
				Wrapped_Window->setSampleCount(s);
				break;
			}
		}
	}
}

VkShaderModule R_Vulkan_Window_Viewport_Renderer::createShader(const QString& name)
{
	QFile file(name);
	if (!file.open(QIODevice::ReadOnly)) {
		qWarning("Failed to read shader %s", qPrintable(name));
		return VK_NULL_HANDLE;
	}
	QByteArray blob = file.readAll();
	file.close();

	VkShaderModuleCreateInfo shaderInfo;
	memset(&shaderInfo, 0, sizeof(shaderInfo));
	shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderInfo.codeSize = blob.size();
	shaderInfo.pCode = reinterpret_cast<const uint32_t*>(blob.constData());
	VkShaderModule shaderModule;
	VkResult err = m_devFuncs->vkCreateShaderModule(Wrapped_Window->device(), &shaderInfo, nullptr, &shaderModule);
	if (err != VK_SUCCESS) {
		qWarning("Failed to create shader module: %d", err);
		return VK_NULL_HANDLE;
	}

	return shaderModule;
}

void R_Vulkan_Window_Viewport_Renderer::initResources() {
	qDebug("initResources");

	VkDevice dev = Wrapped_Window->device();
	m_devFuncs = Wrapped_Window->vulkanInstance()->deviceFunctions(dev);

	// Prepare the vertex and uniform data. The vertex data will never
	// change so one buffer is sufficient regardless of the value of
	// QVulkanWindow::CONCURRENT_FRAME_COUNT. Uniform data is changing per
	// frame however so active frames have to have a dedicated copy.

	// Use just one memory allocation and one buffer. We will then specify the
	// appropriate offsets for uniform buffers in the VkDescriptorBufferInfo.
	// Have to watch out for
	// VkPhysicalDeviceLimits::minUniformBufferOffsetAlignment, though.

	// The uniform buffer is not strictly required in this example, we could
	// have used push constants as well since our single matrix (64 bytes) fits
	// into the spec mandated minimum limit of 128 bytes. However, once that
	// limit is not sufficient, the per-frame buffers, as shown below, will
	// become necessary.

	const int concurrentFrameCount = Wrapped_Window->concurrentFrameCount();
	const VkPhysicalDeviceLimits* pdevLimits = &Wrapped_Window->physicalDeviceProperties()->limits;
	const VkDeviceSize uniAlign = pdevLimits->minUniformBufferOffsetAlignment;
	qDebug("uniform buffer offset alignment is %u", (uint)uniAlign);
	VkBufferCreateInfo bufInfo;
	memset(&bufInfo, 0, sizeof(bufInfo));
	bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	// Our internal layout is vertex, uniform, uniform, ... with each uniform buffer start offset aligned to uniAlign.
	const VkDeviceSize vertexAllocSize = aligned(sizeof(vertexData), uniAlign);
	const VkDeviceSize uniformAllocSize = aligned(UNIFORM_DATA_SIZE, uniAlign);
	bufInfo.size = vertexAllocSize + concurrentFrameCount * uniformAllocSize;
	bufInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

	VkResult err = m_devFuncs->vkCreateBuffer(dev, &bufInfo, nullptr, &m_buf);
	if (err != VK_SUCCESS)
		qFatal("Failed to create buffer: %d", err);

	VkMemoryRequirements memReq;
	m_devFuncs->vkGetBufferMemoryRequirements(dev, m_buf, &memReq);

	VkMemoryAllocateInfo memAllocInfo = {
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		nullptr,
		memReq.size,
		Wrapped_Window->hostVisibleMemoryIndex()
	};

	err = m_devFuncs->vkAllocateMemory(dev, &memAllocInfo, nullptr, &m_bufMem);
	if (err != VK_SUCCESS)
		qFatal("Failed to allocate memory: %d", err);

	err = m_devFuncs->vkBindBufferMemory(dev, m_buf, m_bufMem, 0);
	if (err != VK_SUCCESS)
		qFatal("Failed to bind buffer memory: %d", err);

	quint8* p;
	err = m_devFuncs->vkMapMemory(dev, m_bufMem, 0, memReq.size, 0, reinterpret_cast<void**>(&p));
	if (err != VK_SUCCESS)
		qFatal("Failed to map memory: %d", err);
	memcpy(p, vertexData, sizeof(vertexData));
	QMatrix4x4 ident;
	memset(m_uniformBufInfo, 0, sizeof(m_uniformBufInfo));
	for (int i = 0; i < concurrentFrameCount; ++i) {
		const VkDeviceSize offset = vertexAllocSize + i * uniformAllocSize;
		memcpy(p + offset, ident.constData(), 16 * sizeof(float));
		m_uniformBufInfo[i].buffer = m_buf;
		m_uniformBufInfo[i].offset = offset;
		m_uniformBufInfo[i].range = uniformAllocSize;
	}
	m_devFuncs->vkUnmapMemory(dev, m_bufMem);

	VkVertexInputBindingDescription vertexBindingDesc = {
		0, // binding
		5 * sizeof(float),
		VK_VERTEX_INPUT_RATE_VERTEX
	};
	VkVertexInputAttributeDescription vertexAttrDesc[] = {
		{ // position
			0, // location
			0, // binding
			VK_FORMAT_R32G32_SFLOAT,
			0
		},
		{ // color
			1,
			0,
			VK_FORMAT_R32G32B32_SFLOAT,
			2 * sizeof(float)
		}
	};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.pNext = nullptr;
	vertexInputInfo.flags = 0;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &vertexBindingDesc;
	vertexInputInfo.vertexAttributeDescriptionCount = 2;
	vertexInputInfo.pVertexAttributeDescriptions = vertexAttrDesc;

	// Set up descriptor set and its layout.
	VkDescriptorPoolSize descPoolSizes = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, uint32_t(concurrentFrameCount) };
	VkDescriptorPoolCreateInfo descPoolInfo;
	memset(&descPoolInfo, 0, sizeof(descPoolInfo));
	descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descPoolInfo.maxSets = concurrentFrameCount;
	descPoolInfo.poolSizeCount = 1;
	descPoolInfo.pPoolSizes = &descPoolSizes;
	err = m_devFuncs->vkCreateDescriptorPool(dev, &descPoolInfo, nullptr, &m_descPool);
	if (err != VK_SUCCESS)
		qFatal("Failed to create descriptor pool: %d", err);

	VkDescriptorSetLayoutBinding layoutBinding = {
		0, // binding
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		1,
		VK_SHADER_STAGE_VERTEX_BIT,
		nullptr
	};
	VkDescriptorSetLayoutCreateInfo descLayoutInfo = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		nullptr,
		0,
		1,
		&layoutBinding
	};
	err = m_devFuncs->vkCreateDescriptorSetLayout(dev, &descLayoutInfo, nullptr, &m_descSetLayout);
	if (err != VK_SUCCESS)
		qFatal("Failed to create descriptor set layout: %d", err);

	for (int i = 0; i < concurrentFrameCount; ++i) {
		VkDescriptorSetAllocateInfo descSetAllocInfo = {
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			nullptr,
			m_descPool,
			1,
			&m_descSetLayout
		};
		err = m_devFuncs->vkAllocateDescriptorSets(dev, &descSetAllocInfo, &m_descSet[i]);
		if (err != VK_SUCCESS)
			qFatal("Failed to allocate descriptor set: %d", err);

		VkWriteDescriptorSet descWrite;
		memset(&descWrite, 0, sizeof(descWrite));
		descWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descWrite.dstSet = m_descSet[i];
		descWrite.descriptorCount = 1;
		descWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descWrite.pBufferInfo = &m_uniformBufInfo[i];
		m_devFuncs->vkUpdateDescriptorSets(dev, 1, &descWrite, 0, nullptr);
	}

	// Pipeline cache
	VkPipelineCacheCreateInfo pipelineCacheInfo;
	memset(&pipelineCacheInfo, 0, sizeof(pipelineCacheInfo));
	pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	err = m_devFuncs->vkCreatePipelineCache(dev, &pipelineCacheInfo, nullptr, &m_pipelineCache);
	if (err != VK_SUCCESS)
		qFatal("Failed to create pipeline cache: %d", err);

	// Pipeline layout
	VkPipelineLayoutCreateInfo pipelineLayoutInfo;
	memset(&pipelineLayoutInfo, 0, sizeof(pipelineLayoutInfo));
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &m_descSetLayout;
	err = m_devFuncs->vkCreatePipelineLayout(dev, &pipelineLayoutInfo, nullptr, &m_pipelineLayout);
	if (err != VK_SUCCESS)
		qFatal("Failed to create pipeline layout: %d", err);

	// Shaders
	VkShaderModule vertShaderModule = createShader(QStringLiteral("src/color_vert.spv"));
	VkShaderModule fragShaderModule = createShader(QStringLiteral("src/color_frag.spv"));

	// Graphics pipeline
	VkGraphicsPipelineCreateInfo pipelineInfo;
	memset(&pipelineInfo, 0, sizeof(pipelineInfo));
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

	VkPipelineShaderStageCreateInfo shaderStages[2] = {
		{
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			nullptr,
			0,
			VK_SHADER_STAGE_VERTEX_BIT,
			vertShaderModule,
			"main",
			nullptr
		},
		{
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			nullptr,
			0,
			VK_SHADER_STAGE_FRAGMENT_BIT,
			fragShaderModule,
			"main",
			nullptr
		}
	};
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;

	pipelineInfo.pVertexInputState = &vertexInputInfo;

	VkPipelineInputAssemblyStateCreateInfo ia;
	memset(&ia, 0, sizeof(ia));
	ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	pipelineInfo.pInputAssemblyState = &ia;

	// The viewport and scissor will be set dynamically via vkCmdSetViewport/Scissor.
	// This way the pipeline does not need to be touched when resizing the window.
	VkPipelineViewportStateCreateInfo vp;
	memset(&vp, 0, sizeof(vp));
	vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	vp.viewportCount = 1;
	vp.scissorCount = 1;
	pipelineInfo.pViewportState = &vp;

	VkPipelineRasterizationStateCreateInfo rs;
	memset(&rs, 0, sizeof(rs));
	rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rs.polygonMode = VK_POLYGON_MODE_FILL;
	rs.cullMode = VK_CULL_MODE_NONE; // we want the back face as well
	rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rs.lineWidth = 1.0f;
	pipelineInfo.pRasterizationState = &rs;

	VkPipelineMultisampleStateCreateInfo ms;
	memset(&ms, 0, sizeof(ms));
	ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	// Enable multisampling.
	ms.rasterizationSamples = Wrapped_Window->sampleCountFlagBits();
	pipelineInfo.pMultisampleState = &ms;

	VkPipelineDepthStencilStateCreateInfo ds;
	memset(&ds, 0, sizeof(ds));
	ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	ds.depthTestEnable = VK_TRUE;
	ds.depthWriteEnable = VK_TRUE;
	ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	pipelineInfo.pDepthStencilState = &ds;

	VkPipelineColorBlendStateCreateInfo cb;
	memset(&cb, 0, sizeof(cb));
	cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	// no blend, write out all of rgba
	VkPipelineColorBlendAttachmentState att;
	memset(&att, 0, sizeof(att));
	att.colorWriteMask = 0xF;
	cb.attachmentCount = 1;
	cb.pAttachments = &att;
	pipelineInfo.pColorBlendState = &cb;

	VkDynamicState dynEnable[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	VkPipelineDynamicStateCreateInfo dyn;
	memset(&dyn, 0, sizeof(dyn));
	dyn.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dyn.dynamicStateCount = sizeof(dynEnable) / sizeof(VkDynamicState);
	dyn.pDynamicStates = dynEnable;
	pipelineInfo.pDynamicState = &dyn;

	pipelineInfo.layout = m_pipelineLayout;
	pipelineInfo.renderPass = Wrapped_Window->defaultRenderPass();

	err = m_devFuncs->vkCreateGraphicsPipelines(dev, m_pipelineCache, 1, &pipelineInfo, nullptr, &m_pipeline);
	if (err != VK_SUCCESS)
		qFatal("Failed to create graphics pipeline: %d", err);

	if (vertShaderModule)
		m_devFuncs->vkDestroyShaderModule(dev, vertShaderModule, nullptr);
	if (fragShaderModule)
		m_devFuncs->vkDestroyShaderModule(dev, fragShaderModule, nullptr);
}

void R_Vulkan_Window_Viewport_Renderer::initSwapChainResources() {
	qDebug("initSwapChainResources");

	// Projection matrix
	m_proj = Wrapped_Window->clipCorrectionMatrix(); // adjust for Vulkan-OpenGL clip space differences
	const QSize sz = Wrapped_Window->swapChainImageSize();

	// CAMERA FOV
	m_proj.perspective(60.0f, sz.width() / (float)sz.height(), 0.01f, 100.0f);
	m_proj.translate(0, 0, -4);
}

void R_Vulkan_Window_Viewport_Renderer::releaseSwapChainResources() {
	qDebug("releaseSwapChainResources");
}

void R_Vulkan_Window_Viewport_Renderer::releaseResources() {
	qDebug("releaseResources");

	VkDevice dev = Wrapped_Window->device();

	if (m_pipeline) {
		m_devFuncs->vkDestroyPipeline(dev, m_pipeline, nullptr);
		m_pipeline = VK_NULL_HANDLE;
	}

	if (m_pipelineLayout) {
		m_devFuncs->vkDestroyPipelineLayout(dev, m_pipelineLayout, nullptr);
		m_pipelineLayout = VK_NULL_HANDLE;
	}

	if (m_pipelineCache) {
		m_devFuncs->vkDestroyPipelineCache(dev, m_pipelineCache, nullptr);
		m_pipelineCache = VK_NULL_HANDLE;
	}

	if (m_descSetLayout) {
		m_devFuncs->vkDestroyDescriptorSetLayout(dev, m_descSetLayout, nullptr);
		m_descSetLayout = VK_NULL_HANDLE;
	}

	if (m_descPool) {
		m_devFuncs->vkDestroyDescriptorPool(dev, m_descPool, nullptr);
		m_descPool = VK_NULL_HANDLE;
	}

	if (m_buf) {
		m_devFuncs->vkDestroyBuffer(dev, m_buf, nullptr);
		m_buf = VK_NULL_HANDLE;
	}

	if (m_bufMem) {
		m_devFuncs->vkFreeMemory(dev, m_bufMem, nullptr);
		m_bufMem = VK_NULL_HANDLE;
	}
}

void R_Vulkan_Window_Viewport_Renderer::startNextFrame() {
	VkDevice dev = Wrapped_Window->device();
	VkCommandBuffer cb = Wrapped_Window->currentCommandBuffer();
	const QSize sz = Wrapped_Window->swapChainImageSize();

	VkClearColorValue clearColor = { { 0.15, 0.15, 0.15, 1 } }; // BACKGROUND COLOR
	VkClearDepthStencilValue clearDS = { 1, 0 };
	VkClearValue clearValues[3];
	memset(clearValues, 0, sizeof(clearValues));
	clearValues[0].color = clearValues[2].color = clearColor;
	clearValues[1].depthStencil = clearDS;

	VkRenderPassBeginInfo rpBeginInfo;
	memset(&rpBeginInfo, 0, sizeof(rpBeginInfo));
	rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rpBeginInfo.renderPass = Wrapped_Window->defaultRenderPass();
	rpBeginInfo.framebuffer = Wrapped_Window->currentFramebuffer();
	rpBeginInfo.renderArea.extent.width = sz.width();
	rpBeginInfo.renderArea.extent.height = sz.height();
	rpBeginInfo.clearValueCount = Wrapped_Window->sampleCountFlagBits() > VK_SAMPLE_COUNT_1_BIT ? 3 : 2;
	rpBeginInfo.pClearValues = clearValues;
	VkCommandBuffer cmdBuf = Wrapped_Window->currentCommandBuffer();
	m_devFuncs->vkCmdBeginRenderPass(cmdBuf, &rpBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	quint8* p;
	VkResult err = m_devFuncs->vkMapMemory(dev, m_bufMem, m_uniformBufInfo[Wrapped_Window->currentFrame()].offset,
		UNIFORM_DATA_SIZE, 0, reinterpret_cast<void**>(&p));
	if (err != VK_SUCCESS)
		qFatal("Failed to map memory: %d", err);
	QMatrix4x4 m = m_proj;
	m.rotate(m_rotation, 0, 1, 0);
	memcpy(p, m.constData(), 16 * sizeof(float));
	m_devFuncs->vkUnmapMemory(dev, m_bufMem);

	// Not exactly a real animation system, just advance on every frame for now.
	m_rotation += 1.0f;

	m_devFuncs->vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
	m_devFuncs->vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1,
		&m_descSet[Wrapped_Window->currentFrame()], 0, nullptr);
	VkDeviceSize vbOffset = 0;
	m_devFuncs->vkCmdBindVertexBuffers(cb, 0, 1, &m_buf, &vbOffset);

	VkViewport viewport;
	viewport.x = viewport.y = 0;
	viewport.width = sz.width();
	viewport.height = sz.height();
	viewport.minDepth = 0;
	viewport.maxDepth = 1;
	m_devFuncs->vkCmdSetViewport(cb, 0, 1, &viewport);

	VkRect2D scissor;
	scissor.offset.x = scissor.offset.y = 0;
	scissor.extent.width = viewport.width;
	scissor.extent.height = viewport.height;
	m_devFuncs->vkCmdSetScissor(cb, 0, 1, &scissor);

	m_devFuncs->vkCmdDraw(cb, 3, 1, 0, 0);

	m_devFuncs->vkCmdEndRenderPass(cmdBuf);

	Wrapped_Window->frameReady();
	Wrapped_Window->requestUpdate(); // render continuously, throttled by the presentation rate
}
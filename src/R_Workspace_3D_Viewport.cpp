#include "R_Workspace_3D_Viewport.h"

R_Workspace_3D_Viewport::R_Workspace_3D_Viewport(RUI_Text_Stream* P_Log) : RUI_Linear_Contents("_Container", true) {
	Log = P_Log;

	QVulkanInstance* inst = new QVulkanInstance();
	inst->setLayers({ "VK_LAYER_KHRONOS_validation" });

	if (!inst->create())
		qFatal("Failed to create Vulkan instance: %d", inst->errorCode());

	R_Vulkan_Viewport_Window* vulkanWindow = new R_Vulkan_Viewport_Window();
	vulkanWindow->setVulkanInstance(inst);

	Vulkan_Widget = QWidget::createWindowContainer(vulkanWindow);
	Log->append("<p style = color:rgb(250,140,25);>3D Viewport Initialized</p>");
}
#pragma once

#include "QT_Core.h"
#include <QVulkanWindow>
#include <QVulkanFunctions>

struct R_Vulkan_Window_Viewport_Renderer : public QVulkanWindowRenderer {
	R_Vulkan_Window_Viewport_Renderer(QVulkanWindow* w, bool msaa = false);

	void initResources() override;
	void initSwapChainResources() override;
	void releaseSwapChainResources() override;
	void releaseResources() override;
	void startNextFrame() override;

protected:
	VkShaderModule createShader(const QString& name);

	QVulkanWindow* Wrapped_Window;
	QVulkanDeviceFunctions* m_devFuncs;

	VkDeviceMemory m_bufMem = VK_NULL_HANDLE;
	VkBuffer m_buf = VK_NULL_HANDLE;
	VkDescriptorBufferInfo m_uniformBufInfo[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];

	VkDescriptorPool m_descPool = VK_NULL_HANDLE;
	VkDescriptorSetLayout m_descSetLayout = VK_NULL_HANDLE;
	VkDescriptorSet m_descSet[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];

	VkPipelineCache m_pipelineCache = VK_NULL_HANDLE;
	VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
	VkPipeline m_pipeline = VK_NULL_HANDLE;

	QMatrix4x4 m_proj;
	float m_rotation = 0.0f;
};

struct R_Vulkan_Viewport_Window;

QT_BEGIN_NAMESPACE
struct QTabWidget;
struct QPlainTextEdit;
struct QLCDNumber;
QT_END_NAMESPACE

struct R_Vulkan_Viewport_Widget : public QWidget {
	Q_OBJECT

public:
	QWidget* Window_Wrapper;
public:
	explicit R_Vulkan_Viewport_Widget(R_Vulkan_Viewport_Window* w);

public slots:
	void onGrabRequested();


public:
	R_Vulkan_Viewport_Window* Wrapped_Window;
};

struct R_Vulkan_Viewport_Renderer : public R_Vulkan_Window_Viewport_Renderer {
public:
	R_Vulkan_Viewport_Renderer(R_Vulkan_Viewport_Window* w);

	void initResources() override;
	void startNextFrame() override;
};

struct R_Vulkan_Viewport_Window : public QVulkanWindow {
	Q_OBJECT

public:
	QVulkanWindowRenderer* createRenderer() override;

signals:
	void vulkanInfoReceived(const QString& text);
	void frameQueued(int colorValue);
};

struct R_Workspace_Vulkan_Viewport : QT_Linear_Contents {
	QT_Text_Stream* Log;
	QWidget* Vulkan_Widget;

	R_Workspace_Vulkan_Viewport(QT_Text_Stream* P_Log);
};
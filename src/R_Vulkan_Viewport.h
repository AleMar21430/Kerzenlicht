#pragma once

#include "R_Renderer_Viewport.h"

#include <QVulkanFunctions>
#include <QFileDialog>
#include <QWidget>

struct R_Vulkan_Viewport_Window;

QT_BEGIN_NAMESPACE
struct QTabWidget;
struct QPlainTextEdit;
struct QLCDNumber;
QT_END_NAMESPACE

struct R_Vulkan_Viewport_Widget : public QWidget {
	Q_OBJECT

public:
	QWidget *Window_Wrapper;
public:
	explicit R_Vulkan_Viewport_Widget(R_Vulkan_Viewport_Window *w);

public slots:
	void onGrabRequested();


public:
	R_Vulkan_Viewport_Window *Wrapped_Window;
};

struct R_Vulkan_Viewport_Renderer : public R_Vulkan_Window_Viewport_Renderer {
public:
	R_Vulkan_Viewport_Renderer(R_Vulkan_Viewport_Window *w);

	void initResources() override;
	void startNextFrame() override;
};

struct R_Vulkan_Viewport_Window : public QVulkanWindow {
	Q_OBJECT

public:
	QVulkanWindowRenderer *createRenderer() override;

signals:
	void vulkanInfoReceived(const QString &text);
	void frameQueued(int colorValue);
};
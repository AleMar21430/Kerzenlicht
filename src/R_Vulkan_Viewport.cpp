#include "R_Vulkan_Viewport.h"

R_Vulkan_Viewport_Widget::R_Vulkan_Viewport_Widget(R_Vulkan_Viewport_Window *w) : Wrapped_Window(w) {
	Window_Wrapper = QWidget::createWindowContainer(w);

	//QPushButton *grabButton = new QPushButton(tr("&Grab"));
	//connect(grabButton, &QPushButton::clicked, this, &R_VulkanWidget::onGrabRequested);
}

void R_Vulkan_Viewport_Widget::onGrabRequested() {

	QImage img = Wrapped_Window->grab();

	// Our startNextFrame() implementation is synchronous so img is ready to be
	// used right here.

	QFileDialog fd(this);
	fd.setAcceptMode(QFileDialog::AcceptSave);
	fd.setDefaultSuffix("png");
	fd.selectFile("test.png");
	if (fd.exec() == QDialog::Accepted)
		img.save(fd.selectedFiles().first());
}

QVulkanWindowRenderer *R_Vulkan_Viewport_Window::createRenderer() {
	return new R_Vulkan_Viewport_Renderer(this);
}

R_Vulkan_Viewport_Renderer::R_Vulkan_Viewport_Renderer(R_Vulkan_Viewport_Window *w) : R_Vulkan_Window_Viewport_Renderer(w,true) {
}

void R_Vulkan_Viewport_Renderer::initResources() {
	R_Vulkan_Window_Viewport_Renderer::initResources();
}

void R_Vulkan_Viewport_Renderer::startNextFrame() {
	R_Vulkan_Window_Viewport_Renderer::startNextFrame();
	emit static_cast<R_Vulkan_Viewport_Window *>(Wrapped_Window)->frameQueued(int(m_rotation) % 360);
}
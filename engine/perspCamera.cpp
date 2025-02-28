#include "perspCamera.h"

ENG_API PerspCamera::PerspCamera(const std::string& name, const float& width, const float& height, const float& near_val, const float& far_val, const float& fovY)
	: Camera(name, width, height, near_val, far_val), m_fovY{ fovY }
{
	this->loadProjectionMatrix();
	this->m_notificationService.subscribeListener(this);
}

ENG_API PerspCamera::PerspCamera(const PerspCamera& other) : Camera(other), m_fovY{ other.m_fovY } {
	this->m_notificationService.subscribeListener(this);
}

ENG_API Node* PerspCamera::clone() const {
	PerspCamera* newPerspCamera = new PerspCamera(*this);
	this->recursiveClone(newPerspCamera);
	return newPerspCamera;
}

ENG_API void PerspCamera::loadProjectionMatrix() {
	this->m_projectionMatrix = glm::perspective(m_fovY, m_width / m_height, m_near, m_far);
}
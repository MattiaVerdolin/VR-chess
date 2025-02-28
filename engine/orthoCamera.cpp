#include "orthoCamera.h"

ENG_API OrthoCamera::OrthoCamera(const std::string& name, const float& width, const float& height, const float& near_val, const float& far_val)
	:  Camera(name, width, height, near_val, far_val), m_left{0.0f}, m_bottom{0.0f}
{
	this->m_notificationService.subscribeListener(this);
	this->loadProjectionMatrix();
	this->m_notificationService.subscribeListener(this);
}

ENG_API OrthoCamera::OrthoCamera(const OrthoCamera& other) : Camera(other), m_left{other.m_left}, m_bottom{other.m_bottom} 
{
	this->m_notificationService.subscribeListener(this);
}

ENG_API Node* OrthoCamera::clone() const {
	OrthoCamera* newOrthoCamera = new OrthoCamera(*this);
	this->recursiveClone(newOrthoCamera);
	return newOrthoCamera;
}

void ENG_API OrthoCamera::loadProjectionMatrix() {
	this->m_projectionMatrix = glm::ortho(this->m_left, this->m_width, this->m_bottom, this->m_height);
}
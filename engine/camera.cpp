#include "camera.h"
#include "GL/freeglut.h"


ENG_API Camera::Camera(const std::string& name, const float& width, const float& height, const float& near_val, const float& far_val)
	: Node(name), m_projectionMatrix{ glm::mat4(1.0f) }, m_isCurrentCamera{ false }, m_width{ width }, m_height{ height }, m_near{ near_val }, m_far{ far_val }, m_notificationService{ NotificationService::getInstance()} {}

ENG_API Camera::Camera(const Camera& other) : Node(other), m_projectionMatrix{other.m_projectionMatrix}, m_isCurrentCamera{other.m_isCurrentCamera},
	m_width{other.m_width}, m_height{other.m_height}, m_near{other.m_near}, m_far{other.m_far}, m_notificationService{NotificationService::getInstance()} { }

ENG_API Camera::~Camera() {
	this->m_notificationService.unsubscribeListener(this);
}

const ENG_API glm::mat4 Camera::getInverseCameraFinalMatrix() const {
	return glm::inverse(this->getFinalMatrix());
}

void ENG_API Camera::setMainCamera(const bool& val){
	this->m_isCurrentCamera = val;
}

const ENG_API bool& Camera::isMainCamera() const {
	return this->m_isCurrentCamera;
}

void ENG_API Camera::render(const glm::mat4& matrix) {
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(this->m_projectionMatrix));
	glMatrixMode(GL_MODELVIEW);
}

void ENG_API Camera::onWindowReshape(int width, int height, Shader* shader, int projLoc) {
	glViewport(0, 0, width, height);
	this->m_width = (float) width;
	this->m_height = (float) height;
	this->loadProjectionMatrix();
	shader->setMatrix(projLoc, m_projectionMatrix); //sicuramente sbagliato
}
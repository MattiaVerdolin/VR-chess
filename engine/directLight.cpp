#include "directLight.h"

ENG_API DirectLight::DirectLight(const std::string& name, const glm::vec3& position) : Light(name, glm::vec4(position, 0.0f)) {}

ENG_API DirectLight::DirectLight(const DirectLight& other) : Light(other) {}

ENG_API Node* DirectLight::clone() const {
	DirectLight* newDirectLight = new DirectLight(*this);
	this->recursiveClone(newDirectLight);
	return newDirectLight;
}

void ENG_API DirectLight::setPosition(const glm::vec3& position) {
	this->m_position = glm::vec4(position, 0.0f);
}

void ENG_API DirectLight::render(const glm::mat4& matrix) {
	Light::render(matrix);

	Light::lightActiveCounter++;
}
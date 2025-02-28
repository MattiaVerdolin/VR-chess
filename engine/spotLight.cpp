#include "spotLight.h"

#include <GL/freeglut.h>


ENG_API SpotLight::SpotLight(const std::string& name, const glm::vec3& position, const glm::vec3& direction, const float& cutoff, const float& spotExponent)
	: Light(name, glm::vec4(position, 1.0f)), m_direction{ direction }, m_cutoff{ cutoff }, m_spotExponent{ spotExponent }
{
	if (cutoff < 0.0f || cutoff > 90.0f)
		this->m_cutoff = 45.0f;
}

ENG_API SpotLight::SpotLight(const SpotLight& other) : Light(other), m_direction{ other.m_direction }, m_cutoff{ other.m_cutoff } {}

ENG_API Node* SpotLight::clone() const {
	SpotLight* newSpotLight = new SpotLight(*this);
	this->recursiveClone(newSpotLight);
	return newSpotLight;
}

ENG_API void SpotLight::setDirection(const glm::vec3& direction) {
	this->m_direction = direction;
}

const ENG_API glm::vec3& SpotLight::getDirection() const {
	return this->m_direction;
}

ENG_API void SpotLight::setCutoff(const float& cutoff) {
	if (cutoff >= 0.0f && cutoff <= 90.0f)
		this->m_cutoff = cutoff;
}

const ENG_API float& SpotLight::getCutoff() const {
	return this->m_cutoff;
}

ENG_API void SpotLight::setSpotExponent(const float& spotExponent) {
	this->m_spotExponent = spotExponent;
}

const ENG_API float& SpotLight::getSpotExponent() const {
	return this->m_spotExponent;
}

void ENG_API SpotLight::render(const glm::mat4& matrix) {
	Light::render(matrix);

	glLightfv(Light::lightActiveCounter, GL_SPOT_CUTOFF, &this->getCutoff());
	glLightfv(Light::lightActiveCounter, GL_SPOT_DIRECTION, glm::value_ptr(glm::vec4(this->getDirection(), 1.0f)));
	glLightfv(Light::lightActiveCounter, GL_SPOT_EXPONENT, &this->getSpotExponent());

	Light::lightActiveCounter++;
}

const ENG_API unsigned int SpotLight::parse(const char* data, unsigned int& position) {
	const unsigned int& children = Light::parse(data, position);

	// Direction:
	glm::vec3 direction;
	memcpy(&direction, data + position, sizeof(glm::vec3));
	position += sizeof(glm::vec3);

	this->setDirection(direction);

	//cutoff
	float cutoff;
	memcpy(&cutoff, data + position, sizeof(float));
	position += sizeof(float);
	this->setCutoff(cutoff);

	float spotExponent;
	memcpy(&spotExponent, data + position, sizeof(float));
	position += sizeof(float);
	this->setSpotExponent(spotExponent);

	return children;
}
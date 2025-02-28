#include "omniLight.h"
#include <GL/freeglut.h>

ENG_API OmniLight::OmniLight(const std::string& name, const glm::vec3& position) : Light(name, glm::vec4(position, 1.0f)), m_cutoff{ 180.0f } {}

ENG_API OmniLight::OmniLight(const OmniLight& other) : Light(other), m_cutoff{other.m_cutoff} {}

ENG_API Node* OmniLight::clone() const {
    OmniLight* newOmniLight = new OmniLight(*this);
    this->recursiveClone(newOmniLight);
    return newOmniLight;
}

const ENG_API float& OmniLight::getCutoff() const {
	return this->m_cutoff;
}

void ENG_API OmniLight::render(const glm::mat4& matrix) {
    Light::render(matrix);

    glLightfv(Light::lightActiveCounter, GL_SPOT_CUTOFF, &this->m_cutoff);

    Light::lightActiveCounter++;
} 
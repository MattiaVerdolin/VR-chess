#include "GL/freeglut.h"
#include "light.h"

glm::vec3 deltaIncrement{ glm::vec3(0.1f,0.1f,0.1f) };

unsigned int Light::lightActiveCounter{ GL_LIGHT0 };
unsigned int Light::maxNumberOfActiveLights{ GL_LIGHT0 + 7 };

ENG_API Light::Light(const std::string& name, const glm::vec4& position) : Node(name), m_position{ position }, m_lightMaterial{ nullptr }, m_tempAlbedo{ glm::vec3(0.0f) }, m_lightIsOn{ true } {}

ENG_API Light::Light(const Light& other) : Node(other), m_position{ other.m_position }, m_lightMaterial{ other.m_lightMaterial == nullptr ? other.m_lightMaterial : new Material(*other.m_lightMaterial) }, m_tempAlbedo{ other.m_tempAlbedo }, m_lightIsOn{ other.m_lightIsOn } {}

ENG_API void Light::setPosition(const glm::vec3& position) {
	this->m_position = glm::vec4(position, 1.0f);
}

const ENG_API glm::vec4& Light::getPosition() const {
	return this->m_position;
}

ENG_API void Light::resetLightCounter() {
	for (unsigned int lightCounter = GL_LIGHT0; lightCounter < Light::lightActiveCounter; lightCounter++) glDisable(lightCounter);
	Light::lightActiveCounter = GL_LIGHT0;
}

ENG_API void Light::turnOn() {
	this->m_lightMaterial->setDiffuse(this->m_tempAlbedo);
	this->m_lightMaterial->setAmbient(this->m_tempAlbedo);
	this->m_lightMaterial->setSpecular(this->m_tempAlbedo);
	this->m_lightIsOn = true;
}

ENG_API void Light::turnOff() {
	this->m_tempAlbedo = this->m_lightMaterial->getDiffuse();
	this->m_lightMaterial->setDiffuse(glm::vec3(0.0f));
	this->m_lightMaterial->setAmbient(glm::vec3(0.0f));
	this->m_lightMaterial->setSpecular(glm::vec3(0.0f));
	this->m_lightIsOn = false;
}

ENG_API void Light::increaseIntensity() {
	glm::vec3 diffuse = this->m_lightMaterial->getDiffuse();
	if (diffuse.x >= 10.0) return;
	diffuse += deltaIncrement;
	this->m_lightMaterial->setDiffuse(diffuse);
	this->m_lightMaterial->setAmbient(diffuse);
	this->m_lightMaterial->setSpecular(diffuse);
}

ENG_API void Light::decreaseIntensity() {
	glm::vec3 diffuse = this->m_lightMaterial->getDiffuse();
	if (diffuse.x <= 0.1) return;
	diffuse -= deltaIncrement;
	this->m_lightMaterial->setDiffuse(diffuse);
	this->m_lightMaterial->setAmbient(diffuse);
	this->m_lightMaterial->setSpecular(diffuse);
}

ENG_API const bool& Light::isLightOn() const {
	return this->m_lightIsOn;
}

ENG_API void Light::render(const glm::mat4& matrix) {
	if (this->m_lightMaterial == nullptr) return;
	if (Light::lightActiveCounter > Light::maxNumberOfActiveLights) return;

	glEnable(Light::lightActiveCounter);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(matrix));

	glLightfv(Light::lightActiveCounter, GL_AMBIENT, glm::value_ptr(this->m_lightMaterial->getAmbient()));
	glLightfv(Light::lightActiveCounter, GL_DIFFUSE, glm::value_ptr(this->m_lightMaterial->getDiffuse()));
	glLightfv(Light::lightActiveCounter, GL_SPECULAR, glm::value_ptr(this->m_lightMaterial->getSpecular()));
	glLightfv(Light::lightActiveCounter, GL_POSITION, glm::value_ptr(this->getPosition()));
}

const ENG_API unsigned int Light::parse(const char* data, unsigned int& position) {
	unsigned int children = Node::parse(data, position);

	//subtype
	position += sizeof(unsigned char);

	// Light color:
	glm::vec3 color;
	memcpy(&color, data + position, sizeof(glm::vec3));
	position += sizeof(glm::vec3);

	this->m_lightMaterial = new Material();
	this->m_lightMaterial->setAmbient(color);
	this->m_lightMaterial->setDiffuse(color);
	this->m_lightMaterial->setSpecular(color);

	//Radius
	position += sizeof(float);


	return children;
}

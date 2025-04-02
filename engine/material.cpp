#include "material.h"
#include <GL/freeglut.h>

std::map<std::string, Texture*> Material::m_texturesMap{ std::map<std::string, Texture*>() };

ENG_API Material::Material(const std::string& name)
	: Object(name), m_alpha{ 1.0f }, m_emission{ glm::vec3(0.0f) }, m_ambient{ glm::vec3(0.0f) }, m_specular{ glm::vec3(0.0f) },
	m_diffuse{ glm::vec3(0.0f) }, m_shininess{ 1.0f }, m_texture{ nullptr } {}

ENG_API Material::Material(const Material& other) : Object(other), m_alpha{ other.m_alpha }, m_emission{ other.m_emission }, m_ambient{ other.m_ambient }, m_specular{ other.m_specular },
m_diffuse{ other.m_diffuse }, m_shininess{ other.m_shininess }, m_texture{ other.m_texture } {}

ENG_API void Material::setAlpha(const float& alpha) {
	if (alpha >= 0.0f && alpha <= 1.0f)
		this->m_alpha = alpha;
}

const ENG_API float& Material::getAlpha() const {
	return this->m_alpha;
}

ENG_API void Material::setEmission(const glm::vec3& emission) {
	this->m_emission = emission;
}

const ENG_API glm::vec3& Material::getEmission() const {
	return this->m_emission;
}

ENG_API void Material::setAmbient(const glm::vec3& ambient) {
	this->m_ambient = ambient;
}

const ENG_API glm::vec3& Material::getAmbient() const {
	return this->m_ambient;
}

ENG_API void Material::setSpecular(const glm::vec3& specular) {
	this->m_specular = specular;
}

const ENG_API glm::vec3& Material::getSpecular() const {
	return this->m_specular;
}

ENG_API void Material::setDiffuse(const glm::vec3& diffuse) {
	this->m_diffuse = diffuse;
}

const ENG_API glm::vec3& Material::getDiffuse() const {
	return this->m_diffuse;
}

ENG_API void Material::setShininess(const float& shininess) {
	if (shininess >= 0.0f)
		this->m_shininess = shininess;
}

const ENG_API float& Material::getShininess() const {
	return this->m_shininess;
}

ENG_API void Material::setTexture(Texture* texture) {
	this->m_texture = texture;
}

const ENG_API Texture* Material::getTexture() const {
	return this->m_texture; 
}

ENG_API void Material::render(const glm::mat4& matrix) {
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(glm::vec4(this->getEmission(), this->getAlpha())));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, powf(2.0f, this->getShininess()));
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(glm::vec4(this->getAmbient(), this->getAlpha())));
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(glm::vec4(this->getDiffuse(), this->getAlpha())));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(glm::vec4(this->getSpecular(), this->getAlpha())));

	if (this->m_texture != nullptr)
		this->m_texture->render();
}


const ENG_API unsigned int Material::parse(const char* data, unsigned int& position) {
	char materialName[FILENAME_MAX];
	strncpy(materialName, data + position, sizeof(materialName) - 1);
	this->setName(materialName);
	position += (unsigned int)strlen(materialName) + 1;

	// Material term colors, starting with emissive:
	glm::vec3 emission, albedo;
	memcpy(&emission, data + position, sizeof(glm::vec3));
	position += sizeof(glm::vec3);

	// Albedo:
	memcpy(&albedo, data + position, sizeof(glm::vec3));

	position += sizeof(glm::vec3);

	// Roughness factor:
	float roughness;
	memcpy(&roughness, data + position, sizeof(float));
	this->setShininess((1 - sqrt(roughness)) * 128);
	position += sizeof(float);

	// Metalness factor:
	float metalness;
	memcpy(&metalness, data + position, sizeof(float));
	position += sizeof(float);

	// Transparency factor:
	float alpha;
	memcpy(&alpha, data + position, sizeof(float));
	position += sizeof(float);

	// Albedo texture filename, or [none] if not used:
	char textureName[FILENAME_MAX];
	strncpy(textureName, data + position, sizeof(textureName) - 1);
	position += (unsigned int)strlen(textureName) + 1;

	if (strcmp(textureName, "[none]") != 0) {
		Texture* textureInMap = this->getTexture(textureName);
		if (textureInMap == nullptr) {
			textureInMap = new Texture(textureName);
			textureInMap->parse(data, position);
			Material::m_texturesMap[textureName] = textureInMap;
		}
		this->setTexture(textureInMap);
	}
	else {
		const std::string defaultKey = "default_white";
		Texture* defaultTexture = this->getTexture(defaultKey);
		if (defaultTexture == nullptr) {
			defaultTexture = new Texture(defaultKey);
			defaultTexture->createWhiteTexture();
			Material::m_texturesMap[defaultKey] = defaultTexture;
		}
		this->setTexture(defaultTexture);
	}



	this->setAlpha(alpha);
	this->setEmission(emission);
	this->setAmbient(albedo * 0.2f);
	this->setSpecular(albedo * 0.4f);
	this->setDiffuse(albedo * 0.6f);

	return 0;
}

ENG_API Texture* Material::getTexture(const std::string& textureName) const {
	auto it = Material::m_texturesMap.find(textureName);
	if (it != Material::m_texturesMap.end()) {
		return it->second;
	}
	return nullptr;
}

ENG_API void Material::setEnableTexture() {
	if (this->m_texture != nullptr)
		glEnable(GL_TEXTURE_2D);

}

ENG_API void Material::setDisableTexture() {
	if (this->m_texture != nullptr)
		glDisable(GL_TEXTURE_2D);

}

ENG_API bool Material::isTextureExists() {
	return this->m_texture != nullptr;
}


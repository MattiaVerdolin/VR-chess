#include "lightManager.h"
#include "gameManager.h"

LightManager::LightManager(){}

LightManager& LightManager::getInstance() {
	static LightManager instance;
	return instance;
}

void LightManager::pressButton(const std::string& lightName, Node* parent) {
	if (parent == nullptr) parent = GameManager::getRootNode();
	Light* light = dynamic_cast<Light*>(const_cast<Node*>(parent->findNodeByName(lightName)));

	if (light == nullptr) return;

	if (light->isLightOn())
		light->turnOff();
	else
		light->turnOn();
}

void LightManager::increaseLight(const std::string& lightName, Node* parent) {
	if (parent == nullptr) parent = GameManager::getRootNode();

	Light* light = dynamic_cast<Light*>(const_cast<Node*>(parent->findNodeByName(lightName)));
	if (light == nullptr) return;

	light->increaseIntensity();
}

void LightManager::decreaseLight(const std::string& lightName, Node* parent) {
	if (parent == nullptr) parent = GameManager::getRootNode();

	Light* light = dynamic_cast<Light*>(const_cast<Node*>(parent->findNodeByName(lightName)));
	if (light == nullptr) return;

	light->decreaseIntensity();
}
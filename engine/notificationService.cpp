#include "notificationService.h"

ENG_API NotificationService::NotificationService() : m_reshapeListeners{ std::vector<ReshapeListener*>() }, m_changeMatrixListeners{ std::vector<ChangeMatrixListener*>() } {}

ENG_API NotificationService::~NotificationService() {
	m_reshapeListeners.clear();
	m_changeMatrixListeners.clear();
}

ENG_API NotificationService& NotificationService::getInstance() {
	static NotificationService instance;
	return instance;
}

ENG_API void NotificationService::subscribeListener(ReshapeListener* listener) {
	if (listener != nullptr) this->m_reshapeListeners.push_back(listener);
}

ENG_API void NotificationService::subscribeListener(ChangeMatrixListener* listener) {
	if (listener != nullptr) this->m_changeMatrixListeners.push_back(listener);
}

ENG_API void NotificationService::unsubscribeListener(ReshapeListener* listener) {
	if (listener != nullptr) this->m_reshapeListeners.erase(std::remove(this->m_reshapeListeners.begin(), this->m_reshapeListeners.end(), listener), this->m_reshapeListeners.end());
}

ENG_API void NotificationService::unsubscribeListener(ChangeMatrixListener* listener) {
	if (listener != nullptr) this->m_changeMatrixListeners.erase(std::remove(this->m_changeMatrixListeners.begin(), this->m_changeMatrixListeners.end(), listener), this->m_changeMatrixListeners.end());
}

ENG_API void NotificationService::notifyOnReshapeWindow(int width, int height, Shader* shader) const{
	for (auto* listener : this->m_reshapeListeners)
		listener->onWindowReshape(width, height, shader);
}

ENG_API void NotificationService::notifyOnChangeMatrix(const unsigned int& nodeID) const {
	for (auto* listener : this->m_changeMatrixListeners)
		listener->onMatrixChange(nodeID);
}
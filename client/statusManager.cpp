#include "statusManager.h"

struct StatusManager::Reserved
{
	void (*keyboardCallback)(unsigned char, int, int);
	void (*specialKeyCallback)(int, int, int);
	std::list<std::string> menu;

	Reserved() : keyboardCallback(nullptr), specialKeyCallback(nullptr), menu(std::list<std::string>()) {}
};

StatusManager::StatusManager() : m_reserved{ std::map<GameStatus, StatusManager::Reserved>() }, m_currentState{GameStatus::PRE_GAME}, 
m_listener{std::map<GameStatus, std::list<OnStateUpdateListener*>>()}, m_engine{Eng::Base::getInstance()}
{
	this->m_listener[GameStatus::PRE_GAME] = std::list<OnStateUpdateListener*>();
	this->m_listener[GameStatus::GAME] = std::list<OnStateUpdateListener*>();
	this->m_listener[GameStatus::END_GAME] = std::list<OnStateUpdateListener*>();
}

StatusManager::~StatusManager() = default;

StatusManager& StatusManager::getInstance() {
	static StatusManager instance;
	return instance;
}

void StatusManager::subscribeListener(GameStatus gameState, OnStateUpdateListener* listener) {
	if (listener == nullptr) return;

	this->m_listener[gameState].push_back(listener);
}

void StatusManager::unsubscribeListener(OnStateUpdateListener* listener) {
	if (listener == nullptr) return;

	std::vector<GameStatus> keysToErase;

	for (auto& [gameStatus, listeners] : m_listener) {
		listeners.remove(listener);

		if (listeners.empty())
			keysToErase.push_back(gameStatus);
	}

	for (const auto& key : keysToErase)
		m_listener.erase(key);
	
}

void StatusManager::addGameStatusAndCallbacks(const GameStatus& gameStatus, void (*keyboardCallback)(unsigned char, int, int),
		void (*specialKeyCallback)(int, int, int), const std::list<std::string> menu) {
	Reserved reserved;
	reserved.keyboardCallback = keyboardCallback;
	reserved.specialKeyCallback = specialKeyCallback;
	reserved.menu = menu;

	this->m_reserved[gameStatus] = reserved;
}

void StatusManager::changeState(const GameStatus& gameStatus) {
	this->m_engine.setKeyboardCallback(this->getKeyboardCallback(gameStatus) == nullptr ? [](unsigned char, int, int) {} : this->getKeyboardCallback(gameStatus));
	this->m_engine.setSpecialCallback(this->getSpecialKeyCallback(gameStatus) == nullptr ? [](int, int, int) {} : this->getSpecialKeyCallback(gameStatus));

	for (auto* listener : this->m_listener[gameStatus]) listener->onStateChangeUpdate(gameStatus);

	this->m_currentState = gameStatus;
}

void (*StatusManager::getKeyboardCallback(const GameStatus& gameStatus))(unsigned char, int, int) {
	if (this->m_reserved.count(gameStatus))
		return this->m_reserved[gameStatus].keyboardCallback;
    return nullptr;
}

void (*StatusManager::getSpecialKeyCallback(const GameStatus& gameStatus))(int, int, int) {
	if (this->m_reserved.count(gameStatus))
		return this->m_reserved[gameStatus].specialKeyCallback;
    return nullptr;
}

const std::list<std::string> StatusManager::getMenu() const {
	auto it = this->m_reserved.find(this->m_currentState);
	if (it != this->m_reserved.end())
		return it->second.menu;
	return { };
}

GameStatus StatusManager::getCurrentGameStatus() const {
	return this->m_currentState;
}

#include "onStateUpdate.h"

OnStateUpdateListener::OnStateUpdateListener() : m_mapFunctionOnState{ std::map<GameStatus, std::function<void()>>() } 
{
	this->m_mapFunctionOnState[GameStatus::PRE_GAME] = [this]() {this->preGameHandler();};
	this->m_mapFunctionOnState[GameStatus::CHOICE] = [this]() {this->choiceHandler();};
	this->m_mapFunctionOnState[GameStatus::GAME] = [this]() {this->gameHandler();};
	this->m_mapFunctionOnState[GameStatus::END_GAME] = [this]() {this->endGameHandler();};
}

void OnStateUpdateListener::onStateChangeUpdate(GameStatus gameState) {
	auto it = this->m_mapFunctionOnState.find(gameState);
	if (it != this->m_mapFunctionOnState.end() && it->second != nullptr)
		it->second();
}

void OnStateUpdateListener::preGameHandler() {}
void OnStateUpdateListener::choiceHandler() {}
void OnStateUpdateListener::gameHandler() {}
void OnStateUpdateListener::endGameHandler() {}
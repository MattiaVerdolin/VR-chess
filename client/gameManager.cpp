#include "gameManager.h"
#include <thread>
#include <chrono>
#include <filesystem>

struct GameManager::Reserved
{
	CameraManager& cameraManager;
	StatusManager& statusManager;
	MovementManager& movementManager;
	ListOfPiecesManager& listOfPiecesManager;
	HistoryManager& historyManager;
	LightManager& lightManager;
	Eng::Base& engine;
	Node* rootNode;
	Node* rootResetNode;
	bool isApplicationRunning;

	Reserved() : cameraManager{ CameraManager::getInstance() },
		statusManager{ StatusManager::getInstance() },
		movementManager{ MovementManager::getInstance() },
		listOfPiecesManager{ ListOfPiecesManager::getInstance() },
		historyManager{ HistoryManager::getInstance() },
		lightManager{ LightManager::getInstance() },
		engine{ Eng::Base::getInstance() },
		rootNode{ nullptr },
		rootResetNode{ nullptr },
		isApplicationRunning{false}
	{
	}
};

GameManager::GameManager() : m_reserved(std::make_unique<GameManager::Reserved>()) { this->initialize(); }

GameManager::~GameManager() {
	this->m_reserved->engine.free();
	delete this->m_reserved->rootResetNode;
	delete this->m_reserved->rootNode;
}

GameManager& GameManager::getInstance() {
	static GameManager instance;
	return instance;
}

void GameManager::setRootNode(Node* rootNode) {
	if (rootNode == nullptr)
		return;
	getInstance().m_reserved->rootNode = rootNode;
}

Node* GameManager::getRootNode() {
	return getInstance().m_reserved->rootNode;
}


//------------------------------------------------------------------------------------------------------------------------
void GameManager::keyboardCallbackPreGame(unsigned char key, int mouseX, int mouseY)
{
	switch (key)
	{
	case 32: // Change camera
		this->m_reserved->statusManager.changeState(GameStatus::CHOICE);
		this->createGraphicsList();
		break;
	case 'l':
	case 'L':
		this->m_reserved->lightManager.pressButton(LAMP_OMNI);
		break;
	}
}

void GameManager::specialKeyCallbackPreGame(int key, int mouseX, int mouseY)
{
	switch (key) {
	case 100: // Left arrow
		this->m_reserved->cameraManager.moveCameraLeft();
		break;
	case 101: // Up arrow
		this->m_reserved->cameraManager.moveCameraUp();
		break;
	case 102: // Right arrow
		this->m_reserved->cameraManager.moveCameraRight();
		break;
	case 103: // Dowm arrow
		this->m_reserved->cameraManager.moveCameraDown();
		break;
	}
}

std::list<std::string> GameManager::menuPreGame() {
	std::list<std::string> menu;
	menu.push_back("[Spacebar] Go to cheesboard view");
	menu.push_back("[Arrow key] Move camera");
	menu.push_back("[L] Turn on / off light");
	return menu;
}
//------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------
void GameManager::keyboardCallbackChoice(unsigned char key, int mouseX, int mouseY)
{
	switch (key)
	{
	case 13:
		this->m_reserved->listOfPiecesManager.confirmChoice();
		this->m_reserved->statusManager.changeState(GameStatus::GAME);
		this->createGraphicsList();
		break;
	case 27:
		this->resetGame();
		this->m_reserved->statusManager.changeState(GameStatus::PRE_GAME);
		this->createGraphicsList();
		break;
	case 'f':
	case 'F':
		this->m_reserved->statusManager.changeState(GameStatus::END_GAME);
		this->createGraphicsList();
		break;
	case 'u':
	case 'U':
		if (this->m_reserved->historyManager.undo()) {
			this->m_reserved->statusManager.changeState(GameStatus::CHOICE);
			this->createGraphicsList();
		}
		break;
	case 'r':
	case 'R':
		if (this->m_reserved->historyManager.redo()) {
			this->m_reserved->statusManager.changeState(GameStatus::CHOICE);
			this->createGraphicsList();
		}
		break;
	case 'l':
	case 'L':
		this->m_reserved->lightManager.pressButton(LAMP_OMNI);
		break;
	case '+':
		this->m_reserved->lightManager.increaseLight(CHESSBOARD_SPOT);
		break;
	case '-':
		this->m_reserved->lightManager.decreaseLight(CHESSBOARD_SPOT);
		break;
	}
}

void GameManager::specialKeyCallbackChoice(int key, int mouseX, int mouseY)
{
	switch (key) {
	case 100: // Left arrow
		this->m_reserved->listOfPiecesManager.moveChooseNodeLeft();
		this->createGraphicsList();
		break;
	case 102: // Right arrow
		this->m_reserved->listOfPiecesManager.moveChooseNodeRight();
		this->createGraphicsList();
		break;
	}
}

std::list<std::string> GameManager::menuChoice() {
	std::list<std::string> menu;
	menu.push_back("[ESC] Reset game and go to pre start");
	menu.push_back("[ENTER] Confirm choice");
	menu.push_back("[Left / Right Arrow Key] Move choice selector");
	menu.push_back("[U] Undo");
	menu.push_back("[R] Redo");
	menu.push_back("[F] Finish game");
	menu.push_back("[L] Turn on / off light");
	menu.push_back("[+] Increase light intensity");
	menu.push_back("[-] Decrease light intensity");
	return menu;
}
//------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------
void GameManager::keyboardCallbackGame(unsigned char key, int mouseX, int mouseY)
{
	switch (key)
	{
	case 13: // Confirm choice
		if (!this->m_reserved->listOfPiecesManager.confirmMove()) break;
		this->m_reserved->historyManager.setUndoRedoCalled(false);
		if (this->m_reserved->listOfPiecesManager.isGameFinished()) {
			this->m_reserved->historyManager.takeSnapshot();
			this->m_reserved->statusManager.changeState(GameStatus::END_GAME);
			this->createGraphicsList();
			break;
		}
		this->m_reserved->movementManager.changeTurn();
		this->m_reserved->statusManager.changeState(GameStatus::CHOICE);
		this->createGraphicsList();
		break;
	case 27: // Exit from game
		this->resetGame();
		this->m_reserved->statusManager.changeState(GameStatus::PRE_GAME);
		this->createGraphicsList();
		break;
	case 127:
		this->m_reserved->listOfPiecesManager.deleteChoice();
		this->m_reserved->statusManager.changeState(GameStatus::CHOICE);
		this->createGraphicsList();
		break;
	case 'l':
	case 'L':
		this->m_reserved->lightManager.pressButton(LAMP_OMNI);
		break;
	case '+':
		this->m_reserved->lightManager.increaseLight(CHESSBOARD_SPOT);
		break;
	case '-':
		this->m_reserved->lightManager.decreaseLight(CHESSBOARD_SPOT);
		break;
	}

	//PIECE MOVEMENT
	switch (key) {
	case 'w':
	case 'W':
		this->m_reserved->movementManager.moveUp(this->m_reserved->listOfPiecesManager.getChoosenPiece());
		break;
	case 'a':
	case 'A':
		this->m_reserved->movementManager.moveLeft(this->m_reserved->listOfPiecesManager.getChoosenPiece());
		break;
	case 's':
	case 'S':
		this->m_reserved->movementManager.moveDown(this->m_reserved->listOfPiecesManager.getChoosenPiece());
		break;
	case 'd':
	case 'D':
		this->m_reserved->movementManager.moveRight(this->m_reserved->listOfPiecesManager.getChoosenPiece());
		break;
	}
}

void GameManager::specialKeyCallbackGame(int key, int mouseX, int mouseY)
{
}

std::list<std::string> GameManager::menuGame() {
	std::list<std::string> menu;
	menu.push_back("[ESC] Reset game and go to pre start");
	menu.push_back("[W - A - S - D] Move piece");
	menu.push_back("[ENTER] Confirm Move");
	menu.push_back("[CANC] Return to initial position");
	menu.push_back("[L] Turn on / off light");
	menu.push_back("[+] Increase light intensity");
	menu.push_back("[-] Decrease light intensity");
	return menu;
}
//------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------
void GameManager::keyboardCallbackEndGame(unsigned char key, int mouseX, int mouseY)
{
	switch (key)
	{
	case 27: // Change camera
		this->resetGame();
		this->m_reserved->statusManager.changeState(GameStatus::PRE_GAME);
		this->createGraphicsList();
		break;
	case 'u':
	case 'U':
		this->m_reserved->historyManager.undo();
		this->createGraphicsList();
		break;
	case 'r':
	case 'R':
		this->m_reserved->historyManager.redo();
		this->createGraphicsList();
		break;
	case 'l':
	case 'L':
		this->m_reserved->lightManager.pressButton(LAMP_OMNI);
		break;
	case '+':
		this->m_reserved->lightManager.increaseLight(CHESSBOARD_SPOT);
		break;
	case '-':
		this->m_reserved->lightManager.decreaseLight(CHESSBOARD_SPOT);
		break;
	}
}

void GameManager::specialKeyCallbackEndGame(int key, int mouseX, int mouseY)
{
}

std::list<std::string> GameManager::menuEndGame() {
	std::list<std::string> menu;
	menu.push_back("[ESC] Go to pre start");
	menu.push_back("[U] Undo");
	menu.push_back("[R] Redo");
	menu.push_back("[L] Turn on / off light");
	menu.push_back("[+] Increase light intensity");
	menu.push_back("[-] Decrease light intensity");
	return menu;
}
//------------------------------------------------------------------------------------------------------------------------


void GameManager::closeCallBack() {
	this->m_reserved->isApplicationRunning = false;
}

void GameManager::resetGame() {
	this->m_reserved->rootNode = this->m_reserved->rootResetNode->clone();
	this->m_reserved->listOfPiecesManager.initialize();
}

void GameManager::createGraphicsList() {
	this->m_reserved->engine.clearScene();
	this->m_reserved->engine.passScene(this->m_reserved->rootNode);
}

void GameManager::startGame() {
	this->m_reserved->rootNode = this->m_reserved->engine.load("scenaDef.ovo");
	if (this->m_reserved->rootNode == nullptr) {
		std::cerr << "ERROR: Error during parse of the scene" << std::endl;
		return;
	}
	this->m_reserved->cameraManager.initialize();

	this->m_reserved->listOfPiecesManager.initialize();

	this->m_reserved->rootResetNode = this->m_reserved->rootNode->clone();

	this->gameLoop();
}


void GameManager::gameLoop() {
	this->m_reserved->isApplicationRunning = true;
	this->m_reserved->statusManager.changeState(GameStatus::PRE_GAME);
	this->m_reserved->engine.passScene(this->m_reserved->rootNode);

	while (this->m_reserved->isApplicationRunning) {
		this->m_reserved->engine.clear();

		this->m_reserved->engine.begin3D(this->m_reserved->cameraManager.getMainCamera(),
			this->m_reserved->cameraManager.findCameraByName(MENU_CAMERA),
			this->m_reserved->statusManager.getMenu());

		if (this->m_reserved->statusManager.getCurrentGameStatus() == GameStatus::CHOICE || this->m_reserved->statusManager.getCurrentGameStatus() == GameStatus::GAME)
			this->m_reserved->listOfPiecesManager.updateSelectPointer();

		this->m_reserved->engine.swap();
	}
}

void GameManager::initialize() {
	this->m_reserved->engine.init([]() {getInstance().closeCallBack();});

	this->m_reserved->statusManager.addGameStatusAndCallbacks(GameStatus::PRE_GAME,
		[](unsigned char key, int mouseX, int mouseY) { getInstance().keyboardCallbackPreGame(key, mouseX, mouseY); },
		[](int key, int mouseX, int mouseY) { getInstance().specialKeyCallbackPreGame(key, mouseX, mouseY); },
		this->menuPreGame());

	this->m_reserved->statusManager.addGameStatusAndCallbacks(GameStatus::CHOICE,
		[](unsigned char key, int mouseX, int mouseY) { getInstance().keyboardCallbackChoice(key, mouseX, mouseY); },
		[](int key, int mouseX, int mouseY) { getInstance().specialKeyCallbackChoice(key, mouseX, mouseY); },
		this->menuChoice());

	this->m_reserved->statusManager.addGameStatusAndCallbacks(GameStatus::GAME,
		[](unsigned char key, int mouseX, int mouseY) { getInstance().keyboardCallbackGame(key, mouseX, mouseY); },
		[](int key, int mouseX, int mouseY) { getInstance().specialKeyCallbackGame(key, mouseX, mouseY); },
		this->menuGame());

	this->m_reserved->statusManager.addGameStatusAndCallbacks(GameStatus::END_GAME,
		[](unsigned char key, int mouseX, int mouseY) { getInstance().keyboardCallbackEndGame(key, mouseX, mouseY); },
		[](int key, int mouseX, int mouseY) { getInstance().specialKeyCallbackEndGame(key, mouseX, mouseY); },
		this->menuEndGame());
}
#include "listOfPiecesManager.h"
#include "gameManager.h"
#include "mesh.h"

#define CHESSBOARD "Chessboard"
#define SELECT_POINTER "SelectPointer"

unsigned int tempRow = 0;
unsigned int tempCol = 0;
glm::mat4 tempMatrix = glm::mat4(1.0f);

ListOfPiecesManager::ListOfPiecesManager() : OnStateUpdateListener(), m_movementManager{ MovementManager::getInstance() }, m_statusManager{ StatusManager::getInstance() },
m_whiteList{ nullptr }, m_blackList{ nullptr }, m_selectPointer{ nullptr },
m_iteratorOnList{ 0 }, m_rotationAngle{ 1.2f }
{
	this->m_statusManager.subscribeListener(GameStatus::PRE_GAME, this);
	this->m_statusManager.subscribeListener(GameStatus::CHOICE, this);
	this->m_statusManager.subscribeListener(GameStatus::END_GAME, this);
}

ListOfPiecesManager::~ListOfPiecesManager() {
	this->m_statusManager.unsubscribeListener(this);
	delete this->m_blackList;
	delete this->m_whiteList;
}

ListOfPiecesManager& ListOfPiecesManager::getInstance() {
	static ListOfPiecesManager instance;
	return instance;
}

bool ListOfPiecesManager::initialize() {
	this->clearLists();

	this->buildChessboard();

	this->m_iteratorOnList = 0;

	if (!this->m_selectPointer) {
		this->m_selectPointer = const_cast<Node*>(GameManager::getRootNode()->findNodeByName(SELECT_POINTER));
		if (!this->m_selectPointer) return false;

		if (this->m_selectPointer != nullptr)
			this->m_selectPointer->setMatrix(
				glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.3f, 2.0f)) *
				glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.3f, 0.0f)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::mat4(1.0f));
	}
	return true;
}

void ListOfPiecesManager::clearLists() {
	if (this->m_blackList == nullptr || this->m_whiteList == nullptr) return;

	this->m_whiteList->clearList();
	this->m_blackList->clearList();
}

Piece* ListOfPiecesManager::getChoosenPiece() {
	return this->getCurrentList()->getPieceByIndex(this->m_iteratorOnList);
}

ListOfPieces* ListOfPiecesManager::getCurrentList() const {
	return this->m_movementManager.getTurn() ? this->m_whiteList : this->m_blackList;
}

void ListOfPiecesManager::buildChessboard() {
	std::vector<Piece*> whitePieces;
	std::vector<Piece*> blackPieces;

	Node* tempRoot = GameManager::getRootNode();

	whitePieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("WhiteRookSx")), 0, 0));
	whitePieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("WhiteKnightSx")), 0, 1));
	whitePieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("WhiteBishopSx")), 0, 2));
	whitePieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("WhiteQueen")), 0, 3));
	whitePieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("WhiteKing")), 0, 4));
	whitePieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("WhiteBishopDx")), 0, 5));
	whitePieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("WhiteKnightDx")), 0, 6));
	whitePieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("WhiteRookDx")), 0, 7));

	whitePieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("WhitePawn1")), 1, 0));
	whitePieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("WhitePawn2")), 1, 1));
	whitePieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("WhitePawn3")), 1, 2));
	whitePieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("WhitePawn4")), 1, 3));
	whitePieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("WhitePawn5")), 1, 4));
	whitePieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("WhitePawn6")), 1, 5));
	whitePieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("WhitePawn7")), 1, 6));
	whitePieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("WhitePawn8")), 1, 7));

	this->m_whiteList = new ListOfPieces(whitePieces);

	blackPieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("BlackRookSx")), 7, 7));
	blackPieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("BlackKnightSx")), 7, 6));
	blackPieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("BlackBishopSx")), 7, 5));
	blackPieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("BlackKing")), 7, 4));
	blackPieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("BlackQueen")), 7, 3));
	blackPieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("BlackBishopDx")), 7, 2));
	blackPieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("BlackKnightDx")), 7, 1));
	blackPieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("BlackRookDx")), 7, 0));

	blackPieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("BlackPawn1")), 6, 7));
	blackPieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("BlackPawn2")), 6, 6));
	blackPieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("BlackPawn3")), 6, 5));
	blackPieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("BlackPawn4")), 6, 4));
	blackPieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("BlackPawn5")), 6, 3));
	blackPieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("BlackPawn6")), 6, 2));
	blackPieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("BlackPawn7")), 6, 1));
	blackPieces.push_back(new Piece(const_cast<Node*>(tempRoot->findNodeByName("BlackPawn8")), 6, 0));
	
	this->m_blackList = new ListOfPieces(blackPieces);
}

void ListOfPiecesManager::moveChooseNodeLeft() {
	this->moveChooseNode(-1);
}

void ListOfPiecesManager::moveChooseNodeRight() {
	this->moveChooseNode(1);
}

void ListOfPiecesManager::moveChooseNode(const int& sign) {
	ListOfPieces* currentList = this->getCurrentList();
	currentList->getPieceByIndex(this->m_iteratorOnList)->getNode()->removeChild(this->m_selectPointer);

	this->m_iteratorOnList += sign * 1;
	if (this->m_iteratorOnList < 0) this->m_iteratorOnList = currentList->getSize() - 1;
	else if (this->m_iteratorOnList > currentList->getSize() - 1) this->m_iteratorOnList = 0;

	currentList->getPieceByIndex(this->m_iteratorOnList)->getNode()->addChild(this->m_selectPointer);
}

void ListOfPiecesManager::confirmChoice() {
	this->m_selectPointer->setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.3f, 0.0f)) * this->m_selectPointer->getMatrix());

	tempRow = this->getChoosenPiece()->getRow();
	tempCol = this->getChoosenPiece()->getCol();
	tempMatrix = this->getChoosenPiece()->getNode()->getMatrix();
}

void ListOfPiecesManager::deleteChoice() {
	this->m_selectPointer->setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.3f, 0.0f)) * this->m_selectPointer->getMatrix());

	this->getChoosenPiece()->setCol(tempCol);
	this->getChoosenPiece()->setRow(tempRow);
	this->getChoosenPiece()->getNode()->setMatrix(tempMatrix);

	GameManager::getRootNode()->removeChild(this->m_selectPointer);
	this->m_iteratorOnList = 0;
	this->getCurrentList()->getPieceByIndex(this->m_iteratorOnList)->getNode()->addChild(this->m_selectPointer);
}

bool ListOfPiecesManager::confirmMove() {
	if (this->getChoosenPiece()->getRow() == tempRow && this->getChoosenPiece()->getCol() == tempCol) return false;
	if (this->getCurrentList()->getNumberOfPiecesByCoordinates(this->getChoosenPiece()->getRow(), this->getChoosenPiece()->getCol()) > 1) return false;

	this->m_selectPointer->setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.3f, 0.0f)) * this->m_selectPointer->getMatrix());
	GameManager::getRootNode()->removeChild(this->m_selectPointer);

	ListOfPieces* againstListOfPieces = this->getCurrentList() == this->m_whiteList ? this->m_blackList : this->m_whiteList;
	Piece* eatenPiece = againstListOfPieces->getPieceByCoordinates(this->getChoosenPiece()->getRow(), this->getChoosenPiece()->getCol());
	if (eatenPiece != nullptr) {
		GameManager::getRootNode()->removeChild(eatenPiece->getNode());
		againstListOfPieces->removePiece(eatenPiece);
	}

	if (this->isGameFinished())
		return true;

	this->m_iteratorOnList = 0;
	againstListOfPieces->getPieceByIndex(this->m_iteratorOnList)->getNode()->addChild(this->m_selectPointer);
	return true;
}

void  ListOfPiecesManager::preGameHandler() {
	this->m_iteratorOnList = 0;
	GameManager::getRootNode()->removeChild(this->m_selectPointer);
	this->createShadow();
}

void ListOfPiecesManager::choiceHandler() {
	this->m_iteratorOnList = 0;
	GameManager::getRootNode()->removeChild(this->m_selectPointer);
	this->getCurrentList()->getPieceByIndex(this->m_iteratorOnList)->getNode()->addChild(this->m_selectPointer);
}

void ListOfPiecesManager::endGameHandler() {
	GameManager::getRootNode()->removeChild(this->m_selectPointer);
}

void ListOfPiecesManager::updateSelectPointer() {
	if (this->m_selectPointer != nullptr)
		this->m_selectPointer->setMatrix(this->m_selectPointer->getMatrix() * glm::rotate(glm::mat4(1.0f), glm::radians(this->m_rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)));
}

ListOfPieces* ListOfPiecesManager::getWhitePieces() const {
	return this->m_whiteList;
}

ListOfPieces* ListOfPiecesManager::getBlackPieces() const {
	return this->m_blackList;
}

bool ListOfPiecesManager::isGameFinished() const {
	return (this->getCurrentList() == this->m_whiteList ? this->m_blackList : this->m_whiteList)->getSize() == 0;
}

void ListOfPiecesManager::updateChessboard(ListOfPieces* whiteList, ListOfPieces* blackList) {
	this->m_whiteList = whiteList->clone();
	this->m_blackList = blackList->clone();

	Node* chessboardNode = const_cast<Node*>(GameManager::getRootNode()->findNodeByName(CHESSBOARD));
	for (auto* child : chessboardNode->getChildren())
		chessboardNode->removeChild(child);

	for (int i = 0; i < this->m_whiteList->getSize(); i++)
		chessboardNode->addChild(this->m_whiteList->getPieceByIndex(i)->getNode());

	for (int i = 0; i < this->m_blackList->getSize(); i++)
		chessboardNode->addChild(this->m_blackList->getPieceByIndex(i)->getNode());
}

void ListOfPiecesManager::createShadow() {
	Node* chessboardNode = const_cast<Node*>(GameManager::getRootNode()->findNodeByName(CHESSBOARD));

	for (Node* child : chessboardNode->getChildren()) {
		Mesh* meshChild = dynamic_cast<Mesh*>(child);
		float minY = std::numeric_limits<float>::max();
		float maxY = std::numeric_limits<float>::lowest();

		for (const glm::vec3& vertex : meshChild->getVertices()) {
			minY = std::min(minY, vertex.y);
			maxY = std::max(maxY, vertex.y);
		}

		float height = maxY - minY;
		float offset = height / 2.0f;

		Node* childCloned = meshChild->clone();
		childCloned->setEnableLighting(false);
		childCloned->setName(childCloned->getName() + "_shadow");
		childCloned->setMatrix(
			glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, -offset + 0.01f, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 1.0f))
		);
		auto* material = childCloned->getMaterial();
		material->setTexture(nullptr);
		material->setAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
		material->setDiffuse(glm::vec3(0.2f, 0.2f, 0.2f));
		material->setSpecular(glm::vec3(0.2f, 0.2f, 0.2f));
		child->addChild(childCloned);
	}
}
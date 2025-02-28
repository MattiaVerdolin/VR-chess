#include "historyManager.h"
#include "gameManager.h"

struct HistoryManager::Reserved {
	ListOfPieces* listOfPiecesWhite;
	ListOfPieces* listOfPiecesBlack;

	Reserved(ListOfPieces* piecesWhite, ListOfPieces* piecesBlack) :
		listOfPiecesWhite{ piecesWhite },
		listOfPiecesBlack{ piecesBlack }
		{}

	~Reserved() {
		deleteClonedNode(listOfPiecesBlack);
		deleteClonedNode(listOfPiecesWhite);
		delete listOfPiecesBlack;
		delete listOfPiecesWhite;
	}

private:
	void deleteClonedNode(ListOfPieces* listOfPieces) {
		for (int i = 0; i < listOfPieces->getSize(); i++)
			delete listOfPieces->getPieceByIndex(i)->getNode();
	}
};

HistoryManager::HistoryManager() : m_history(std::vector<HistoryManager::Reserved*>()),
		m_listOfPiecesManager{ListOfPiecesManager::getInstance()},
		m_movementManager{MovementManager::getInstance()},
		m_statusManager{StatusManager::getInstance()},
		m_pointer{ -1 }, m_undoRedoCalled{ false } 
{
	this->m_statusManager.subscribeListener(GameStatus::PRE_GAME, this);
	this->m_statusManager.subscribeListener(GameStatus::CHOICE, this);
	this->m_statusManager.subscribeListener(GameStatus::END_GAME, this);
}

HistoryManager::~HistoryManager() {
	this->m_statusManager.unsubscribeListener(this);
	for (auto* reserved : this->m_history)
		delete reserved;
}

HistoryManager& HistoryManager::getInstance() {
	static HistoryManager instance;
	return instance;
}

void HistoryManager::choiceHandler() {
	if(!this->isUndoRedoCalled())
		this->takeSnapshot();
}

void HistoryManager::preGameHandler() {
	for (auto* reserved : this->m_history)
		delete reserved;
	this->m_history.clear();
	this->m_pointer = -1;
	this->m_undoRedoCalled = false;
}

void HistoryManager::endGameHandler() {
	this->m_pointer = 1;
	this->undo();
}

void HistoryManager::takeSnapshot() {
	Reserved* reserved = new Reserved(this->m_listOfPiecesManager.getWhitePieces()->clone(),
		this->m_listOfPiecesManager.getBlackPieces()->clone());
	this->m_history.push_back(reserved);
	this->m_pointer++;
}

bool HistoryManager::undo() {
	if (this->m_pointer == 0)
		return false;
	this->m_pointer--;
	this->m_movementManager.changeTurn();
	this->m_listOfPiecesManager.updateChessboard(this->m_history[this->m_pointer]->listOfPiecesWhite, this->m_history[this->m_pointer]->listOfPiecesBlack);
	GameManager::getRootNode()->removeChild(const_cast<Node*>(GameManager::getRootNode()->findNodeByName("SelectPointer")));
	this->m_undoRedoCalled = true;
	return true;
}

bool HistoryManager::redo() {
	if (this->m_pointer == (int)this->m_history.size() - 1)
		return false;
	this->m_pointer++;
	this->m_movementManager.changeTurn();
	this->m_listOfPiecesManager.updateChessboard(this->m_history[this->m_pointer]->listOfPiecesWhite, this->m_history[this->m_pointer]->listOfPiecesBlack);
	GameManager::getRootNode()->removeChild(const_cast<Node*>(GameManager::getRootNode()->findNodeByName("SelectPointer")));
	return true;
}

bool HistoryManager::isUndoRedoCalled() {
	return this->m_undoRedoCalled;
}
void HistoryManager::setUndoRedoCalled(const bool& val) {
	this->m_undoRedoCalled = val;
}
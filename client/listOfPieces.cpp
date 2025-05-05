#include "listOfPieces.h"

ListOfPieces::ListOfPieces(const std::vector<Piece*>& pieces) : m_listOfPieces{pieces} {}

ListOfPieces::ListOfPieces(const ListOfPieces& other) : m_listOfPieces{ std::vector<Piece*>() } {
	for (auto* piece : other.m_listOfPieces)
		this->m_listOfPieces.push_back(new Piece(*piece));
}

ListOfPieces::~ListOfPieces() {
	for (auto* piece : this->m_listOfPieces)
		delete piece;
	this->m_listOfPieces.clear();
}

ListOfPieces* ListOfPieces::clone() {
	return new ListOfPieces(*this);
}

bool ListOfPieces::removePiece(Piece* piece) {
	if (piece == nullptr) return false;

	auto it = std::remove(this->m_listOfPieces.begin(), this->m_listOfPieces.end(), piece);
	if (it != this->m_listOfPieces.end()) {
		this->m_listOfPieces.erase(it, this->m_listOfPieces.end());
		return true;
	}
	return false;
}

void ListOfPieces::clearList() {
	this->m_listOfPieces.clear();
}

int ListOfPieces::getSize() const {
	return (int)this->m_listOfPieces.size();
}

Piece* ListOfPieces::getPieceByIndex(const unsigned int& index) const {
	if (index < 0 || index >= this->m_listOfPieces.size()) return nullptr;

	return this->m_listOfPieces[index];
}

Piece* ListOfPieces::getPieceByCoordinates(const unsigned int& row, const unsigned int& col) const {
	for (auto* piece : this->m_listOfPieces)
		if (piece->getRow() == row && piece->getCol() == col)
			return piece;
	return nullptr;
}

unsigned int ListOfPieces::getNumberOfPiecesByCoordinates(const unsigned int& row, const unsigned int& col) const {
	unsigned int numberOfPiecesInSameCoordinates = 0;
	for (auto* piece : this->m_listOfPieces)
		if (piece->getRow() == row && piece->getCol() == col)
			numberOfPiecesInSameCoordinates++;
	return numberOfPiecesInSameCoordinates;
}

std::vector<Piece*> ListOfPieces::getAllPieces() const {
	return m_listOfPieces;
}


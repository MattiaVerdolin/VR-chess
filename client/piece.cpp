#include "piece.h"

Piece::Piece(Node* node, const unsigned int& row, const unsigned int& col) : m_node{ node }, m_row{ row }, m_col{ col } {}

Piece::Piece(const Piece& other) : m_node{ other.m_node->clone() }, m_row{ other.m_row }, m_col{ other.m_col } {}
 
Node* Piece::getNode() const {
	return this->m_node;
}
const unsigned int& Piece::getRow() const {
	return this->m_row;
}
void Piece::setRow(const unsigned int& row) {
	this->m_row = row;
}
const unsigned int& Piece::getCol() const {
	return this->m_col;
}
void Piece::setCol(const unsigned int& col) {
	this->m_col = col;
}
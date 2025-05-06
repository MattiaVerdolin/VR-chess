/**
 * @file    piece.h
 * @brief   Declaration of the Piece class, representing a game piece in a chessboard scene.
 *
 * This file defines the Piece class, which encapsulates a game piece's position (row and column)
 * and its associated node in the 3D scene. The class provides methods for manipulating the piece's
 * position and accessing the associated node.
 *
 * @author	Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 * @author	Riccardo Cristallo (C) SUPSI [riccardo.cristallo@student.supsi.ch]
 * @author	Mattia Verdolin (C) SUPSI [mattia.verdolin@student.supsi.ch]
 */

#pragma once

//////////////
// #INCLUDE //
//////////////

// Project-specific headers
#include "node.h"

/**
 * @class Piece
 * @brief Class representing a game piece with its position and associated node in the scene.
 *
 * The Piece class encapsulates the position of a game piece (row and column) and maintains a reference
 * to the associated Node that represents the piece's visual representation in the 3D scene. It allows
 * modification of the position and provides access to the Node object.
 */
class Piece
{
public:
    /**
     * @brief Constructor that initializes a Piece with a specific node, row, and column.
     *
     * This constructor initializes the Piece object with the specified node, row, and column values.
     * The node represents the 3D object associated with the piece in the scene.
     *
     * @param node A pointer to the Node object representing the visual aspect of the piece.
     * @param row The row position of the piece on the chessboard.
     * @param col The column position of the piece on the chessboard.
     */
	Piece(Node* node, const unsigned int& row, const unsigned int& col);

    /**
     * @brief Copy constructor for the Piece class.
     * 
     * This constructor creates a new Piece object by copying an existing Piece. The node is cloned to 
     * ensure the new piece has a separate instance of the associated node.
     *
     * @param other The Piece object to copy.
     */
	Piece(const Piece& other);

    /**
     * @brief Destructor for the Piece class.
     */
	~Piece() = default;

    /**
     * @brief Retrieve the associated Node object of the piece.
     * 
     * This method returns a pointer to the Node object associated with the piece. The node represents
     * the 3D object in the scene.
     *
     * @return A pointer to the Node object associated with the piece.
     */
	Node* getNode() const;

    /**
     * @brief Retrieve the row position of the piece.
     * 
     * This method returns the row position of the piece on the chessboard.
     *
     * @return The row position of the piece.
     */
	const unsigned int& getRow() const;

    /**
     * @brief Set the row position of the piece.
     * 
     * This method sets the row position of the piece to the specified value.
     *
     * @param row The new row position for the piece.
     */
	void setRow(const unsigned int& row);

    /**
     * @brief Retrieve the column position of the piece.
     * 
     * This method returns the column position of the piece on the chessboard.
     *
     * @return The column position of the piece.
     */
	const unsigned int& getCol() const;

    /**
     * @brief Set the column position of the piece.
     * 
     * This method sets the column position of the piece to the specified value.
     *
     * @param col The new column position for the piece.
     */
	void setCol(const unsigned int& col);

private:
    /// Pointer to the Node object representing the visual representation of the piece.
    Node* m_node;

    /// The row position of the piece on the chessboard.
    unsigned int m_row;

    /// The column position of the piece on the chessboard.
    unsigned int m_col;
};

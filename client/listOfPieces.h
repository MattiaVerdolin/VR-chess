/**
 * @file    listOfPieces.h
 * @brief   Declaration of the ListOfPieces class, managing a collection of Piece objects.
 *
 * This file defines the ListOfPieces class, which manages a list of Piece objects in a chessboard scene.
 * The class provides methods for adding, removing, cloning, and querying pieces based on various criteria,
 * such as coordinates or index.
 *
 * @authors Luca Fantò (C) SUPSI [luca.fanto@student.supsi.ch]
 *          Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 *          Antonio Marroffino (C) SUPSI [antonio.marroffino@student.supsi.ch]
 */

#pragma once

//////////////
// #INCLUDE //
//////////////

// Standard libraries
#include <vector>
#include <algorithm>

// Project-specific headers
#include "piece.h"

/**
 * @class ListOfPieces
 * @brief Class representing a collection of Piece objects.
 *
 * The ListOfPieces class encapsulates a collection of pieces on the chessboard.
 * It provides various methods for interacting with the list, including removing pieces,
 * cloning the list, and retrieving pieces by their index or coordinates.
 */
class ListOfPieces
{
public:
    /**
     * @brief Constructor initializing the list with a vector of Piece pointers.
     * 
     * Initializes the ListOfPieces object with an existing vector of Piece pointers.
     * 
     * @param pieces A vector of pointers to Piece objects.
     */
	ListOfPieces(const std::vector<Piece*>& pieces);

    /**
     * @brief Destructor that cleans up resources.
     *
     * This destructor is responsible for releasing any allocated memory or resources
     * associated with the ListOfPieces object.
     */
    ~ListOfPieces();

    /**
     * @brief Clone the current ListOfPieces object.
     * 
     * Creates a deep copy of the current ListOfPieces object, including the pieces it contains.
     * 
     * @return A new instance of ListOfPieces that is a copy of the current object.
     */
	ListOfPieces* clone();

    /**
     * @brief Remove a specific piece from the list.
     * 
     * This method removes the specified piece from the list of pieces.
     * 
     * @param piece A pointer to the Piece to remove.
     * @return True if the piece was successfully removed, otherwise false.
     */
	bool removePiece(Piece* piece);

    /**
     * @brief Clear all pieces from the list.
     * 
     * This method clears the list, removing all pieces from it.
     */
	void clearList();

    /**
     * @brief Get the number of pieces in the list.
     * 
     * This method returns the number of pieces currently in the list.
     * 
     * @return The number of pieces in the list.
     */
	int getSize() const;

    /**
     * @brief Retrieve a piece by its index in the list.
     * 
     * This method retrieves a piece based on its index in the list. If the index is invalid,
     * it returns a nullptr.
     * 
     * @param index The index of the piece to retrieve.
     * @return A pointer to the Piece at the specified index, or nullptr if the index is out of range.
     */
	Piece* getPieceByIndex(const unsigned int& index) const;

    /**
     * @brief Retrieve a piece by its row and column coordinates.
     * 
     * This method retrieves a piece based on its row and column coordinates. If no piece is found
     * at the given coordinates, it returns a nullptr.
     * 
     * @param row The row coordinate of the piece to retrieve.
     * @param col The column coordinate of the piece to retrieve.
     * @return A pointer to the Piece located at the specified coordinates, or nullptr if no such piece exists.
     */
	Piece* getPieceByCoordinates(const unsigned int& row, const unsigned int& col) const;

    /**
     * @brief Get the number of pieces located at specific coordinates.
     * 
     * This method counts how many pieces are located at the specified row and column coordinates.
     * 
     * @param row The row coordinate to search for.
     * @param col The column coordinate to search for.
     * @return The number of pieces found at the specified coordinates.
     */
	unsigned int getNumberOfPiecesByCoordinates(const unsigned int& row, const unsigned int& col) const;

private:
    /**
     * @brief Copy constructor for the ListOfPieces class.
     * 
     * This constructor creates a new ListOfPieces object by copying an existing one. It ensures that
     * the pieces are deep-copied so that the new list has its own copies of the pieces.
     * 
     * @param other The ListOfPieces object to copy.
     */
	ListOfPieces(const ListOfPieces& other);

    /// Vector holding the list of pointers to Piece objects.
    std::vector<Piece*> m_listOfPieces;
};

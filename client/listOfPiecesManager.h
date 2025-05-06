/**
 * @file ListOfPiecesManager.h
 * @brief Manages the list of chess pieces for a chess game, including initialization,
 *        selection, and movement. Provides functionality for moving pieces, selecting,
 *        confirming choices, and updating the game state.
 * 
 * @author	Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 * @author	Riccardo Cristallo (C) SUPSI [riccardo.cristallo@student.supsi.ch]
 * @author	Mattia Verdolin (C) SUPSI [mattia.verdolin@student.supsi.ch]
 */

#pragma once

//////////////
// #INCLUDE //
//////////////

// Standard libraries
#include <map>

// Project-specific headers
#include "listOfPieces.h"
#include "movementManager.h"
#include "onStateUpdate.h"
#include "statusManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

 /**
  * @class ListOfPiecesManager
  * @brief Manages the list of chess pieces for a chess game, including initialization,
  *        selection, and movement.
  */
class ListOfPiecesManager : public OnStateUpdateListener {
public:
    /**
     * @brief Deleted copy constructor to enforce singleton pattern.
     */
    ListOfPiecesManager(const ListOfPiecesManager&) = delete;

    /**
     * @brief Deleted assignment operator to enforce singleton pattern.
     */
    ListOfPiecesManager& operator=(const ListOfPiecesManager&) = delete;

    /**
     * @brief Destructor for ListOfPiecesManager.
     *
     * Cleans up resources by unsubscribing the listener from the status manager and deleting
     * the lists of white and black pieces.
     */
    ~ListOfPiecesManager();

    /**
     * @brief Returns the singleton instance of ListOfPiecesManager.
     * @return Reference to the singleton instance.
     */
    static ListOfPiecesManager& getInstance();

    /**
     * @brief Initializes the chessboard and pointer for piece selection.
     * @return True if initialization is successful, false otherwise.
     */
    bool initialize();

    /**
     * @brief Retrieves the currently selected chess piece.
     * @return Pointer to the currently selected chess piece.
     */
    Piece* getChoosenPiece();

    /**
     * @brief Moves the selection pointer to the left in the list of pieces.
     */
    void moveChooseNodeLeft();

    /**
     * @brief Moves the selection pointer to the right in the list of pieces.
     */
    void moveChooseNodeRight();

    /**
     * @brief Confirms the selection of the currently highlighted piece.
     */
    void confirmChoice();

    /**
     * @brief Deletes the current selection and resets the selection pointer.
     */
    void deleteChoice();

    /**
     * @brief Confirms the movement of the selected chess piece.
     * @return True if the move is valid, false otherwise.
     */
    bool confirmMove();

    /**
     * @brief Updates the selection pointer's transformation matrix.
     */
    void updateSelectPointer();

    /**
     * @brief Gets the list of white chess pieces.
     * @return Pointer to the list of white pieces.
     */
    ListOfPieces* getWhitePieces() const;

    /**
     * @brief Gets the list of black chess pieces.
     * @return Pointer to the list of black pieces.
     */
    ListOfPieces* getBlackPieces() const;

    /**
     * @brief Updates the chessboard with new lists of white and black pieces.
     * @param whiteList List of white pieces.
     * @param blackList List of black pieces.
     */
    void updateChessboard(ListOfPieces* whiteList, ListOfPieces* blackList);

    /**
     * @brief Checks if the game is finished.
     *
     * This method checks whether the game has ended.
     *
     * @return True if the game is finished, false otherwise, based on turn.
     */
    bool isGameFinished() const;

    /**
     * @brief Updates the state of chess pieces based on Leap Motion controller input.
     * 
     * This method handles the interaction between the Leap Motion controller and chess pieces.
     * It manages piece grabbing, movement, and snapping to the chessboard grid.
     * 
     * The method uses a radius-based detection system to determine when a piece can be grabbed,
     * and implements a snapping mechanism to ensure pieces are placed on valid board positions.
     */
    void updateLeapMotion();

private:
    /**
     * @brief Constructs a ListOfPiecesManager.
     */
    ListOfPiecesManager();

    /**
     * @brief Gets the current list of pieces based on the current turn.
     * @return Pointer to the current list of pieces.
     */
    ListOfPieces* getCurrentList() const;

    /**
     * @brief Moves the selection pointer within the current list.
     * @param sign Determines the direction (-1 for left, 1 for right).
     */
    void moveChooseNode(const int& sign);

    /**
     * @brief Handles events during the pre-game state.
     */
    void preGameHandler() override;

    /**
     * @brief Handles events during the piece choice state.
     */
    void choiceHandler() override;

    /**
     * @brief Handles events during the end-game state.
     */
    void endGameHandler() override;

    /**
     * @brief Builds the initial chessboard with all pieces.
     */
    void buildChessboard();

    /**
     * @brief Clears the lists of pieces for both players.
     */
    void clearLists();

    /**
     * @brief Creates shadow effects for the chess pieces.
     */
    void createShadow();

    /**
     * @brief Reference to the movement manager.
     */
    MovementManager& m_movementManager;

    /**
     * @brief Reference to the status manager.
     */
    StatusManager& m_statusManager;

    /**
     * @brief Pointer to the list of white pieces.
     */
    ListOfPieces* m_whiteList;

    /**
     * @brief Pointer to the list of black pieces.
     */
    ListOfPieces* m_blackList;

    /**
     * @brief Pointer to the selection node.
     */
    Node* m_selectPointer;

    /**
     * @brief Index of the currently selected piece.
     */
    int m_iteratorOnList;

    /**
     * @brief Rotation angle for the selection pointer animation.
     */
    float m_rotationAngle;


    // Stato per la presa

    glm::vec3 snapToClosestCell(const glm::vec3& pos);


    bool isHoldingPiece = false;
    Piece* grabbedPiece = nullptr;

};

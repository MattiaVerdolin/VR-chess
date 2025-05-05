/**
 * @file    movementManager.h
 * @brief   Declaration and implementation of the MovementManager class, handling movement logic for game pieces.
 *
 * This file defines the MovementManager class, responsible for managing the movement of pieces on the game board.
 * The class includes functionality for moving pieces in all four directions (left, right, up, down), 
 * as well as managing turns between two players (white and black).
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
#include <array>

// Project-specific headers
#include "onStateUpdate.h"
#include "statusManager.h"
#include "piece.h"
#include "node.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

/**
 * @class MovementManager
 * @brief Class for managing the movement of pieces on the board.
 * 
 * The MovementManager class handles the logic for moving pieces in four directions (left, right, up, down)
 * and managing the turn-based system. It updates the game state and transforms the position of pieces
 * based on player turns.
 */
class MovementManager : public OnStateUpdateListener
{
public:
    /**
     * @brief Deleted copy constructor to prevent copying of the MovementManager instance.
     */
	MovementManager(const MovementManager&) = delete;

    /**
     * @brief Deleted assignment operator to prevent assignment of the MovementManager instance.
     */
	MovementManager& operator=(const MovementManager&) = delete;

    /**
     * @brief Destructor to clean up resources and unsubscribe from the status manager.
     *
     * This destructor is responsible for unsubscribing the MovementManager from the
     * status manager's listeners. This ensures that no further updates are received
     * once the object is destroyed, preventing any dangling pointers or memory leaks.
     */
	~MovementManager();

    /**
     * @brief Static method to get the singleton instance of the MovementManager.
     * 
     * @return The singleton instance of the MovementManager class.
     */
	static MovementManager& getInstance();

    /**
     * @brief Get the current player's turn.
     * 
     * @return True if it's the white player's turn, false if it's the black player's turn.
     */
	bool getTurn() const;

    /**
     * @brief Get the movement factor based on the current player's turn.
     * 
     * @return 1.0f for white player's turn and -1.0f for black player's turn.
     */
	float getFactoryByTurn() const;

    /**
     * @brief Change the current turn from white to black or black to white.
     */
	void changeTurn();

    /**
     * @brief Move the specified piece to the right.
     * 
     * This method moves the piece one step to the right, adjusting its column.
     * It also checks that the piece does not move out of bounds.
     * 
     * @param piece The piece to move.
     */
	void moveRight(Piece* piece);

    /**
     * @brief Move the specified piece to the left.
     * 
     * This method moves the piece one step to the left, adjusting its column.
     * It also checks that the piece does not move out of bounds.
     * 
     * @param piece The piece to move.
     */
	void moveLeft(Piece* piece);

    /**
     * @brief Move the specified piece upwards.
     * 
     * This method moves the piece one step upwards, adjusting its row.
     * It also checks that the piece does not move out of bounds.
     * 
     * @param piece The piece to move.
     */
	void moveUp(Piece* piece);

    /**
     * @brief Move the specified piece downwards.
     * 
     * This method moves the piece one step downwards, adjusting its row.
     * It also checks that the piece does not move out of bounds.
     * 
     * @param piece The piece to move.
     */
	void moveDown(Piece* piece);

private:
    /**
     * @brief Private constructor to initialize the MovementManager.
     * 
     * Initializes the MovementManager and subscribes to the PRE_GAME state for handling game setup.
     */
	MovementManager();

    /**
     * @brief Method to handle game status updates before the game starts.
     * 
     * Resets the turn to white before the game begins.
     */
	void preGameHandler() override;

    /// Flag indicating the current player's turn (TRUE for white, FALSE for black).
	bool m_turn;

    /// Reference to the StatusManager instance for managing game status.
	StatusManager& m_statusManager;

    /// Constant representing the space the pieces move in one step.
	const float MOVEMENT_SPACE = 0.075f;
};

/**
 * @file    gameStatus.h
 * @brief   Declaration of the GameStatus enum class for representing game states.
 *
 * This file defines the GameStatus enumeration, which represents the various
 * states of the game lifecycle in the 3D chessboard rendering application.
 *
 * @author	Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 * @author	Riccardo Cristallo (C) SUPSI [riccardo.cristallo@student.supsi.ch]
 * @author	Mattia Verdolin (C) SUPSI [mattia.verdolin@student.supsi.ch]
 */

#pragma once

/**
 * @enum    GameStatus
 * @brief   Enumeration representing the various states of the game.
 *
 * The GameStatus enum class defines four states for managing the game lifecycle:
 * - PRE_GAME: The game has not yet started.
 * - CHOICE: The phase where players make choices, selecting pieces.
 * - GAME:  The phase where players move the selected piece.
 * - END_GAME: The game has concluded.
 */
enum class GameStatus {
    PRE_GAME,
    CHOICE,
    GAME,
    END_GAME
};

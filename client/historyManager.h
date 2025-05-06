/**
 * @file    historyManager.h
 * @brief   Declaration of the HistoryManager class, responsible for managing history of the game.
 *
 * This file defines the HistoryManager class, which handles moves tracking od pieces during the game.
 * It allows undo and redo to previous or following (if available) moves.
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
#include <vector>

// Project-specific includes
#include "listOfPiecesManager.h"
#include "movementManager.h"
#include "onStateUpdate.h"
#include "statusManager.h"
#include "cameraManager.h"

/**
 * @class HistoryManager
 * @brief Manages the history of game state changes, supporting undo and redo operations.
 *
 * The HistoryManager class tracks snapshots of the game state, enabling the functionality to undo and redo movements
 * during the game. It is responsible for storing the game state in a history vector and providing mechanisms to roll back
 * or move forward through the history. It listens to game state changes and updates its internal state accordingly.
 */
class HistoryManager : public OnStateUpdateListener {
public:
    /**
     * @brief Delete the copy constructor for HistoryManager.
     */
    HistoryManager(const HistoryManager&) = delete;

    /**
     * @brief Delete the copy assignment operator for HistoryManager.
     */
    HistoryManager& operator=(const HistoryManager&) = delete;

    /**
     * @brief Destructor for HistoryManager.
     *
     * The destructor is responsible for cleaning up resources when the HistoryManager instance is destroyed.
     * It performs the following actions:
     * - Unsubscribes the HistoryManager as a listener from the StatusManager, ensuring that it no longer receives
     *   updates or notifications from the game state changes.
     * - Iterates over the `m_history` vector, which contains pointers to stored snapshots of the game state,
     *   and deletes each snapshot to free up memory.
     *
     * These steps ensure that the HistoryManager cleans up both its own internal state and any dynamic memory
     * associated with the game state snapshots, preventing memory leaks.
     */
    ~HistoryManager();

    /**
     * @brief Get the singleton instance of HistoryManager.
     * @return The singleton HistoryManager instance.
     */
    static HistoryManager& getInstance();

    /**
     * @brief Take a snapshot of the current game state.
     */
    void takeSnapshot();

    /**
     * @brief Undo the last action in the game.
     * @return True if the undo operation was successful, false otherwise.
     */
    bool undo();

    /**
     * @brief Redo the last undone action.
     * @return True if the redo operation was successful, false otherwise.
     */
    bool redo();

    /**
     * @brief Set whether the undo/redo operation was called.
     * @param val The new value to set for the undo/redo flag.
     */
    void setUndoRedoCalled(const bool& val);

private:
    /**
     * @brief Private constructor for HistoryManager.
     */
    HistoryManager();

    /**
     * @brief Check if undo or redo has been called.
     * @return True if undo/redo was called, false otherwise.
     */
    bool isUndoRedoCalled();

    /**
     * @brief Handler for the pre-game state update.
     *        Clears the history and resets the pointer.
     */
    void preGameHandler() override;

    /**
     * @brief Handler for the choice state update.
     *        Takes a snapshot if undo/redo hasn't been called.
     */
    void choiceHandler() override;

    /**
     * @brief Handler for the end game state update.
     *        Resets the pointer and attempts to undo the last action.
     */
    void endGameHandler() override;

    /**
     * @brief Represents a snapshot of the game state at a specific moment.
     */
    struct Reserved;

    /**
     * @brief Stores the history of game state snapshots.
     */
    std::vector<Reserved*> m_history;

    /**
     * @brief The manager responsible for handling the list of pieces in the game.
     */
    ListOfPiecesManager& m_listOfPiecesManager;

    /**
     * @brief The manager responsible for handling the movement logic in the game.
     */
    MovementManager& m_movementManager;

    /**
     * @brief The manager responsible for handling the status of the game.
     */
    StatusManager& m_statusManager;

    /**
     * @brief The current position in the history vector.
     */
    int m_pointer;

    /**
     * @brief Flag indicating whether undo/redo has been called.
     */
    bool m_undoRedoCalled;
};

/**
 * @file    statusManager.h
 * @brief   Declaration of the StatusManager class for managing game states and callbacks.
 *
 * The StatusManager class is a singleton responsible for managing game states, their associated
 * callbacks, and state-specific menus. It also allows listeners to subscribe to state updates
 * and triggers appropriate callbacks during state transitions.
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
#include <memory>
#include <list>
#include <map>
#include <string>

// Project-specific includes
#include "gameStatus.h"
#include "onStateUpdate.h"
#include "engine.h"

 /**
  * @class StatusManager
  * @brief Singleton class for managing game states and their associated behaviors.
  *
  * The StatusManager class maintains a mapping of game states to their callbacks
  * and menus. It also allows for the registration of listeners that are notified
  * when the game state changes.
  */
class StatusManager {
public:
    /**
     * @brief Deleted copy constructor to enforce singleton pattern.
     */
    StatusManager(const StatusManager&) = delete;

    /**
     * @brief Deleted copy assignment operator to enforce singleton pattern.
     */
    StatusManager& operator=(const StatusManager&) = delete;

    /**
     * @brief Destructor for the StatusManager class.
     */
    ~StatusManager();

    /**
     * @brief Retrieve the singleton instance of StatusManager.
     * @return Reference to the singleton instance.
     */
    static StatusManager& getInstance();

    /**
     * @brief Subscribe a listener to updates for a specific game state.
     * @param gameState The game state to which the listener subscribes.
     * @param listener Pointer to the listener to notify on state updates.
     */
    void subscribeListener(GameStatus gameState, OnStateUpdateListener* listener);

    /**
     * @brief Unsubscribe a listener from receiving updates for all game states.
     * @param listener Pointer to the listener to unsubscribe. If the pointer
     *                 is null, the method performs no operation.
     */
    void unsubscribeListener(OnStateUpdateListener* listener);

    /**
     * @brief Add a new game state with its associated callbacks and menu.
     * @param gameStatus The game state to add.
     * @param keyboardCallback Optional keyboard callback for the state.
     * @param specialKeyCallback Optional special key callback for the state.
     * @param menu Optional menu associated with the state.
     */
    void addGameStatusAndCallbacks(
        const GameStatus& gameStatus,
        void (*keyboardCallback)(unsigned char, int, int) = nullptr,
        void (*specialKeyCallback)(int, int, int) = nullptr,
        const std::list<std::string> menu = std::list<std::string>()
    );

    /**
     * @brief Change the current game state and trigger associated behaviors.
     * @param gameStatus The new game state to set.
     */
    void changeState(const GameStatus& gameStatus);

    /**
     * @brief Retrieve the menu associated with the current game state.
     * @return List of menu items for the current state.
     */
    const std::list<std::string> getMenu() const;

    /**
     * @brief Retrieve the current game state.
     * @return The current game state.
     */
    GameStatus getCurrentGameStatus() const;

private:
    /**
     * @brief Private constructor to enforce singleton pattern.
     */
    StatusManager();

    /**
     * @brief Retrieve the keyboard callback associated with a game state.
     * @param gameStatus The game state to query.
     * @return Pointer to the keyboard callback function.
     */
    void (*getKeyboardCallback(const GameStatus& gameStatus))(unsigned char, int, int);

    /**
     * @brief Retrieve the special key callback associated with a game state.
     * @param gameStatus The game state to query.
     * @return Pointer to the special key callback function.
     */
    void (*getSpecialKeyCallback(const GameStatus& gameStatus))(int, int, int);

    /**
     * @brief Internal structure for managing reserved resources.
     *
     * This structure holds keyboardCallback, specialKeyCallback and menu voices.
     */
    struct Reserved;

    /**
     * @brief Opaque data structure for managing game state data.
     */
    std::map<GameStatus, Reserved> m_reserved;

    /**
     * @brief Current game state.
     */
    GameStatus m_currentState;

    /**
     * @brief Mapping of game states to their respective listeners.
     */
    std::map<GameStatus, std::list<OnStateUpdateListener*>> m_listener;

    /**
     * @brief Reference to the rendering engine instance.
     */
    Eng::Base& m_engine;
};

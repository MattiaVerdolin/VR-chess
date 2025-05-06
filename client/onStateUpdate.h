/**
 * @file    onStateUpdate.h
 * @brief   Declaration of the OnStateUpdateListener class for handling game state updates.
 *
 * This file defines the OnStateUpdateListener class, which provides an interface
 * and mechanism to handle updates when the game state changes. It uses a map to associate
 * game states with their corresponding handler functions, allowing modular and extensible
 * state management.
 *
 * @author	Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 * @author	Riccardo Cristallo (C) SUPSI [riccardo.cristallo@student.supsi.ch]
 * @author	Mattia Verdolin (C) SUPSI [mattia.verdolin@student.supsi.ch]
 */

#pragma once

//////////////
// #INCLUDE //
//////////////

// Project-specific includes
#include "gameStatus.h"

// Standard libraries
#include <map>
#include <functional>

 /**
  * @class OnStateUpdateListener
  * @brief Class for handling updates when the game state changes.
  *
  * The OnStateUpdateListener class maps game states to their corresponding
  * handler functions. Derived classes can override the virtual handler methods
  * to provide specific behavior for each game state.
  */
class OnStateUpdateListener {
public:
    /**
     * @brief Constructor for the OnStateUpdateListener class.
     *
     * Initializes the mapping of game states to their respective handler functions.
     */
    OnStateUpdateListener();

    /**
     * @brief Trigger the appropriate handler for a given game state.
     *
     * Searches for the corresponding handler function in the map and invokes it
     * if it exists.
     *
     * @param gameState The current state of the game.
     */
    void onStateChangeUpdate(GameStatus gameState);

    /**
     * @brief Handler for the PRE_GAME state.
     *
     * This method is intended to be overridden by derived classes to define
     * specific behavior for the PRE_GAME state.
     */
    virtual void preGameHandler();

    /**
     * @brief Handler for the CHOICE state.
     *
     * This method is intended to be overridden by derived classes to define
     * specific behavior for the CHOICE state.
     */
    virtual void choiceHandler();

    /**
     * @brief Handler for the GAME state.
     *
     * This method is intended to be overridden by derived classes to define
     * specific behavior for the GAME state.
     */
    virtual void gameHandler();

    /**
     * @brief Handler for the END_GAME state.
     *
     * This method is intended to be overridden by derived classes to define
     * specific behavior for the END_GAME state.
     */
    virtual void endGameHandler();

protected:
    /**
     * @brief Map of game states to their corresponding handler functions.
     *
     * This map associates each GameStatus with a std::function, allowing
     * dynamic invocation of the appropriate handler function.
     */
    std::map<GameStatus, std::function<void()>> m_mapFunctionOnState;
};

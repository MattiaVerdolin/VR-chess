/**
 * @file gameManager.h
 *
 * @brief GameManager is responsible for managing the state and progression of the game, including handling user inputs,
 *        transitioning between game states, and controlling the camera, lights, pieces, and movements in the game world.
 *        It follows the Singleton design pattern to ensure that there is only one instance of GameManager during the
 *        game lifecycle.
 *
 * This header defines the GameManager class and its methods for handling game initialization, game loop, and various
 * callbacks for keyboard inputs during different game states (e.g., pre-game, choice, game, and end-game). It also manages
 * the game state transitions and calls appropriate methods for handling game actions.
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
#include <array>

// Project-specific headers
#include "statusManager.h"
#include "cameraManager.h"
#include "movementManager.h"
#include "listOfPiecesManager.h"
#include "historyManager.h"
#include "lightManager.h"
#include "engine.h"
#include "node.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

 /**
  * @class GameManager
  * @brief Singleton class responsible for managing the game state, input handling, and transitioning between different game modes.
  *
  * The GameManager class controls the game flow by maintaining and managing various game subsystems such as camera,
  * status, movement, light, history, and game pieces. It also handles user input for camera control, light toggling,
  * piece movement, and game state transitions (pre-game, in-game, and end-game).
  *
  * This class follows the Singleton pattern, meaning only one instance of GameManager can exist during the game
  * lifecycle. It handles the game loop, processes user inputs via keyboard and special keys, and ensures that the
  * appropriate actions are taken at each stage of the game.
  */
class GameManager {
public:
	/**
	 * @brief Deleted copy constructor to prevent copying of the singleton instance.
	 */
	GameManager(const GameManager&) = delete;

	/**
	 * @brief Deleted copy assignment operator to prevent copying of the singleton instance.
	 */
	GameManager& operator=(const GameManager&) = delete;

    /**
     * @brief Destructor to release resources associated with the game.
     *
     * This destructor is responsible for cleaning up all resources allocated by the GameManager.
     * It performs the following operations:
     * - Frees the engine resources by calling `free()` on the `engine` instance.
     * - Deletes the `rootResetNode` and `rootNode`, which represent the root nodes in the game scene.
     *
     * This ensures that all game subsystems are properly cleaned up and that memory is freed when the
     * `GameManager` instance is destroyed, helping to prevent memory leaks.
     */
	~GameManager();

	/**
	 * @brief Returns the singleton instance of GameManager.
	 * @return Reference to the singleton GameManager instance.
	 */
	static GameManager& getInstance();

	/**
	 * @brief Sets the root node for the game scene.
	 * @param rootNode The root node to set for the scene.
	 */
	static void setRootNode(Node* rootNode);

	/**
	 * @brief Returns the root node of the game scene.
	 * @return Pointer to the root node.
	 */
	static Node* getRootNode();

	/**
	 * @brief Starts the game by loading the scene and initializing necessary components.
	 */
	void startGame();

private:
	/**
	 * @brief Initializes the game components.
	 */
	void initialize();

	/**
	 * @brief Main game loop that processes game logic and rendering.
	 */
	void gameLoop();

	/**
	 * @brief Resets the game state to the initial pre-game state.
	 */
	void resetGame();

	/**
	 * @brief Creates the graphics list for the game scene.
	 */
	void createGraphicsList();

    /**
        * @brief Handles keyboard input during the pre-game state.
        * @param key The pressed key.
        * @param mouseX The mouse X position.
        * @param mouseY The mouse Y position.
        */
    void keyboardCallbackPreGame(unsigned char key, int mouseX, int mouseY);

    /**
     * @brief Handles special key input during the pre-game state.
     * @param key The pressed special key.
     * @param mouseX The mouse X position.
     * @param mouseY The mouse Y position.
     */
    void specialKeyCallbackPreGame(int key, int mouseX, int mouseY);

    /**
     * @brief Displays the menu options during the pre-game state.
     * @return List of menu options as strings.
     */
    std::list<std::string> menuPreGame();

    /**
     * @brief Handles keyboard input during the choice state.
     * @param key The pressed key.
     * @param mouseX The mouse X position.
     * @param mouseY The mouse Y position.
     */
    void keyboardCallbackChoice(unsigned char key, int mouseX, int mouseY);

    /**
     * @brief Handles special key input during the choice state.
     * @param key The pressed special key.
     * @param mouseX The mouse X position.
     * @param mouseY The mouse Y position.
     */
    void specialKeyCallbackChoice(int key, int mouseX, int mouseY);

    /**
     * @brief Displays the menu options during the choice state.
     * @return List of menu options as strings.
     */
    std::list<std::string> menuChoice();

    /**
     * @brief Handles keyboard input during the game state.
     * @param key The pressed key.
     * @param mouseX The mouse X position.
     * @param mouseY The mouse Y position.
     */
    void keyboardCallbackGame(unsigned char key, int mouseX, int mouseY);

    /**
     * @brief Handles special key input during the game state.
     * @param key The pressed special key.
     * @param mouseX The mouse X position.
     * @param mouseY The mouse Y position.
     */
    void specialKeyCallbackGame(int key, int mouseX, int mouseY);

    /**
     * @brief Displays the menu options during the game state.
     * @return List of menu options as strings.
     */
    std::list<std::string> menuGame();

    /**
     * @brief Handles keyboard input during the end game state.
     * @param key The pressed key.
     * @param mouseX The mouse X position.
     * @param mouseY The mouse Y position.
     */
    void keyboardCallbackEndGame(unsigned char key, int mouseX, int mouseY);

    /**
     * @brief Handles special key input during the end game state.
     * @param key The pressed special key.
     * @param mouseX The mouse X position.
     * @param mouseY The mouse Y position.
     */
    void specialKeyCallbackEndGame(int key, int mouseX, int mouseY);

    /**
     * @brief Displays the menu options during the end game state.
     * @return List of menu options as strings.
     */
	std::list<std::string> menuEndGame();

    /**
     * @brief Handles the callback for closing the application.
     *
     * This method is invoked when the application needs to shut down. It updates the internal state to indicate
     * that the application is no longer running, allowing for a graceful termination of the game loop and other
     * ongoing processes.
     */
    void closeCallBack();

	GameManager();

    /**
    * @struct Reserved
    * @brief Struct to hold references to various game subsystems.
    *
    * This struct holds references to game components such as camera, status manager, movement manager, list of pieces
    * manager, history manager, light manager, engine, and root nodes for the game scene. It is used internally by
    * the GameManager class to interact with these subsystems.
    */
	struct Reserved;

	std::unique_ptr<Reserved> m_reserved;
};
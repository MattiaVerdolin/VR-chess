/**
 * @file    cameraManager.h
 * @brief   Manages multiple cameras for rendering a 3D chessboard game scene.
 *
 * This file declares the CameraManager class, which is responsible for handling
 * multiple cameras, including their creation, management, and transitions
 * during different game states. It provides methods to add, remove, and find cameras
 * by name and also handles camera movement and state updates.
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

// Project-specific headers
#include "onStateUpdate.h"
#include "statusManager.h"
#include "movementManager.h"
#include "perspCamera.h"
#include "orthoCamera.h"

/**
 * @brief Macro definitions for default cameras names.
 *
 * Defines the default names for the cameras used in the scene.
 */
#define MAIN_CAMERA "firstCamera"
#define CHESSBOARD_CAMERA "chessboardCamera"
#define PLAYER_WHITE_CAMERA "playerWhiteCamera"
#define PLAYER_BLACK_CAMERA "playerBlackCamera"
#define MENU_CAMERA "menuCamera"

////////////////////
// CAMERA MANAGER //
////////////////////

/**
 * @brief Singleton class for managing cameras in a 3D rendering scene.
 *
 * The CameraManager handles the creation, initialization, and management of
 * different cameras used in the 3D chessboard game. It provides functionality
 * to add and remove cameras, change the main camera, and move the camera's position.
 * It also listens to game state changes to update the active camera accordingly.
 */
class CameraManager : public OnStateUpdateListener
{
public:
    /**
     * @brief Deleted copy constructor to enforce singleton pattern.
     */
    CameraManager(const CameraManager&) = delete;

    /**
     * @brief Deleted assignment operator to enforce singleton pattern.
     */
    CameraManager& operator=(const CameraManager&) = delete;

    /**
     * @brief Destructor for CameraManager.
     * 
     * Unsubscribes the CameraManager as a listener from the StatusManager, 
     * ensuring that it no longer receives updates or notifications from the game state changes.
     */
    ~CameraManager();

    /**
     * @brief Retrieves the singleton instance of CameraManager.
     * @return A reference to the CameraManager instance.
     */
    static CameraManager& getInstance();

    /**
     * @brief Initializes the CameraManager by creating default cameras.
     */
    void initialize();

    /**
     * @brief Adds a new camera to the scene.
     * @param newCamera Pointer to the camera object to add.
     * @param parentNode Pointer to the parent node (default is root).
     * @return True if the camera was successfully added, false otherwise.
     */
    bool addNewCamera(Camera* newCamera, Node* parentNode = nullptr);

    /**
     * @brief Removes a camera from the scene by name.
     * @param cameraName The name of the camera to remove.
     * @param parentNode Pointer to the parent node (default is root).
     * @return True if the camera was successfully removed, false otherwise.
     */
    bool removeCamera(const std::string& cameraName, Node* parentNode = nullptr);

    /**
     * @brief Sets a new main camera by name.
     * @param cameraName The name of the new main camera.
     * @param parentNode Pointer to the parent node (default is root).
     * @return True if the main camera was successfully set, false otherwise.
     */
    bool setNewMainCamera(const std::string& cameraName, Node* parentNode = nullptr) const;

    /**
     * @brief Retrieves the current main camera.
     * @param parentNode Pointer to the parent node (default is root).
     * @return Pointer to the main Camera object.
     */
    Camera* getMainCamera(const Node* parentNode = nullptr) const;

    /**
     * @brief Finds a camera by its name.
     * @param cameraName The name of the camera to find.
     * @param parentNode Pointer to the parent node (default is root).
     * @return Pointer to the Camera object if found, nullptr otherwise.
     */
    Camera* findCameraByName(const std::string& cameraName, const Node* parentNode = nullptr) const;

    /**
     * @brief Moves the main camera to the right.
     */
    void moveCameraRight();

    /**
     * @brief Moves the main camera to the left.
     */
    void moveCameraLeft();

    /**
     * @brief Moves the main camera upwards.
     */
    void moveCameraUp();

    /**
     * @brief Moves the main camera downwards.
     */
    void moveCameraDown();

private:
    /**
     * @brief Private constructor for singleton pattern.
     */
    CameraManager();

    /**
     * @brief Creates default cameras for the game scene.
     */
    void createCameras();

    /**
     * @brief Handles the pre-game state transition.
     */
    void preGameHandler() override;

    /**
     * @brief Handles the choice state transition.
     */
    void choiceHandler() override;

    /**
     * @brief Handles the end-game state transition.
     */
    void endGameHandler() override;

    /// Reference to the StatusManager instance.
    StatusManager& m_statusManager;

    /// Reference to the MovementManager instance.
    MovementManager& m_movementManager;
};

/**
 * @file    lightManager.h
 * @brief   Declaration of the LightManager class, responsible for managing light states in the 3D scene.
 *
 * This file defines the LightManager class, which handles the activation, deactivation,
 * and intensity adjustments of lights in the scene. It ensures the proper interaction with
 * the light objects and follows the Singleton design pattern to ensure a single instance.
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

// Project-specific headers
#include "omniLight.h"
#include "spotLight.h"
#include "directLight.h"

/**
 * @brief Macro definitions for default light names.
 *
 * Defines the default names for the Omni and Spot lights used in the scene.
 */
#define LAMP_OMNI "Omni001"
#define CHESSBOARD_SPOT "Fspot001"

 /**
  * @class LightManager
  * @brief Singleton class responsible for managing light objects in the scene.
  *
  * The LightManager class provides functionality to manage and control the lights in the
  * 3D scene, including turning lights on and off, as well as adjusting their intensity.
  * It follows the Singleton pattern to ensure that only one instance of the manager exists
  * throughout the application.
  */
class LightManager
{
public:
    /**
     * @brief Deleted copy constructor.
     *
     * Prevents copying of the LightManager instance, enforcing the Singleton pattern.
     */
    LightManager(const LightManager&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     *
     * Prevents assignment of the LightManager instance, enforcing the Singleton pattern.
     */
    LightManager& operator=(const LightManager&) = delete;

    /**
     * @brief Get the singleton instance of the LightManager.
     *
     * Returns the unique instance of the LightManager, creating it if necessary.
     *
     * @return A reference to the singleton instance of LightManager.
     */
    static LightManager& getInstance();

    /**
     * @brief Toggle the state of a light (on/off).
     *
     * This function finds the light by its name and toggles its state (on or off).
     * If the light is off, it will be turned on, and vice versa.
     *
     * @param lightName The name of the light to toggle.
     * @param parent The parent node to search for the light within (default is the root node).
     */
    void pressButton(const std::string& lightName, Node* parent = nullptr);

    /**
     * @brief Increase the intensity of a light.
     *
     * This function finds the light by its name and increases its intensity.
     *
     * @param lightName The name of the light whose intensity will be increased.
     * @param parent The parent node to search for the light within (default is the root node).
     */
    void increaseLight(const std::string& lightName, Node* parent = nullptr);

    /**
     * @brief Decrease the intensity of a light.
     *
     * This function finds the light by its name and decreases its intensity.
     *
     * @param lightName The name of the light whose intensity will be decreased.
     * @param parent The parent node to search for the light within (default is the root node).
     */
    void decreaseLight(const std::string& lightName, Node* parent = nullptr);

private:
    /**
     * @brief Private constructor for LightManager.
     *
     * The constructor is private to prevent instantiating the class directly.
     * Use getInstance() to obtain the singleton instance.
     */
    LightManager();
};

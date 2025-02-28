/**
 * @file    textManager.h
 * @brief   Manager for text rendering.
 *
 * This file contains the `TextManager` class, which manages and displays text
 * on the graphical window. The class can render a list of strings and frames per second (FPS)
 * in a 3D context using an orthographic camera for rendering.
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
#include <list>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

// Project-specific includes
#include "orthoCamera.h"

/**
 * @brief Class for managing text rendering.
 *
 * The `TextManager` class handles rendering text on the graphical window using an
 * orthographic `Camera` for projection. It supports rendering a list of strings
 * and displaying frames per second (FPS).
 */
class ENG_API TextManager {
public:
    /**
     * @brief Deleted copy constructor to avoid unnecessary copies.
     */
    TextManager(const TextManager&) = delete;

    /**
     * @brief Deleted assignment operator to avoid unnecessary assignments.
     */
    TextManager& operator=(const TextManager&) = delete;

    /**
     * @brief Default destructor.
     */
    ~TextManager() = default;

    /**
     * @brief Returns the singleton instance of the `TextManager` class.
     *
     * This function provides access to the single instance of the class using the Singleton pattern.
     *
     * @return The singleton instance of the `TextManager` class.
     */
    static TextManager& getInstance();

    /**
     * @brief Renders a list of text strings on the window.
     *
     * This method uses orthographic projection to render text on the graphical window.
     * Each string is displayed starting at the defined vertical position, incrementing the
     * vertical position for each line of text.
     *
     * @param texts The list of strings to render.
     * @param orthoCamera The orthographic camera to use for projection.
     */
    void displayText(const std::list<std::string>& texts, Camera* orthoCamera) const;

    /**
     * @brief Displays the frames per second (FPS) on the window.
     *
     * This method renders the FPS value on the window, positioning it at the bottom of the window.
     *
     * @param fps The FPS value to display.
     * @param orthoCamera The orthographic camera to use for projection.
     */
    void displayFPS(const float& fps, Camera* orthoCamera) const;

private:
    /**
     * @brief Default constructor.
     */
    TextManager();

    /**
     * @brief Calculates the dynamic width for text rendering.
     *
     * Computes the maximum width of the list of strings based on the length of the longest string.
     *
     * @param texts The list of strings to calculate the width from.
     * @return The calculated maximum width.
     */
    float setDynamicWidth(const std::list<std::string>& texts) const;
};

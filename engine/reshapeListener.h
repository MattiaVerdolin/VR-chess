/**
 * @file    reshapelistener.h
 * @brief   Declaration of the ReshapeListener class, an interface for handling window resize events.
 *
 * This file defines the ReshapeListener class, which provides an interface for listening to
 * window reshape events. Classes that wish to respond to window resizing can implement this interface.
 *
 * @authors Luca Fantò (C) SUPSI [luca.fanto@student.supsi.ch]
 *          Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 *          Antonio Marroffino (C) SUPSI [antonio.marroffino@student.supsi.ch] */

#pragma once

// Specifies I/O linkage for Windows (VC++ spec)
#ifdef _WINDOWS
// Defines API linkage depending on whether ENGINE_EXPORTS is defined
#ifdef ENGINE_EXPORTS
#define ENG_API __declspec(dllexport)
#else
#define ENG_API __declspec(dllimport)
#endif

// Disables specific warnings related to DLL export/import
#pragma warning(disable : 4251)
#else // For Linux
#define ENG_API
#endif
#include "shader.h"

////////////////////////////////
// RESHAPE LISTENER INTERFACE //
////////////////////////////////

/**
 * @class ReshapeListener
 * @brief Interface for receiving notifications when a window is resized.
 *
 * The ReshapeListener interface defines a method that is called whenever the window size changes.
 * Classes that want to be notified of window reshape events must implement the onWindowReshape method.
 */
class ENG_API ReshapeListener {
public:
    /**
     * @brief Virtual destructor for the ReshapeListener class.
     *
     * Ensures proper cleanup of derived class resources when a ReshapeListener is destroyed.
     */
    virtual ~ReshapeListener() = default;

    /**
     * @brief Callback method to be called when the window is reshaped.
     *
     * This method must be implemented by any class that wishes to listen for window resize events.
     *
     * @param width The new width of the window.
     * @param height The new height of the window.
     */
    virtual void onWindowReshape(int width, int height, Shader* shader, int projLoc) = 0;
};

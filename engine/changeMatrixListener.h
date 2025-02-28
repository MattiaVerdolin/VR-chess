/**
 * @file    changematrixlistener.h
 * @brief   Declaration of the ChangeMatrixListener class, an interface for listening to matrix changes.
 *
 * This file defines the ChangeMatrixListener class, which provides a listener interface for
 * handling matrix changes. Classes that wish to be notified when a matrix changes can
 * implement this interface.
 *
 * @authors Luca Fantò (C) SUPSI [luca.fanto@student.supsi.ch]
 *          Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 *          Antonio Marroffino (C) SUPSI [antonio.marroffino@student.supsi.ch]
 */

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

//////////////////////////////////////
// CHANGE MATRIX LISTENER INTERFACE //
//////////////////////////////////////

/**
 * @class ChangeMatrixListener
 * @brief Interface for receiving notifications when a matrix changes.
 *
 * The ChangeMatrixListener interface defines a method that is called whenever a matrix is changed.
 * Classes that want to receive these notifications must implement the onMatrixChange method.
 */
class ENG_API ChangeMatrixListener {
public:
    /**
     * @brief Virtual destructor for the ChangeMatrixListener class.
     *
     * Ensures proper cleanup of derived class resources when a ChangeMatrixListener is destroyed.
     */
    virtual ~ChangeMatrixListener() = default;

    /**
     * @brief Callback method to be called when a matrix change occurs.
     *
     * This method must be implemented by any class that wishes to listen for matrix changes.
     *
     * @param matrixId The ID of the matrix that was changed.
     */
    virtual void onMatrixChange(const unsigned int& matrixId) = 0;
};

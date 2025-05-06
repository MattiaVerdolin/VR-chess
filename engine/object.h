/**
 * @file    object.h
 * @brief   Declaration of the Object class, an abstract base class for 3D scene objects.
 *
 * This file defines the Object class, which includes methods for object management
 * (e.g., ID generation, name management) and rendering. The class is designed as an
 * abstract base class, requiring derived classes to implement specific functionality.
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
#include <string>
#include <iostream>
#include <iomanip>
#include <limits.h>
#include <cstring>

// Project-specific headers
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

////////////////
// VERSIONING //
////////////////

#ifdef _WINDOWS
    // Specifies I/O linkage for Windows (VC++ spec)
#ifdef ENGINE_EXPORTS
#define ENG_API __declspec(dllexport)
#else
#define ENG_API __declspec(dllimport)
#endif
#pragma warning(disable : 4251) // Disable specific warnings
#else
    // Linux compatibility
#define ENG_API
#endif

///////////////////////////
// ABSTRACT OBJECT CLASS //
///////////////////////////

/**
 * @brief Abstract base class for 3D objects in a rendering engine.
 *
 * The Object class provides common functionality such as ID generation,
 * name management, and a virtual render method that must be implemented
 * by derived classes.
 */
class ENG_API Object {
public:
    /**
     * @brief Destructor for the Object class.
     */
    virtual ~Object() = default;

    /**
     * @brief Retrieve the ID of the object.
     * @return A constant reference to the object's ID.
     */
    const unsigned int& getId() const;

    /**
     * @brief Retrieve the name of the object.
     * @return A constant reference to the object's name.
     */
    const std::string& getName() const;

    /**
     * @brief Set the name of the object.
     * @param name The new name to assign to the object.
     */
    void setName(const std::string& name);

    /**
     * @brief Render the object with a given transformation matrix.
     *
     * This method is pure virtual and must be implemented by derived classes.
     *
     * @param transform The transformation matrix to apply (default is the identity matrix).
     */
    virtual void render(const glm::mat4& transform = glm::mat4(1.0f)) = 0;

    /**
     * @brief Parse the object from a character buffer.
     *
     * This method is pure virtual and must be implemented by derived classes.
     *
     * @param buffer The character buffer containing object data.
     * @param offset The current offset within the buffer.
     * @return The new offset after parsing the object.
     */
    virtual const unsigned int parse(const char* buffer, unsigned int& offset) = 0;

protected:
    /**
     * @brief Constructor with a specified name.
     * @param name The name of the object (default is "defaultObject").
     */
    Object(const std::string& name = "defaultObject");

    /**
     * @brief Copy constructor.
     * @param other The Object instance to copy.
     */
    Object(const Object& other);

    /**
     * @brief Copy assignment operator.
     * @param other The Object instance to assign.
     * @return A reference to the assigned Object instance.
     */
    Object& operator=(const Object& other);

    /**
     * @brief Generate a new unique ID.
     * @return The newly generated unique ID.
     */
    unsigned int createNewId();

    /// The unique ID of the object.
    unsigned int m_id;

    /// The name of the object.
    std::string m_name;

    /// Counter to keep track of the number of objects and generate unique IDs.
    static unsigned int m_cnt_object;
};

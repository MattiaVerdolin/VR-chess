/**
 * @file		ovoObjectFactory.h
 * @brief	OVO Object Factory header file
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
#include "object.h"

 /**
  * @brief Factory class to create objects based on chunk ID and data.
  *
  * The `OVOObjectFactory` class provides a static method `createObjectByChunkID()` to create
  * various objects (e.g., nodes, materials, meshes, lights) depending on the provided chunk ID and data.
  * The factory class ensures that objects are created in a structured manner based on their type and subtype.
  */
class ENG_API OVOObjectFactory {
public:
    /**
     * @brief Deleted default constructor to prevent instantiation.
     */
    OVOObjectFactory() = delete;

    /**
     * @brief Deleted copy constructor to prevent copying of the factory object.
     */
    OVOObjectFactory(const OVOObjectFactory&) = delete;

    /**
     * @brief Deleted assignment operator to prevent assignment of factory objects.
     */
    OVOObjectFactory& operator=(const OVOObjectFactory&) = delete;

    /**
     * @brief Creates an object based on a chunk ID and associated data.
     *
     * This method uses the chunk ID and data to determine the type of object to create and returns
     * a pointer to the newly created object.
     *
     * @param chunkID The chunk ID representing the object type.
     * @param data The data associated with the object.
     * @return A pointer to the created object, or `nullptr` if an unsupported type is encountered.
     */
    static Object* createObjectByChunkID(const unsigned int& chunkID, const char* data);

private:
    /**
     * @brief Skips to the subtype section of the data.
     *
     * This helper method processes the data and extracts the subtype information. It is used
     * specifically for objects of type `LIGHT` to determine the kind of light (e.g., omni, directional, spot).
     *
     * @param data The data to process.
     * @return The subtype of the light object.
     */
    static const unsigned char skipToSubtype(const char* data);

    /**
     * @brief Creates a light object based on its subtype.
     *
     * This method creates a specific light object (e.g., `DirectLight`, `OmniLight`, `SpotLight`)
     * based on the provided subtype value.
     *
     * @param subtype The subtype of the light.
     * @return A pointer to the created light object, or `nullptr` if the subtype is unsupported.
     */
    static Object* createLight(const unsigned char& subtype);
};

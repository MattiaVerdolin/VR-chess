/**
 * @file    texture.h
 * @brief   Declaration of the Texture class, a concrete class for handling 2D textures in the rendering engine.
 *
 * This file defines the Texture class, which provides functionality for loading,
 * managing, and rendering 2D textures. It also supports anisotropic filtering
 * if the feature is available on the system.
 *
 * @authors Luca Fantò (C) SUPSI [luca.fanto@student.supsi.ch]
 *          Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 *          Antonio Marroffino (C) SUPSI [antonio.marroffino@student.supsi.ch]
 */

#pragma once

//////////////
// #INCLUDE //
//////////////

// Project-specific includes
#include "object.h"

// Standard libraries
#include <memory>

///////////////////////
// MAIN TEXTURE CLASS //
///////////////////////

/**
 * @brief Concrete class for 2D textures in a rendering engine.
 *
 * The Texture class is responsible for loading, managing, and rendering
 * 2D textures. It includes support for anisotropic filtering and interacts
 * with OpenGL for texture operations.
 */
class ENG_API Texture : public Object {
public:
    /**
     * @brief Constructor that initializes a texture with a specific name.
     * @param name The name of the texture, used for loading the corresponding file.
     */
    Texture(const std::string& name);

    /**
     * @brief Destructor that releases texture resources.
     *
     * This destructor ensures that all resources allocated for the texture,
     * such as the OpenGL texture ID and any associated data, are properly
     * released to avoid memory leaks or dangling pointers.
     */
    ~Texture();

    /**
     * @brief Render the texture with the specified transformation matrix.
     *
     * This method binds the texture and applies the necessary OpenGL settings
     * to render it.
     *
     * @param matrix The transformation matrix to apply (default is the identity matrix).
     */
    void render(const glm::mat4& matrix = glm::mat4(1.0f)) override;

    /**
     * @brief Parse the texture from a file based on its name.
     *
     * This method loads the texture file using FreeImage, flips it vertically,
     * and prepares it for rendering.
     *
     * @param data The character buffer containing texture-related data (not used in this implementation).
     * @param position The current offset within the buffer (not used in this implementation).
     * @return The new offset after parsing the texture (always 0 in this implementation).
     */
    const unsigned int parse(const char* data, unsigned int& position) override;

    void createWhiteTexture();


private:
    /**
     * @brief Internal structure for managing reserved resources.
     *
     * This structure holds texture-related data such as the OpenGL texture ID
     * and the FreeImage bitmap.
     */
    struct Reserved;

    /// Unique pointer to manage reserved resources.
    std::unique_ptr<Reserved> m_reserved;

    /// Indicates if anisotropic filtering is supported.
    static bool isAnisotropicSupported;

    /// The maximum anisotropic filtering level supported.
    static int anisotropicLevel;

    /// Indicates if anisotropic filtering has already been set.
    static bool isAnisotropicAlreadySet;

    /**
     * @brief Configure anisotropic filtering settings.
     *
     * This method checks for anisotropic filtering support in the system
     * and sets the maximum available level.
     */
    static void setAnisotropicFilter();
};

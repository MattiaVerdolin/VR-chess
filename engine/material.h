/**
 * @file		material.h
 * @brief	Material class header file
 *
 * @author	Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 * @author	Riccardo Cristallo (C) SUPSI [riccardo.cristallo@student.supsi.ch]
 * @author	Mattia Verdolin (C) SUPSI [mattia.verdolin@student.supsi.ch]
 *
 * This file contains the declaration of the Material class,
 * which handles material properties for rendering, including
 * texture management and material attributes such as emission,
 * ambient, specular, and diffuse components.
 */

#pragma once

//////////////
// #INCLUDE //
//////////////

// Standard libraries
#include <map>

// Project-specific headers
#include "object.h"
#include "texture.h"

///////////////////////////
// MAIN MATERIAL CLASS //
///////////////////////////

/**
 * @brief Concrete Material class for rendering.
 *
 * The Material class represents a material that can be applied
 * to 3D objects during rendering. It manages material properties
 * like colors, shininess, alpha transparency, and textures.
 */
class ENG_API Material : public Object {
public:
    /**
     * @brief Construct a Material with an optional name.
     * @param name The name of the material. Defaults to "defaultMaterial".
     */
    Material(const std::string& name = "defaultMaterial");

    /**
     * @brief Copy constructor.
     * @param other The Material instance to copy.
     */
    Material(const Material& other);

    /**
     * @brief Destructor for the Material class.
     *
     * The destructor is responsible for cleaning up resources associated with the Material object.
     */
    ~Material() = default;

    /**
     * @brief Deleted assignment operator.
     */
    Material& operator=(const Material&) = delete;

    /**
     * @brief Render the material.
     * @param matrix Transformation matrix for rendering. Defaults to identity.
     */
    void render(const glm::mat4& matrix = glm::mat4(1.0f)) override;

    /**
     * @brief Parse material data from a binary format.
     * @param data Pointer to the binary data.
     * @param position Current position in the data stream.
     * @return The parsed size of the material data.
     */
    const unsigned int parse(const char* data, unsigned int& position) override;

    // Setters and getters for material properties
    void setAlpha(const float& alpha);
    const float& getAlpha() const;

    void setEmission(const glm::vec3& emission);
    const glm::vec3& getEmission() const;

    void setAmbient(const glm::vec3& ambient);
    const glm::vec3& getAmbient() const;

    void setSpecular(const glm::vec3& specular);
    const glm::vec3& getSpecular() const;

    void setDiffuse(const glm::vec3& diffuse);
    const glm::vec3& getDiffuse() const;

    void setShininess(const float& shininess);
    const float& getShininess() const;

    void setTexture(Texture* texture);
    const Texture* getTexture() const;

    /**
     * @brief Enable the texture for rendering if available.
     */
    void setEnableTexture();

    /**
     * @brief Disable the texture for rendering if available.
     */
    void setDisableTexture();

    /**
     * @brief Check if a texture is assigned to the material.
     * @return True if a texture exists, false otherwise.
     */
    bool isTextureExists();

    /**
     * @brief Static map of textures associated with their names.
     */
    static std::map<std::string, Texture*> m_texturesMap;

private:
    /**
     * @brief Retrieve a texture by name.
     * @param textureName The name of the texture.
     * @return A pointer to the texture if found, nullptr otherwise.
     */
    Texture* getTexture(const std::string& textureName) const;

    float m_alpha;           ///< Alpha transparency of the material.
    glm::vec3 m_emission;    ///< Emission color of the material.
    glm::vec3 m_ambient;     ///< Ambient color of the material.
    glm::vec3 m_specular;    ///< Specular color of the material.
    glm::vec3 m_diffuse;     ///< Diffuse color of the material.
    float m_shininess;       ///< Shininess factor of the material.
    Texture* m_texture;      ///< Pointer to the texture assigned to the material.
};

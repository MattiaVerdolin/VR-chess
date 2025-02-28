/**
 * @file    omniLight.h
 * @brief   OmniLight include file
 *
 * This file defines the OmniLight class, which represents an omnidirectional light source in a 3D scene.
 * The OmniLight emits light in all directions from a single point.
 *
 * @authors Luca Fantò (C) SUPSI [luca.fanto@student.supsi.ch]
 *          Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 *          Antonio Marroffino (C) SUPSI [antonio.marroffino@student.supsi.ch]
 */

#pragma once

//////////////
// #INCLUDE //
//////////////

// Project-specific headers
#include "light.h"

//////////////////////////
// MAIN OMNILIGHT CLASS //
//////////////////////////

/**
 * @brief Concrete OmniLight class.
 *
 * The OmniLight class represents an omnidirectional light source in a 3D scene. Unlike spotlights, which emit light
 * in a specific direction, an omnidirectional light emits light uniformly in all directions from a single point.
 */
class ENG_API OmniLight : public Light {
public:
    /**
     * @brief Constructor for initializing the omnidirectional light with specific parameters.
     *
     * This constructor creates an OmniLight object with the given name and position. The cutoff angle is set to the default
     * of 180 degrees, as it emits light in all directions.
     *
     * @param name The name of the omni light (default: "defaultOmniLight").
     * @param position The position of the omni light in 3D space (default: glm::vec3(0.0f)).
     */
    OmniLight(const std::string& name = "defaultOmniLight", const glm::vec3& position = glm::vec3(0.0f));

    /**
     * @brief Destructor for the OmniLight class.
     *
     * The destructor cleans up any resources associated with the OmniLight object.
     */
    ~OmniLight() = default;

    /**
     * @brief Copy constructor for creating an OmniLight object from another OmniLight.
     *
     * This constructor creates a new OmniLight object by copying the properties of an existing OmniLight.
     *
     * @param other The OmniLight object to copy.
     */
    OmniLight(const OmniLight& other);

    /**
     * @brief Assignment operator is deleted to prevent assignment of OmniLight objects.
     */
    OmniLight& operator=(const OmniLight&) = delete;

    /**
     * @brief Renders the omnidirectional light in the scene.
     *
     * This method updates the OpenGL settings for rendering the OmniLight with its current properties.
     * It sets the cutoff angle (always 180 degrees for omnidirectional lights) and other light properties.
     *
     * @param matrix The transformation matrix to apply (default: glm::mat4(1.0f)).
     */
    void render(const glm::mat4& matrix = glm::mat4(1.0f)) override;

    /**
     * @brief Clones the OmniLight object.
     *
     * This method creates and returns a new clone of the OmniLight object, including its properties.
     *
     * @return A new cloned OmniLight object.
     */
    Node* clone() const override;

    /**
     * @brief Gets the cutoff angle of the omni light.
     *
     * This method returns the cutoff angle of the omnidirectional light, which is fixed to 180 degrees.
     *
     * @return The cutoff angle in degrees (always 180.0f).
     */
    const float& getCutoff() const;

private:
    /**
     * @brief The cutoff angle for the omni light (in degrees).
     *
     * Since omni lights emit light in all directions, the cutoff is fixed to 180 degrees.
     */
    float m_cutoff;
};

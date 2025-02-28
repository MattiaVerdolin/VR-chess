/**
 * @file    light.h
 * @brief   Declaration of the Light class, an abstract base class for managing light sources.
 *
 * This file defines the Light class, which provides functionality for managing light sources in a 3D scene.
 * It includes methods to control light properties like position, intensity, and status (on/off),
 * as well as rendering the light source.
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
#include "node.h"

// Standard libraries
#include <memory>

///////////////////////
// MAIN LIGHT CLASS //
///////////////////////

/**
 * @brief Abstract base class for light sources.
 *
 * The Light class is an abstract base class that represents a light source in a 3D scene.
 * It manages the position, intensity, and on/off status of the light, and allows for rendering
 * the light as part of a scene. Derived classes can provide specific light types and behavior.
 */
class ENG_API Light : public Node {
public:
    /**
     * @brief Render the light with a given transformation matrix.
     *
     * This method applies the transformation matrix and renders a representation of the light.
     * The light's material properties (ambient, diffuse, and specular) are set to the OpenGL context.
     *
     * @param matrix The transformation matrix to apply to the light (default is the identity matrix).
     */
    virtual void render(const glm::mat4& matrix = glm::mat4(1.0f)) override;

    /**
     * @brief Destructor for the Light class.
     *
     * The destructor is responsible for cleaning up resources associated with the Light object.
     */
    virtual ~Light() = default;

    /**
     * @brief Parse the light data from a buffer and initialize the light's properties.
     *
     * This method loads light-specific data (such as color and radius) from a provided buffer
     * and updates the light's material properties accordingly.
     *
     * @param data The buffer containing the light data.
     * @param position The current position within the buffer (updated as the data is parsed).
     * @return The number of child nodes parsed (not used in this implementation).
     */
    const virtual unsigned int parse(const char* data, unsigned int& position) override;

    /**
     * @brief Clone the current light object.
     *
     * This method creates and returns a copy of the current light object.
     * It is pure virtual, and must be implemented by derived classes.
     *
     * @return A new instance of the light object.
     */
    virtual Node* clone() const override = 0;

    /**
     * @brief Set the position of the light in 3D space.
     *
     * This method updates the position of the light source.
     *
     * @param position The new position for the light source.
     */
    virtual void setPosition(const glm::vec3& position);

    /**
     * @brief Get the current position of the light.
     *
     * @return The current position of the light as a glm::vec4.
     */
    const glm::vec4& getPosition() const;

    /**
     * @brief Reset the light counter and disable previously active lights.
     *
     * This method resets the light counter, effectively disabling any active lights.
     */
    static void resetLightCounter();

    /**
     * @brief Turn the light on, restoring its material properties.
     *
     * This method restores the light's material properties (ambient, diffuse, and specular) to their original values.
     */
    void turnOn();

    /**
     * @brief Turn the light off, disabling its material properties.
     *
     * This method disables the light by setting its material properties to black (0, 0, 0).
     */
    void turnOff();

    /**
     * @brief Increase the light's intensity.
     *
     * This method increases the light's diffuse intensity by a small increment.
     * The intensity is capped at a maximum value.
     */
    void increaseIntensity();

    /**
     * @brief Decrease the light's intensity.
     *
     * This method decreases the light's diffuse intensity by a small increment.
     * The intensity is capped at a minimum value.
     */
    void decreaseIntensity();

    /**
     * @brief Check if the light is currently on.
     *
     * @return A boolean indicating whether the light is on (true) or off (false).
     */
    const bool& isLightOn() const;

protected:
    /**
     * @brief Constructor for initializing a light with a name and position.
     *
     * This constructor initializes the light with a given name and position in 3D space.
     *
     * @param name The name of the light.
     * @param position The position of the light in 3D space.
     */
    Light(const std::string& name, const glm::vec4& position);

    /**
     * @brief Copy constructor for creating a new light object based on an existing one.
     *
     * This constructor creates a copy of an existing light object, including its material properties.
     *
     * @param other The light object to copy.
     */
    Light(const Light& other);

    /**
     * @brief Assignment operator is deleted to prevent assignment of light objects.
     *
     * The assignment operator is not allowed for the Light class.
     */
    Light& operator=(const Light&) = delete;

    /// The position of the light in 3D space.
    glm::vec4 m_position;

    /// The material properties of the light (ambient, diffuse, specular).
    Material* m_lightMaterial;

    /// Temporary albedo values used when turning the light on/off.
    glm::vec3 m_tempAlbedo;

    /// Flag indicating whether the light is currently on.
    bool m_lightIsOn;

    /// The maximum number of active lights.
    static unsigned int maxNumberOfActiveLights;

    /// The current active light counter.
    static unsigned int lightActiveCounter;
};

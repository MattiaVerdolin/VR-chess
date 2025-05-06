/**
 * @file		spotLight.h
 * @brief	Spot Light header file
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
#include "light.h"

//////////////////////////
// MAIN SPOTLIGHT CLASS //
//////////////////////////

/**
 * @brief Concrete SpotLight class.
 *
 * The SpotLight class represents a spotlight light source in a 3D scene. A spotlight is a light source that
 * emits light in a specific direction, typically with a defined angle of influence (cutoff) and an intensity
 * falloff based on the angle (spot exponent).
 */
class ENG_API SpotLight : public Light {
public:
    /**
     * @brief Constructor for initializing the spotlight with specific parameters.
     *
     * This constructor creates a SpotLight object with the given name, position, direction, cutoff angle,
     * and spot exponent. Default values are provided for each parameter.
     *
     * @param name The name of the spotlight (default: "defaultSpotLight").
     * @param position The position of the spotlight in 3D space (default: glm::vec3(0.0f)).
     * @param direction The direction of the spotlight beam (default: glm::vec3(1.0f)).
     * @param cutoff The angle of the spotlight's cutoff (default: 45.0f).
     * @param spotExponent The spot exponent that controls the intensity falloff (default: 1.0f).
     */
    SpotLight(const std::string& name = "defaultSpotLight", const glm::vec3& position = glm::vec3(0.0f),
        const glm::vec3& direction = glm::vec3(1.0f), const float& cutoff = 45.0f, const float& spotExponent = 1.0f);

    /**
     * @brief Default destructor for the SpotLight class.
     * 
     * The destructor cleans up any resources associated with the SpotLight object.
     */
    ~SpotLight() = default;

    /**
     * @brief Copy constructor for creating a SpotLight object from another SpotLight.
     *
     * This constructor creates a new SpotLight object by copying the properties of an existing SpotLight.
     *
     * @param other The SpotLight object to copy.
     */
    SpotLight(const SpotLight& other);

    /**
     * @brief Assignment operator is deleted to prevent assignment of SpotLight objects.
     */
    SpotLight& operator=(const SpotLight&) = delete;

    /**
     * @brief Renders the spotlight in the scene.
     *
     * This method updates the OpenGL settings for rendering the spotlight with its current properties.
     * It configures the spotlight's cutoff angle, direction, and spot exponent.
     *
     * @param matrix The transformation matrix to apply (default: glm::mat4(1.0f)).
     */
    void render(const glm::mat4& matrix = glm::mat4(1.0f)) override;

    /**
     * @brief Parses the data for the spotlight from a file or buffer.
     *
     * This method reads the spotlight data from the provided byte stream and updates the spotlight's properties.
     *
     * @param data The byte stream containing the spotlight data.
     * @param position The position in the byte stream to start reading from.
     * @return The number of child nodes (if any) parsed from the data.
     */
    const unsigned int parse(const char* data, unsigned int& position) override;

    /**
     * @brief Clones the SpotLight object.
     *
     * This method creates and returns a new clone of the SpotLight object, including its properties.
     *
     * @return A new cloned SpotLight object.
     */
    Node* clone() const override;

    /**
     * @brief Sets the direction of the spotlight.
     *
     * This method sets the direction in which the spotlight's beam is emitted.
     *
     * @param direction The direction vector for the spotlight's beam.
     */
    void setDirection(const glm::vec3& direction);

    /**
     * @brief Gets the direction of the spotlight.
     *
     * This method returns the current direction of the spotlight's beam.
     *
     * @return The direction vector of the spotlight's beam.
     */
    const glm::vec3& getDirection() const;

    /**
     * @brief Sets the cutoff angle of the spotlight.
     *
     * This method sets the cutoff angle for the spotlight's beam. The cutoff angle defines the area where
     * the spotlight's light begins to fade out.
     *
     * @param cutoff The cutoff angle in degrees (range: 0.0f to 90.0f).
     */
    void setCutoff(const float& cutoff);

    /**
     * @brief Gets the cutoff angle of the spotlight.
     *
     * This method returns the current cutoff angle of the spotlight's beam.
     *
     * @return The cutoff angle in degrees.
     */
    const float& getCutoff() const;

    /**
     * @brief Sets the spot exponent of the spotlight.
     *
     * This method sets the spot exponent that controls the falloff of the spotlight's intensity.
     * A higher exponent makes the spotlight's intensity fall off more sharply.
     *
     * @param spotExponent The spot exponent value.
     */
    void setSpotExponent(const float& spotExponent);

    /**
     * @brief Gets the spot exponent of the spotlight.
     *
     * This method returns the current spot exponent of the spotlight, which controls the falloff of intensity.
     *
     * @return The spot exponent.
     */
    const float& getSpotExponent() const;

private:
    /**
     * @brief The direction of the spotlight's beam.
     */
    glm::vec3 m_direction;

    /**
     * @brief The cutoff angle for the spotlight's beam (in degrees).
     */
    float m_cutoff;

    /**
     * @brief The spot exponent that controls the intensity falloff of the spotlight.
     */
    float m_spotExponent;
};

/**
 * @file		directLight.h
 * @brief	Direct light class header file
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

////////////////////////////
// MAIN DIRECTLIGHT CLASS //
////////////////////////////

/**
 * @brief Concrete DirectLight class.
 *
 * The DirectLight class represents a directional light source in a 3D scene. Unlike other light types,
 * a directional light has no position but instead shines with parallel rays in a specific direction.
 * The DirectLight class extends the abstract Light class, providing specific behavior for a directional light.
 */
class ENG_API DirectLight : public Light {
public:
    /**
     * @brief Constructor for creating a new DirectLight object with a specified name and direction.
     *
     * This constructor initializes the directional light with a given name and direction vector. The position
     * is set to a default value with the w-component set to 0, as the direction is more important than the position.
     *
     * @param name The name of the light (default is "defaultDirectLight").
     * @param direction The direction of the light (default is glm::vec3(0.0f)).
     */
    DirectLight(const std::string& name = "defaultDirectLight", const glm::vec3& direction = glm::vec3(0.0f));

    /**
     * @brief Default destructor for the DirectLight class.
     * 
     * The destructor cleans up any resources associated with the DirectLight object.
     */
    ~DirectLight() = default;

    /**
     * @brief Copy constructor for creating a new DirectLight object based on an existing one.
     *
     * This constructor creates a new DirectLight object by copying the properties of an existing one.
     *
     * @param other The DirectLight object to copy.
     */
    DirectLight(const DirectLight& other);

    /**
     * @brief Assignment operator is deleted to prevent assignment of DirectLight objects.
     *
     * The assignment operator is not allowed for the DirectLight class.
     */
    DirectLight& operator=(const DirectLight&) = delete;

    /**
     * @brief Render the direct light with a given transformation matrix.
     *
     * This method applies the transformation matrix and renders a representation of the directional light.
     * The light's material properties (ambient, diffuse, and specular) are set to the OpenGL context, and the
     * light is activated for rendering.
     *
     * @param matrix The transformation matrix to apply to the light (default is the identity matrix).
     */
    void render(const glm::mat4& matrix = glm::mat4(1.0f)) override;

    /**
     * @brief Clone the current DirectLight object.
     *
     * This method creates and returns a copy of the current DirectLight object, including all properties.
     *
     * @return A new instance of the DirectLight object.
     */
    Node* clone() const override;

    /**
     * @brief Set the direction of the light (position).
     *
     * This method updates the position of the directional light. The position's w-component is set to 0 to indicate
     * that the light is directional (no specific position).
     *
     * @param direction The new direction of the light.
     */
    void setPosition(const glm::vec3& direction) override;
};

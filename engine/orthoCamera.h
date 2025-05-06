/**
 * @file		orthoCamera.h
 * @brief	Orthographic Camera header file
 *
 * @author	Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 * @author	Riccardo Cristallo (C) SUPSI [riccardo.cristallo@student.supsi.ch]
 * @author	Mattia Verdolin (C) SUPSI [mattia.verdolin@student.supsi.ch]
 */

#pragma once

//////////////
// #INCLUDE //
//////////////

// Project-specific headers
#include "camera.h"

////////////////////////////
// MAIN ORTHOCAMERA CLASS //
////////////////////////////

/**
 * @brief Concrete OrthoCamera class.
 *
 * The OrthoCamera class represents a camera that uses an orthographic projection, where objects are projected
 * without perspective distortion (i.e., parallel lines remain parallel). It inherits from the Camera class and
 * implements the necessary methods to load and apply the orthographic projection matrix.
 */
class ENG_API OrthoCamera : public Camera {
public:
    /**
     * @brief Constructor for initializing the orthographic camera.
     *
     * This constructor initializes the OrthoCamera with the provided parameters, including the camera's
     * name, viewport dimensions, near and far clipping planes, and the initial left and bottom boundaries of
     * the orthographic projection.
     *
     * @param name The name of the camera.
     * @param width The width of the camera's viewport.
     * @param height The height of the camera's viewport.
     * @param near_val The near clipping plane distance.
     * @param far_val The far clipping plane distance.
     */
    OrthoCamera(const std::string& name, const float& width, const float& height, const float& near_val, const float& far_val);

    /**
     * @brief Destructor for the OrthoCamera class.
     *
     * The destructor releases any resources associated with the OrthoCamera object.
     */
    ~OrthoCamera() = default;

    /**
     * @brief Copy constructor for creating a new OrthoCamera object from an existing one.
     *
     * This constructor creates a new OrthoCamera object by copying the properties of an existing OrthoCamera.
     *
     * @param other The OrthoCamera object to copy.
     */
    OrthoCamera(const OrthoCamera& other);

    /**
     * @brief Assignment operator is deleted to prevent assignment of OrthoCamera objects.
     */
    OrthoCamera& operator=(const OrthoCamera&) = delete;

    /**
     * @brief Clone the OrthoCamera object.
     *
     * This method creates and returns a new clone of the OrthoCamera object.
     *
     * @return A new cloned OrthoCamera object.
     */
    Node* clone() const override;

    /**
     * @brief Load the orthographic projection matrix for the camera.
     *
     * This method loads the orthographic projection matrix based on the camera's left, bottom, width, and height.
     * It uses the `glm::ortho` function to create the orthographic projection matrix and assigns it to the
     * `m_projectionMatrix` member.
     */
    void loadProjectionMatrix() override;

private:
    /**
     * @brief The left boundary for the orthographic projection.
     */
    float m_left;

    /**
     * @brief The bottom boundary for the orthographic projection.
     */
    float m_bottom;
};

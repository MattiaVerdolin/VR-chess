/**
 * @file    perspCamera.h
 * @brief   PerspCamera include file
 *
 * This file defines the PerspCamera class, which represents a perspective camera in a 3D scene.
 * A perspective camera simulates a camera that uses perspective projection, where objects appear smaller
 * as they are farther from the camera.
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
#include "camera.h"

////////////////////////////
// MAIN PERSPCAMERA CLASS //
////////////////////////////

/**
 * @brief Concrete PerspCamera class.
 *
 * The PerspCamera class represents a perspective camera in a 3D scene. It simulates a camera using perspective
 * projection, where objects appear smaller as they get further from the camera.
 */
class ENG_API PerspCamera : public Camera {
public:
    /**
     * @brief Constructor for initializing the perspective camera with specific parameters.
     *
     * This constructor creates a PerspCamera object with the given name, screen dimensions, near and far planes,
     * and the vertical field of view (fovY).
     *
     * @param name The name of the perspective camera (default: "defaultPerspCamera").
     * @param width The width of the camera view (in pixels).
     * @param height The height of the camera view (in pixels).
     * @param near_val The near clipping plane distance.
     * @param far_val The far clipping plane distance.
     * @param fovY The vertical field of view in degrees.
     */
    PerspCamera(const std::string& name, const float& width, const float& height, const float& near_val, const float& far_val, const float& fovY);

    /**
     * @brief Destructor for the PerspCamera class.
     *
     * The destructor releases any resources associated with the PerspCamera object.
     */
    ~PerspCamera() = default;

    /**
     * @brief Copy constructor for creating a PerspCamera object from another PerspCamera.
     *
     * This constructor creates a new PerspCamera object by copying the properties of an existing PerspCamera.
     *
     * @param other The PerspCamera object to copy.
     */
    PerspCamera(const PerspCamera& other);

    /**
     * @brief Assignment operator is deleted to prevent assignment of PerspCamera objects.
     */
    PerspCamera& operator=(const PerspCamera&) = delete;

    /**
     * @brief Renders the perspective camera in the scene.
     *
     * This method updates the OpenGL settings for rendering the PerspCamera with its current properties.
     *
     * @return A new cloned PerspCamera object.
     */
    Node* clone() const override;

    /**
     * @brief Loads the projection matrix for the perspective camera.
     *
     * This method computes and sets the projection matrix for the perspective camera using the
     * vertical field of view (fovY), aspect ratio, near and far clipping planes.
     */
    void loadProjectionMatrix() override;

private:
    /**
     * @brief The vertical field of view (in degrees).
     *
     * The vertical field of view controls how wide or narrow the camera's view is. A higher value gives a
     * wider view, while a smaller value gives a narrower view.
     */
    float m_fovY;
};

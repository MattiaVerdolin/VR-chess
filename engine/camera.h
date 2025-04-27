/**
 * @file    camera.h
 * @brief   Declaration of the Camera class, an abstract base class for camera objects.
 *
 * This file defines the abstract base class for a camera in a 3D scene. The camera defines the projection
 * matrix and handles the window resizing event to update the projection matrix accordingly. Subclasses
 * of this class implement different types of camera behavior (e.g., perspective, orthographic).
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
#include "glm/gtc/matrix_transform.hpp"
#include "notificationService.h"

///////////////////////
// MAIN CAMERA CLASS //
///////////////////////

/**
 * @brief Abstract base Camera class.
 *
 * The Camera class is an abstract base class for implementing different types of cameras in a 3D scene.
 * The camera defines the projection matrix and provides an interface for handling camera-specific rendering
 * and projection. It also handles window resizing to adjust the viewport and update the projection matrix.
 */
class ENG_API Camera : public Node, public ReshapeListener {
public:
    /**
     * @brief Destructor for cleaning up resources and unsubscribing from notifications.
     */
    virtual ~Camera();

    /**
     * @brief Load the projection matrix for the camera.
     *
     * This method is virtual and must be implemented in subclasses to load the appropriate projection matrix
     * for the specific type of camera (e.g., perspective, orthographic).
     */
    virtual void loadProjectionMatrix() = 0;

    /**
     * @brief Render the camera's view using a given transformation matrix.
     *
     * This method is responsible for setting up the OpenGL matrices for rendering the scene from the camera's
     * viewpoint. It uses the camera's projection matrix and modelview transformation to render the scene.
     *
     * @param matrix The transformation matrix to apply to the camera (default is the identity matrix).
     */
    virtual void render(const glm::mat4& matrix = glm::mat4(1.0f)) override;

    /**
     * @brief Parse camera-specific data from a buffer.
     *
     * This method parses the camera's data from a given buffer. It can be overridden in subclasses to handle
     * specific camera data.
     *
     * @param data The buffer containing the camera's data.
     * @param position The current position in the buffer to start parsing from.
     * @return The number of child nodes parsed.
     */
    const virtual unsigned int parse(const char* data, unsigned int& position) override { return 0; };

    /**
     * @brief Clone the camera object.
     *
     * This method creates and returns a copy of the camera object. It is a pure virtual function,
     * requiring subclasses to provide an implementation.
     *
     * @return A new cloned camera object.
     */
    virtual Node* clone() const override = 0;

    /**
     * @brief Handle window reshape events to update the projection matrix.
     *
     * This method is called when the window is resized, and it updates the camera's viewport and projection
     * matrix to match the new window dimensions.
     *
     * @param width The new width of the window.
     * @param height The new height of the window.
     */
    void onWindowReshape(int width, int height, Shader* shader) override;

    /**
     * @brief Get the inverse of the camera's final transformation matrix.
     *
     * This method returns the inverse of the camera's final transformation matrix, which is used for
     * various calculations, such as world-to-camera space transformations.
     *
     * @return The inverse of the camera's final transformation matrix.
     */
    const glm::mat4 getInverseCameraFinalMatrix() const;

    const glm::mat4 getProjectionMatrix() const;

    /**
     * @brief Set the camera as the main camera.
     *
     * This method marks the camera as the main camera, which may be used for rendering the scene.
     *
     * @param val True if this camera should be set as the main camera, false otherwise.
     */
    void setMainCamera(const bool& val);

    /**
     * @brief Check if the camera is the main camera.
     *
     * This method returns whether the camera is the main camera, typically used for rendering the scene.
     *
     * @return True if this is the main camera, false otherwise.
     */
    virtual const bool& isMainCamera() const;

protected:
    /**
     * @brief Constructor for initializing the camera with projection and viewport settings.
     *
     * This constructor initializes the camera with the given parameters, including its name, dimensions,
     * near and far planes, and a default notification service for event handling.
     *
     * @param name The name of the camera.
     * @param width The width of the camera's viewport.
     * @param height The height of the camera's viewport.
     * @param near_val The near clipping plane distance.
     * @param far_val The far clipping plane distance.
     */
    Camera(const std::string& name, const float& width, const float& height, const float& near_val, const float& far_val);

    /**
     * @brief Copy constructor for creating a new Camera object from an existing one.
     *
     * This constructor creates a new Camera object by copying the properties of an existing camera.
     *
     * @param other The Camera object to copy.
     */
    Camera(const Camera& other);

    /**
     * @brief Assignment operator is deleted to prevent assignment of Camera objects.
     */
    Camera& operator=(const Camera&) = delete;

    /**
     * @brief The camera's projection matrix.
     *
     * This matrix is used for transforming the 3D scene from camera space to screen space.
     */
    glm::mat4 m_projectionMatrix;

    /**
     * @brief Flag indicating if this is the current active camera.
     */
    bool m_isCurrentCamera;

    /**
     * @brief The width of the camera's viewport.
     */
    float m_width;

    /**
     * @brief The height of the camera's viewport.
     */
    float m_height;

    /**
     * @brief The near clipping plane distance.
     */
    float m_near;

    /**
     * @brief The far clipping plane distance.
     */
    float m_far;

    /**
     * @brief The notification service instance for event handling.
     */
    NotificationService& m_notificationService;
};

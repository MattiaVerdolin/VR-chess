/**
 * @file		ovr.h
 * @brief	Oculus VR SDK wrapper header file
 *
 * @author	Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 * @author	Riccardo Cristallo (C) SUPSI [riccardo.cristallo@student.supsi.ch]
 * @author	Mattia Verdolin (C) SUPSI [mattia.verdolin@student.supsi.ch]
 */
#pragma once

 // GLM:
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// OpenVR:
#include "openvr.h"

// C/C++:
#include <iostream>
#include <string>
#include <vector>
#include "object.h"

/**
 * @brief OpenGL-OpenVR interface.
 * 
 * This class provides a wrapper for the OpenVR SDK, handling VR device initialization,
 * tracking, rendering, and controller input management.
 */
class ENG_API OvVR {
public:
    /**
     * @brief Structure representing a VR controller.
     */
    struct Controller;

    /**
     * @brief Enumeration for VR eye types.
     */
    enum OvEye {
        EYE_LEFT = 0,  ///< Left eye
        EYE_RIGHT = 1, ///< Right eye
        EYE_LAST       ///< Terminator
    };

    /**
     * @brief Constructor for the OvVR class.
     * 
     * Initializes the VR system pointers to nullptr and clears the controllers list.
     */
    OvVR();

    /**
     * @brief Destructor for the OvVR class.
     */
    ~OvVR();

    /**
     * @brief Initializes the VR system and controllers.
     * 
     * This method initializes the OpenVR runtime, render models interface, and compositor.
     * It also detects and initializes any connected VR controllers.
     * 
     * @return true if initialization was successful, false otherwise.
     */
    bool init();

    /**
     * @brief Frees all VR system resources.
     * 
     * This method cleans up all VR-related resources, including controllers,
     * and shuts down the VR runtime.
     * 
     * @return true if cleanup was successful.
     */
    bool free();

    /**
     * @brief Gets the name of the tracking system.
     * 
     * @return A string containing the tracking system name.
     */
    std::string getTrackingSysName();

    /**
     * @brief Gets the manufacturer name of the VR device.
     * 
     * @return A string containing the manufacturer name.
     */
    std::string getManufacturerName();

    /**
     * @brief Gets the model number of the VR device.
     * 
     * @return A string containing the model number.
     */
    std::string getModelNumber();

    /**
     * @brief Prints information about available render models.
     * 
     * @return true if the operation was successful.
     */
    bool printRenderModels();

    /**
     * @brief Gets the ideal horizontal resolution for the HMD.
     * 
     * @return The recommended horizontal resolution in pixels.
     */
    unsigned int getHmdIdealHorizRes();

    /**
     * @brief Gets the ideal vertical resolution for the HMD.
     * 
     * @return The recommended vertical resolution in pixels.
     */
    unsigned int getHmdIdealVertRes();

    /**
     * @brief Gets the projection matrix for a specific eye.
     * 
     * @param eye The eye to get the projection matrix for.
     * @param nearPlane The near clipping plane distance.
     * @param farPlane The far clipping plane distance.
     * @return The projection matrix for the specified eye.
     */
    glm::mat4 getProjMatrix(OvEye eye, float nearPlane, float farPlane);

    /**
     * @brief Gets the eye-to-head transformation matrix.
     * 
     * @param eye The eye to get the transformation for.
     * @return The eye-to-head transformation matrix.
     */
    glm::mat4 getEye2HeadMatrix(OvEye eye);

    /**
     * @brief Gets the current modelview matrix for the HMD.
     * 
     * @return The modelview matrix representing the HMD's position and orientation.
     */
    glm::mat4 getModelviewMatrix();

    /**
     * @brief Updates the tracking state of all VR devices.
     * 
     * This method updates the poses of all tracked devices, including the HMD and controllers.
     * 
     * @return true if the update was successful.
     */
    bool update();

    /**
     * @brief Gets the number of connected controllers.
     * 
     * @return The number of detected VR controllers.
     */
    unsigned int getNrOfControllers();

    /**
     * @brief Gets a controller by its index.
     * 
     * @param pos The index of the controller to retrieve.
     * @return A pointer to the Controller object, or nullptr if the index is invalid.
     */
    Controller* getController(unsigned int pos) const;

    /**
     * @brief Sets the reprojection mode for the VR compositor.
     * 
     * @param flag true to enable interleaved reprojection, false to disable it.
     */
    void setReprojection(bool flag);

    /**
     * @brief Submits a rendered eye texture to the VR compositor.
     * 
     * @param eye The eye the texture is for.
     * @param eyeTexture The OpenGL texture ID to submit.
     */
    void pass(OvEye eye, unsigned int eyeTexture);

    /**
     * @brief Signals the end of the frame to the VR compositor.
     */
    void render();

    /**
     * @brief Converts an OpenVR 3x4 matrix to an OpenGL 4x4 matrix.
     * 
     * @param matrix The OpenVR matrix to convert.
     * @return The converted OpenGL matrix.
     */
    static glm::mat4 ovr2ogl(const vr::HmdMatrix34_t& matrix);

    /**
     * @brief Converts an OpenVR 4x4 matrix to an OpenGL 4x4 matrix.
     * 
     * @param matrix The OpenVR matrix to convert.
     * @return The converted OpenGL matrix.
     */
    static glm::mat4 ovr2ogl(const vr::HmdMatrix44_t& matrix);

    /**
     * @brief Structure representing a VR controller.
     */
    struct Controller {
        unsigned int id;                          ///< Controller device ID
        vr::VRControllerState_t pControllerState; ///< Current controller state
        glm::mat4 matrix;                         ///< Controller transformation matrix

        /**
         * @brief Constructor for the Controller structure.
         */
        ENG_API Controller();

        /**
         * @brief Destructor for the Controller structure.
         */
        ENG_API ~Controller();

        /**
         * @brief Gets the controller's transformation matrix.
         * 
         * @return The current transformation matrix of the controller.
         */
        ENG_API glm::mat4 getMatrix();

        /**
         * @brief Gets the state of a controller axis.
         * 
         * @param axisId The ID of the axis to query.
         * @param bPressed Reference to store the pressed buttons state.
         * @param bTouched Reference to store the touched buttons state.
         * @return A 2D vector containing the axis values.
         */
        ENG_API glm::vec2 getAxis(unsigned int axisId, unsigned long long int& bPressed, unsigned long long int& bTouched);

        /**
         * @brief Checks if a specific button is pressed.
         * 
         * @param id The button ID to check.
         * @param mask The button state mask.
         * @return true if the button is pressed, false otherwise.
         */
        ENG_API bool isButtonPressed(unsigned long long int id, unsigned long long int mask);
    };

private:
    vr::IVRSystem* vrSys;                                    ///< OpenVR system interface
    vr::IVRRenderModels* vrModels;                          ///< OpenVR render models interface
    vr::IVRCompositor* vrComp;                              ///< OpenVR compositor interface
    vr::TrackedDevicePose_t vrPoses[vr::k_unMaxTrackedDeviceCount]; ///< Array of device poses
    std::vector<Controller*> controllers;                    ///< List of connected controllers
};

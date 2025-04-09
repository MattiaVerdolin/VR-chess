/**
 * @file		ovr.h
 * @brief	Self-contained helper class for interfacing OpenGL and OpenVR. Shortened version of Overvision's OvVR module.
 *
 * @author	Achille Peternier (C) SUPSI [achille.peternier@supsi.ch]
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
 */
class ENG_API OvVR {
public:
    struct Controller;

    enum OvEye {
        EYE_LEFT = 0,
        EYE_RIGHT = 1,
        EYE_LAST
    };

    OvVR();
    ~OvVR();

    bool init();
    bool free();

    std::string getTrackingSysName();
    std::string getManufacturerName();
    std::string getModelNumber();

    bool printRenderModels();

    unsigned int getHmdIdealHorizRes();
    unsigned int getHmdIdealVertRes();

    glm::mat4 getProjMatrix(OvEye eye, float nearPlane, float farPlane);
    glm::mat4 getEye2HeadMatrix(OvEye eye);
    glm::mat4 getModelviewMatrix();

    bool update();

    unsigned int getNrOfControllers();
    Controller* getController(unsigned int pos) const;

    void setReprojection(bool flag);
    void pass(OvEye eye, unsigned int eyeTexture);
    void render();

    static glm::mat4 ovr2ogl(const vr::HmdMatrix34_t& matrix);
    static glm::mat4 ovr2ogl(const vr::HmdMatrix44_t& matrix);

    struct Controller {
        unsigned int id;
        vr::VRControllerState_t pControllerState;
        glm::mat4 matrix;

        ENG_API Controller();
        ENG_API ~Controller();

        ENG_API glm::mat4 getMatrix();
        ENG_API glm::vec2 getAxis(unsigned int axisId, unsigned long long int& bPressed, unsigned long long int& bTouched);
        ENG_API bool isButtonPressed(unsigned long long int id, unsigned long long int mask);
    };

private:
    vr::IVRSystem* vrSys;
    vr::IVRRenderModels* vrModels;
    vr::IVRCompositor* vrComp;
    vr::TrackedDevicePose_t vrPoses[vr::k_unMaxTrackedDeviceCount];
    std::vector<Controller*> controllers;
};

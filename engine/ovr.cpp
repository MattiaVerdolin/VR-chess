#include "ovr.h"

// GLEW:
#include "GL/glew.h"

// FreeGLUT:
#include "GL/freeglut.h"

using namespace std;

// --- OvVR Methods ---

ENG_API OvVR::OvVR() : vrSys(nullptr), vrModels(nullptr), vrComp(nullptr) {
    controllers.clear();
}

ENG_API OvVR::~OvVR() {}

ENG_API bool OvVR::init() {
    vr::EVRInitError error = vr::VRInitError_None;

    cout << "Using OpenVR " << vr::k_nSteamVRVersionMajor << "." << vr::k_nSteamVRVersionMinor << "." << vr::k_nSteamVRVersionBuild << endl;
    vrSys = vr::VR_Init(&error, vr::VRApplication_Scene);
    if (error != vr::VRInitError_None) {
        vrSys = nullptr;
        cout << "[ERROR] Unable to init VR runtime: " << vr::VR_GetVRInitErrorAsEnglishDescription(error) << endl;
        return false;
    }

    vrModels = (vr::IVRRenderModels*)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &error);
    if (!vrModels) {
        vrSys = nullptr;
        vr::VR_Shutdown();
        cout << "[ERROR] Unable to get render model interface: " << vr::VR_GetVRInitErrorAsEnglishDescription(error) << endl;
        return false;
    }

    vrComp = vr::VRCompositor();
    if (!vrComp) {
        vrModels = nullptr;
        vrSys = nullptr;
        vr::VR_Shutdown();
        cout << "[ERROR] Unable to get VR compositor" << endl;
        return false;
    }

    controllers.clear();
    for (unsigned int c = 0; c < vr::k_unMaxTrackedDeviceCount; c++) {
        if (vrSys->GetTrackedDeviceClass(c) == vr::TrackedDeviceClass_Controller) {
            cout << "   Found controller at " << c << endl;
            Controller* cont = new Controller();
            cont->id = c;

            unsigned int bufferLen = vrSys->GetStringTrackedDeviceProperty(c, vr::Prop_RenderModelName_String, nullptr, 0, nullptr);
            if (bufferLen == 0) {
                cout << "[ERROR] Unable to get controller render model" << endl;
                delete cont;
                continue;
            }

            string result;
            result.resize(bufferLen);
            vrSys->GetStringTrackedDeviceProperty(c, vr::Prop_RenderModelName_String, &result[0], bufferLen, nullptr);
            cout << "   Controller render model: '" << result << "'" << endl;
            controllers.push_back(cont);
        }
    }

    return true;
}

ENG_API bool OvVR::free() {
    for (Controller* c : controllers)
        delete c;
    controllers.clear();

    vr::VR_Shutdown();
    vrComp = nullptr;
    vrModels = nullptr;
    vrSys = nullptr;

    return true;
}

ENG_API string OvVR::getTrackingSysName() {
    unsigned int len = vrSys->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String, nullptr, 0, nullptr);
    if (len == 0) return string();

    string result;
    result.resize(len);
    vrSys->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String, &result[0], len, nullptr);
    return result;
}

ENG_API string OvVR::getManufacturerName() {
    unsigned int len = vrSys->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_ManufacturerName_String, nullptr, 0, nullptr);
    if (len == 0) return string();

    string result;
    result.resize(len);
    vrSys->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_ManufacturerName_String, &result[0], len, nullptr);
    return result;
}

ENG_API string OvVR::getModelNumber() {
    unsigned int len = vrSys->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_ModelNumber_String, nullptr, 0, nullptr);
    if (len == 0) return string();

    string result;
    result.resize(len);
    vrSys->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_ModelNumber_String, &result[0], len, nullptr);
    return result;
}

ENG_API bool OvVR::printRenderModels() {
    for (unsigned int i = 0; i < vrModels->GetRenderModelCount(); i++) {
        char buffer[256];
        vrModels->GetRenderModelName(i, buffer, 256);
        cout << "   " << i << ") " << buffer << " model" << endl;

        for (unsigned int j = 0; j < vrModels->GetComponentCount(buffer); j++) {
            char cbuffer[256];
            vrModels->GetComponentName(buffer, j, cbuffer, 256);
            cout << "     " << j << ") " << cbuffer << endl;
        }
    }
    return true;
}

ENG_API unsigned int OvVR::getHmdIdealHorizRes() {
    unsigned int w, h;
    vrSys->GetRecommendedRenderTargetSize(&w, &h);
    return w;
}

ENG_API unsigned int OvVR::getHmdIdealVertRes() {
    unsigned int w, h;
    vrSys->GetRecommendedRenderTargetSize(&w, &h);
    return h;
}

ENG_API glm::mat4 OvVR::ovr2ogl(const vr::HmdMatrix34_t& m) {
    return glm::mat4(
        m.m[0][0], m.m[1][0], m.m[2][0], 0.0f,
        m.m[0][1], m.m[1][1], m.m[2][1], 0.0f,
        m.m[0][2], m.m[1][2], m.m[2][2], 0.0f,
        m.m[0][3], m.m[1][3], m.m[2][3], 1.0f
    );
}

ENG_API glm::mat4 OvVR::ovr2ogl(const vr::HmdMatrix44_t& m) {
    return glm::mat4(
        m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
        m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
        m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
        m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]
    );
}

ENG_API glm::mat4 OvVR::getProjMatrix(OvEye eye, float nearPlane, float farPlane) {
    switch (eye) {
    case EYE_LEFT: return ovr2ogl(vrSys->GetProjectionMatrix(vr::Eye_Left, nearPlane, farPlane));
    case EYE_RIGHT: return ovr2ogl(vrSys->GetProjectionMatrix(vr::Eye_Right, nearPlane, farPlane));
    default: return glm::mat4(1.0f);
    }
}

ENG_API glm::mat4 OvVR::getEye2HeadMatrix(OvEye eye) {
    switch (eye) {
    case EYE_LEFT: return ovr2ogl(vrSys->GetEyeToHeadTransform(vr::Eye_Left));
    case EYE_RIGHT: return ovr2ogl(vrSys->GetEyeToHeadTransform(vr::Eye_Right));
    default: return glm::mat4(1.0f);
    }
}

ENG_API glm::mat4 OvVR::getModelviewMatrix() {
    if (!vrPoses[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
        return glm::mat4(1.0f);
    return ovr2ogl(vrPoses[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking);
}

ENG_API bool OvVR::update() {
    vrComp->WaitGetPoses(vrPoses, vr::k_unMaxTrackedDeviceCount, nullptr, 0);
    for (auto c : controllers)
        if (vrPoses[c->id].bPoseIsValid)
            c->matrix = ovr2ogl(vrPoses[c->id].mDeviceToAbsoluteTracking);
    return true;
}

ENG_API unsigned int OvVR::getNrOfControllers() {
    return (unsigned int)controllers.size();
}

ENG_API OvVR::Controller* OvVR::getController(unsigned int pos) const {
    if (pos >= controllers.size()) return nullptr;
    return controllers.at(pos);
}

ENG_API void OvVR::setReprojection(bool flag) {
    vrComp->ForceInterleavedReprojectionOn(flag);
}

ENG_API void OvVR::pass(OvEye eye, unsigned int tex) {
    const vr::Texture_t t = { reinterpret_cast<void*>(uintptr_t(tex)), vr::TextureType_OpenGL, vr::ColorSpace_Linear };
    switch (eye) {
    case EYE_LEFT: vrComp->Submit(vr::Eye_Left, &t); break;
    case EYE_RIGHT: vrComp->Submit(vr::Eye_Right, &t); break;
    }
}

ENG_API void OvVR::render() {
    vrComp->PostPresentHandoff();
}

// --- Controller Methods ---

OvVR::Controller::Controller() : id(0), matrix(glm::mat4(1.0f)) {}

OvVR::Controller::~Controller() {}

glm::mat4 OvVR::Controller::getMatrix() {
    return matrix;
}

glm::vec2 OvVR::Controller::getAxis(unsigned int axisId, unsigned long long int& bPressed, unsigned long long int& bTouched) {
    if (axisId >= vr::k_unControllerStateAxisCount)
        return glm::vec2(0.0f);

    bPressed = pControllerState.ulButtonPressed;
    bTouched = pControllerState.ulButtonTouched;

    return glm::vec2(pControllerState.rAxis[axisId].x, pControllerState.rAxis[axisId].y);
}

bool OvVR::Controller::isButtonPressed(unsigned long long int id, unsigned long long int mask) {
    return (mask & (1ull << id)) != 0;
}

#pragma once

#include "test.h"
#include "camera.h"

class MockCamera : public Camera {
public:
    MockCamera(const std::string& name = "DefaultMainCamera",
        const float& width = 100.0f,
        const float& height = 100.0,
        const float& near = 0.1f,
        const float& far = 100.0f)
        : Camera(name, width, height, near, far) 
    {
        
    }

    MOCK_METHOD(Node*, clone, (), (const, override));
    MOCK_METHOD(const bool&, isMainCamera, (), (const, override));
    MOCK_METHOD(void, loadProjectionMatrix, (), (override));
    MOCK_METHOD(void, render, (const glm::mat4&), (override));
    MOCK_METHOD(void, onWindowReshape, (int, int), (override));
    MOCK_METHOD(const glm::mat4, getFinalMatrix, (), (const, override));
    MOCK_METHOD(const std::vector<Node*>, getChildren, (), (const, override));
};
#pragma once

#include "test.h"
#include "node.h"

class MockNode : public Node {
public:
    MockNode(const std::string& name = "defaultNode") : Node(name) {}

    MOCK_METHOD(const glm::mat4, getFinalMatrix, (), (const, override));
    MOCK_METHOD(const std::vector<Node*>, getChildren, (), (const, override));
    MOCK_METHOD(void, render, (const glm::mat4&), (override));
};

#pragma once

#include "test.h"
#include "light.h"

class MockLight : public Light {
public:
	MockLight(const std::string& name = "defaultLight", const glm::vec4& position = glm::vec4(1.0f)) : Light(name, position) {}

	MOCK_METHOD(Node*, clone, (), (const, override));
	MOCK_METHOD(void, render, (const glm::mat4&), (override));
	MOCK_METHOD(const glm::mat4, getFinalMatrix, (), (const, override));
	MOCK_METHOD(const std::vector<Node*>, getChildren, (), (const, override));
};
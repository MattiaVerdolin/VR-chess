#pragma once

#include "test.h"
#include "mockNode.h"
#include "mockCamera.h"
#include "mockLight.h"
#include "list.h"

class ListTest : public ::testing::Test {
protected:
	static List list;
	static MockNode* root;
	static MockNode* sun;
	static MockNode* earth;
	static MockNode* mars;
	static MockNode* moon;
	static MockCamera* camera;
	static MockLight* omni;

	static void SetUpTestSuite();

	static void TearDownTestSuite();
};

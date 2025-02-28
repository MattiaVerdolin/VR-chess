#pragma once

#include "test.h"
#include "orthoCamera.h"

class OrthoCameraTest : public ::testing::Test {
protected:
	static OrthoCamera* orthoCamera;

	static void SetUpTestSuite();

	static void TearDownTestSuite();
};
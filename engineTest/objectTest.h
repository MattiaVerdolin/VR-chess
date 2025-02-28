#pragma once

#include "test.h"

class ObjectTest : public ::testing::Test {
protected:
	static Object* object1;
	static Object* object2;

	static void SetUpTestSuite();

	static void TearDownTestSuite();
};


#include "objectTest.h"

Object* ObjectTest::object1 = nullptr;
Object* ObjectTest::object2 = nullptr;

void ObjectTest::SetUpTestSuite() {
	object1 = new Node("Object1");
	object2 = new Node("Object2");
}

void ObjectTest::TearDownTestSuite() {
	delete object1;
	delete object2;
}

TEST_F(ObjectTest, Constructor) {
	unsigned int idFirstObject = object1->getId();
	EXPECT_NE(object2->getId(), idFirstObject);
}

TEST_F(ObjectTest, GetName) {
	EXPECT_EQ(object1->getName(), "Object1");
	EXPECT_EQ(object2->getName(), "Object2");
}

TEST_F(ObjectTest, SetName) {
	object2->setName("UpdatedObject2");
	EXPECT_EQ(object2->getName(), "UpdatedObject2");
}
#include "listTest.h"

List ListTest::list;
MockNode* ListTest::root = nullptr;
MockNode* ListTest::sun = nullptr;
MockNode* ListTest::earth = nullptr;
MockNode* ListTest::mars = nullptr;
MockNode* ListTest::moon = nullptr;
MockCamera* ListTest::camera = nullptr;
MockLight* ListTest::omni = nullptr;

void ListTest::SetUpTestSuite() {
	root = new MockNode();
	sun = new MockNode();
	earth = new MockNode();
	mars = new MockNode();
	moon = new MockNode();

	camera = new MockCamera();

	omni = new MockLight();
}

void ListTest::TearDownTestSuite() {
	delete root;
	delete sun;
	delete earth;
	delete mars;
	delete moon;

	delete camera;

	delete omni;
}

TEST_F(ListTest, Constructor) {
	EXPECT_EQ(list.getNumberOfElementsInList(), 0);
}

TEST_F(ListTest, Pass) {
	glm::mat4 sunFinalMatrix = glm::mat4(2.0f);
	glm::mat4 omniFinalMatrix = glm::mat4(3.0f);
	glm::mat4 earthFinalMatrix = glm::mat4(4.0f);
	glm::mat4 marsFinalMatrix = glm::mat4(5.0f);
	glm::mat4 moonFinalMatrix = glm::mat4(6.0f);

	std::vector<Node*> rootGetChildren;
	rootGetChildren.push_back(sun);

	std::vector<Node*> sunGetChildren;
	sunGetChildren.push_back(omni);
	sunGetChildren.push_back(earth);
	sunGetChildren.push_back(mars);

	std::vector<Node*> earthGetChildren;
	earthGetChildren.push_back(camera);
	earthGetChildren.push_back(moon);


	EXPECT_CALL(*root, getChildren)
		.Times(1)
		.WillOnce(::testing::Return(rootGetChildren));

	EXPECT_CALL(*sun, getFinalMatrix)
		.Times(1)
		.WillOnce(::testing::Return(sunFinalMatrix));

	EXPECT_CALL(*sun, getChildren)
		.Times(1)
		.WillOnce(::testing::Return(sunGetChildren));

	EXPECT_CALL(*omni, getFinalMatrix)
		.Times(1)
		.WillOnce(::testing::Return(omniFinalMatrix));

	EXPECT_CALL(*earth, getFinalMatrix)
		.Times(1)
		.WillOnce(::testing::Return(earthFinalMatrix));

	EXPECT_CALL(*mars, getFinalMatrix)
		.Times(1)
		.WillOnce(::testing::Return(marsFinalMatrix));

	EXPECT_CALL(*omni, getChildren)
		.Times(1)
		.WillOnce(::testing::Return(std::vector<Node*>()));

	EXPECT_CALL(*earth, getChildren)
		.Times(1)
		.WillOnce(::testing::Return(earthGetChildren));

	EXPECT_CALL(*camera, getFinalMatrix)
		.Times(0);

	EXPECT_CALL(*moon, getFinalMatrix)
		.Times(1)
		.WillOnce(::testing::Return(moonFinalMatrix));

	EXPECT_CALL(*camera, getChildren)
		.Times(1)
		.WillOnce(::testing::Return(std::vector<Node*>()));

	EXPECT_CALL(*moon, getChildren)
		.Times(1)
		.WillOnce(::testing::Return(std::vector<Node*>()));

	EXPECT_CALL(*mars, getChildren)
		.Times(1)
		.WillOnce(::testing::Return(std::vector<Node*>()));

	list.pass(root);

	EXPECT_EQ(list.getNumberOfElementsInList(), 5);
}


TEST_F(ListTest, RenderElements) {
	::testing::InSequence seq;

	EXPECT_CALL(*omni, render)
		.Times(1);
	EXPECT_CALL(*sun, render)
		.Times(1);
	EXPECT_CALL(*earth, render)
		.Times(1);
	EXPECT_CALL(*moon, render)
		.Times(1);
	EXPECT_CALL(*mars, render)
		.Times(1);

	EXPECT_CALL(*root, render)
		.Times(0);
	EXPECT_CALL(*camera, render)
		.Times(0);

	list.renderElements(glm::mat4(1.0f));
}

TEST_F(ListTest, ClearList) {
	EXPECT_EQ(list.getNumberOfElementsInList(), 5);
	list.clearList();
	EXPECT_EQ(list.getNumberOfElementsInList(), 0);
}
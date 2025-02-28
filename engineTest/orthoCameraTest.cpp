#include "orthoCameraTest.h"

OrthoCamera* OrthoCameraTest::orthoCamera = nullptr;

void OrthoCameraTest::SetUpTestSuite() {
	orthoCamera = new OrthoCamera("orthoCamera", 1000.0f, 1000.0f, 1.0f, 100.0f);
}

void OrthoCameraTest::TearDownTestSuite() {
	delete orthoCamera;
}

TEST_F(OrthoCameraTest, Constructor) {
	EXPECT_EQ(orthoCamera->getInverseCameraFinalMatrix(), glm::mat4(1.0f));
	EXPECT_EQ(orthoCamera->isMainCamera(), false);
}

TEST_F(OrthoCameraTest, SetMainCamera) {
	orthoCamera->setMainCamera(true);
	EXPECT_EQ(orthoCamera->isMainCamera(), true);
}

TEST_F(OrthoCameraTest, GetInverseCameraFinalMatrix) {
	glm::mat4 rotate_camera = glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 translate_camera = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, 0.0f));
	glm::mat4 camera_matrix = translate_camera * rotate_camera;
	orthoCamera->setMatrix(camera_matrix);

	EXPECT_EQ(orthoCamera->getInverseCameraFinalMatrix(), glm::inverse(camera_matrix));


	glm::mat4 scale_root = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
	glm::mat4 translate_root = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));
	glm::mat4 root_matrix = translate_root * scale_root;
	Node* nodeRoot = new Node("tempRootNode");
	nodeRoot->setMatrix(root_matrix);

	nodeRoot->addChild(orthoCamera);
	EXPECT_EQ(orthoCamera->getInverseCameraFinalMatrix(), glm::inverse(root_matrix * camera_matrix));
	
	nodeRoot->removeChild(orthoCamera);
	delete nodeRoot;
}
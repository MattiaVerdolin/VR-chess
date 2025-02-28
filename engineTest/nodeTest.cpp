#include "nodeTest.h"

Node* NodeTest::node_root = nullptr;
Node* NodeTest::node_middle = nullptr;
Node* NodeTest::node_leaf = nullptr;

void NodeTest::SetUpTestSuite() {
	node_root = new Node("root");
	node_middle = new Node("middle");
	node_leaf = new Node("leaf");
}

void NodeTest::TearDownTestSuite() {
	delete node_root;
}

TEST_F(NodeTest, Constructor) {
	EXPECT_EQ(node_root->getMatrix(), glm::mat4(1.0f));
	EXPECT_EQ(node_root->getParent(), nullptr);
	EXPECT_EQ(node_root->getMaterial(), nullptr);
	EXPECT_TRUE(node_root->getChildren().empty());
}

TEST_F(NodeTest, AddChild) {
	EXPECT_FALSE(node_root->addChild(nullptr));
	EXPECT_EQ(node_root->getNumberOfChildren(), 0);

	EXPECT_TRUE(node_root->addChild(node_middle));
	EXPECT_EQ(node_root->getNumberOfChildren(), 1);
	EXPECT_EQ(node_middle->getParent(), node_root);

	EXPECT_FALSE(node_root->addChild(node_middle));
	EXPECT_EQ(node_root->getNumberOfChildren(), 1);

	node_middle->addChild(node_leaf);
}

TEST_F(NodeTest, GetChildren) {
	auto children = node_root->getChildren();

	ASSERT_EQ(children.size(), 1);
	EXPECT_EQ(children[0], node_middle);
	EXPECT_EQ(children[0]->getId(), node_middle->getId());
	EXPECT_EQ(children[0]->getName(), node_middle->getName());
}

TEST_F(NodeTest, RemoveChild) {
	EXPECT_FALSE(node_middle->removeChild(nullptr));
	EXPECT_EQ(node_middle->getNumberOfChildren(), 1);

	EXPECT_FALSE(node_leaf->removeChild(node_root));
	EXPECT_EQ(node_leaf->getNumberOfChildren(), 0);

	EXPECT_TRUE(node_middle->removeChild(node_leaf));
	EXPECT_EQ(node_middle->getNumberOfChildren(), 0);
	EXPECT_EQ(node_leaf->getParent(), nullptr);

	node_middle->addChild(node_leaf);
	EXPECT_TRUE(node_root->removeChild(node_middle));
	EXPECT_EQ(node_root->getNumberOfChildren(), 0);
	EXPECT_EQ(node_middle->getParent(), nullptr);
	EXPECT_EQ(node_leaf->getParent(), node_middle);

	node_root->addChild(node_middle);
}

TEST_F(NodeTest, RemoveChildByPosition) {
	EXPECT_EQ(node_root->removeChildByPosition(1), nullptr);
	EXPECT_EQ(node_root->getNumberOfChildren(), 1);

	EXPECT_EQ(node_middle->removeChildByPosition(0), node_leaf);
	EXPECT_EQ(node_middle->getNumberOfChildren(), 0);
	EXPECT_EQ(node_leaf->getParent(), nullptr);

	node_middle->addChild(node_leaf);
	EXPECT_EQ(node_root->removeChildByPosition(0), node_middle);
	EXPECT_EQ(node_root->getNumberOfChildren(), 0);
	EXPECT_EQ(node_middle->getParent(), nullptr);
	EXPECT_EQ(node_leaf->getParent(), node_middle);

	node_root->addChild(node_middle);
}

TEST_F(NodeTest, GetParent) {
	EXPECT_EQ(node_root->getParent(), nullptr);
	EXPECT_EQ(node_middle->getParent(), node_root);
}

TEST_F(NodeTest, GetNumberOfChildren) {
	EXPECT_EQ(node_root->getNumberOfChildren(), 1);
	EXPECT_EQ(node_leaf->getNumberOfChildren(), 0);
}

TEST_F(NodeTest, SetAndGetMaterial) {
	Material* material = new Material("material");
	node_root->setMaterial(material);
	EXPECT_EQ(node_root->getMaterial(), material);
	node_root->setMaterial(nullptr);
	EXPECT_EQ(node_root->getMaterial(), nullptr);
	delete material;
}

TEST_F(NodeTest, SetAndGetMatrix) {
	node_root->setMatrix(glm::mat4(2.0f));
	EXPECT_EQ(node_root->getMatrix(), glm::mat4(2.0f));
}

TEST_F(NodeTest, GetFinalMatrix) {
	glm::mat4 scale_root = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
	glm::mat4 translate_root = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));
	glm::mat4 root_matrix = translate_root * scale_root;
	node_root->setMatrix(root_matrix);

	glm::mat4 rotate_middle = glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 translate_middle = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, 0.0f));
	glm::mat4 middle_matrix = translate_middle * rotate_middle;
	node_middle->setMatrix(middle_matrix);

	glm::mat4 scale_leaf = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	glm::mat4 rotate_leaf = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 translate_leaf = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 1.0f));
	glm::mat4 leaf_matrix = translate_leaf * rotate_leaf * scale_leaf;
	node_leaf->setMatrix(leaf_matrix);

	glm::mat4 expected_root_matrix = root_matrix;
	glm::mat4 expected_middle_matrix = root_matrix * middle_matrix;
	glm::mat4 expected_leaf_matrix = expected_middle_matrix * leaf_matrix;

	EXPECT_EQ(node_root->getFinalMatrix(), expected_root_matrix);
	EXPECT_EQ(node_middle->getFinalMatrix(), expected_middle_matrix);
	EXPECT_EQ(node_leaf->getFinalMatrix(), expected_leaf_matrix);
}

TEST_F(NodeTest, FindNodeByName) {
	EXPECT_EQ(node_root->findNodeByName("ThisNodeDoesNotExist"), nullptr);
	EXPECT_EQ(node_root->findNodeByName("middle"), node_middle);
	EXPECT_EQ(node_root->findNodeByName("leaf"), node_leaf);
}

TEST_F(NodeTest, GetMainCamera) {
	MockCamera mockCamera1;
	MockCamera mockCamera2;
	const bool expectedValueFalse = false;
	const bool expectedValueTrue = true;


	EXPECT_EQ(node_root->getMainCamera(), nullptr);


	node_leaf->addChild(&mockCamera1);
	EXPECT_CALL(mockCamera1, isMainCamera())
		.Times(1)
		.WillOnce(testing::ReturnRef(expectedValueFalse));
	EXPECT_EQ(node_root->getMainCamera(), nullptr);


	EXPECT_CALL(mockCamera1, isMainCamera())
		.Times(1)
		.WillOnce(testing::ReturnRef(expectedValueTrue));
	EXPECT_EQ(node_root->getMainCamera(), &mockCamera1);


	node_middle->addChild(&mockCamera2);
	EXPECT_CALL(mockCamera1, isMainCamera())
		.Times(1)
		.WillOnce(testing::ReturnRef(expectedValueTrue));
	EXPECT_CALL(mockCamera2, isMainCamera())
		.Times(0);
	EXPECT_EQ(node_root->getMainCamera(), &mockCamera1);


	node_middle->addChild(&mockCamera2);
	EXPECT_CALL(mockCamera1, isMainCamera())
		.Times(1)
		.WillOnce(testing::ReturnRef(expectedValueFalse));
	EXPECT_CALL(mockCamera2, isMainCamera())
		.Times(1)
		.WillOnce(testing::ReturnRef(expectedValueTrue));
	EXPECT_EQ(node_root->getMainCamera(), &mockCamera2);

	node_leaf->removeChild(&mockCamera1);
	node_middle->removeChild(&mockCamera2);
}

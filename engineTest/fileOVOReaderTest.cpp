#include "fileOVOReaderTest.h"

TEST(FileOVOReaderTest, GetInstance) {
	FileOVOReader& readerOVO1 = FileOVOReader::getInstance();
	FileOVOReader& readerOVO2 = FileOVOReader::getInstance();
	EXPECT_EQ(&readerOVO1, &readerOVO2);
}

TEST(FileOVOReaderTest, HasOVOExtension) {
	FileOVOReader& readerOVO = FileOVOReader::getInstance();
	EXPECT_FALSE(readerOVO.hasOVOExtension("fileIsNotOVO.txt"));
	EXPECT_TRUE(readerOVO.hasOVOExtension("fileIsOVO.ovo"));
}

TEST(FileOVOReaderTest, Parse) {
	FileOVOReader& readerOVO = FileOVOReader::getInstance();
	EXPECT_EQ(readerOVO.parseFile("thisFileDoesNotExist.ovo"), nullptr);

	Node* rootTestingNode = readerOVO.parseFile("testing.ovo");

	EXPECT_NE(rootTestingNode, nullptr);

	EXPECT_EQ(rootTestingNode->getNumberOfChildren(), 1);

	Mesh* floor = dynamic_cast<Mesh*>(const_cast<Node*>(rootTestingNode->findNodeByName("Floor")));
	EXPECT_NE(floor, nullptr);
	EXPECT_EQ(floor->getNumberOfChildren(), 3);

	Mesh* box = dynamic_cast<Mesh*>(const_cast<Node*>(floor->findNodeByName("Box")));
	EXPECT_NE(box, nullptr);
	EXPECT_EQ(box->getNumberOfChildren(), 0);

	OmniLight* omni = dynamic_cast<OmniLight*>(const_cast<Node*>(floor->findNodeByName("Omni001")));
	EXPECT_NE(omni, nullptr);
	EXPECT_EQ(omni->getNumberOfChildren(), 0);

	SpotLight* spot = dynamic_cast<SpotLight*>(const_cast<Node*>(floor->findNodeByName("Fspot001")));
	EXPECT_NE(spot, nullptr);
	EXPECT_EQ(spot->getNumberOfChildren(), 0);
}
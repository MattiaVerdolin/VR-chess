#include "spotLightTest.h"

TEST(SpotLightTest, Constructor){
    SpotLight* spot = new SpotLight();

    EXPECT_EQ(spot->getPosition(), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    EXPECT_EQ(spot->getDirection(), glm::vec3(1.0f));
    EXPECT_EQ(spot->getCutoff(), 45.0f);

    SpotLight* spot2 = new SpotLight("name", glm::vec3(1.0f), glm::vec3(1.0f), 91.0f);
    EXPECT_EQ(spot2->getCutoff(), 45.0f);


    SpotLight* spot3 = new SpotLight("name", glm::vec3(1.0f), glm::vec3(1.0f), 90.0f);
    EXPECT_EQ(spot3->getCutoff(), 90.0f);

    SpotLight* spot4 = new SpotLight("name", glm::vec3(1.0f), glm::vec3(1.0f), -1.0f);
    EXPECT_EQ(spot4->getCutoff(), 45.0f);

    delete spot;
    delete spot2;
    delete spot3;
    delete spot4;
}

TEST(SpotLightTest, SetPosition){
    SpotLight* spot = new SpotLight();

    spot->setPosition(glm::vec3(2.0f, 3.0f, 4.0f));
    EXPECT_EQ(spot->getPosition().w , 1.0f);

    delete spot;
}

TEST(SpotLightTest, SetCutoff){
    SpotLight* spot = new SpotLight();

    spot->setCutoff(91.0f);
    EXPECT_EQ(spot->getCutoff(), 45.0f);

    spot->setCutoff(90.0f);
    EXPECT_EQ(spot->getCutoff(), 90.0f);

    spot->setCutoff(-1.0f);
    EXPECT_EQ(spot->getCutoff(), 90.0f);

    delete spot;
}
#include "omniLightTest.h"

TEST(OmniLightTest, Costructor){
    OmniLight* omni = new OmniLight();

    EXPECT_EQ(omni->getPosition().w, 1.0f);
    EXPECT_EQ(omni->getCutoff(), 180.0f);

    delete omni;
}

TEST(OmniLightTest, SetPosition){
    OmniLight* omni = new OmniLight();
    omni->setPosition(glm::vec3(4.0f,5.0f,6.0f));
    EXPECT_EQ(omni->getPosition().w, 1.0f);
    EXPECT_EQ(omni->getCutoff(), 180.0f);

    delete omni;
}
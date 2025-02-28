#include "directLightTest.h"

TEST(DirectLightTest, Constructor){
    DirectLight* direct = new DirectLight();

    EXPECT_EQ(direct->getPosition(), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

    delete direct;
}

TEST(DirectLightTest, SetPosition){
    DirectLight* direct = new DirectLight();

    glm::vec3 newPos{glm::vec3(4.0f, 5.0f, 2.0f)};
    direct->setPosition(newPos);

    EXPECT_EQ(direct->getPosition().x, 4.0f);
    EXPECT_EQ(direct->getPosition().y, 5.0f);
    EXPECT_EQ(direct->getPosition().z, 2.0f);
    EXPECT_EQ(direct->getPosition().w, 0.0f);

    delete direct;
}
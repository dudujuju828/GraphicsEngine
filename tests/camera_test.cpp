// camera_test.cpp
/*
AI GENERATED TEST
*/
#include <gtest/gtest.h>
#include <glm/glm.hpp>

#include "camera.hpp"

// 1) Basic sanity check: view matrix should put the camera at the origin in view space.
TEST(CameraTest, ViewMatrixPutsCameraAtOrigin) {
    Camera cam;  // uses your default constructor

    glm::mat4 view = cam.getViewMatrix();

    // Transform the camera position into view space.
    glm::vec4 camPosWS(cam.getPosition(), 1.0f);
    glm::vec4 camPosVS = view * camPosWS;

    // In view space, the camera should sit at the origin.
    EXPECT_NEAR(camPosVS.x, 0.0f, 1e-4f);
    EXPECT_NEAR(camPosVS.y, 0.0f, 1e-4f);
    EXPECT_NEAR(camPosVS.z, 0.0f, 1e-4f);

    // Optional: check that the world origin ends up in front of the camera (negative z).
    glm::vec4 originWS(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 originVS = view * originWS;
    EXPECT_LT(originVS.z, 0.0f);
}

// 2) Moving forward should move the camera along its viewing direction,
//    and the distance should match movementSpeed * deltaTime (15 * dt in your header).
TEST(CameraTest, ForwardMovementChangesPositionAlongViewDirection) {
    Camera cam;  // default position is (0, 0, 3) in your header

    glm::vec3 start = cam.getPosition();

    float dt = 1.0f;  // pretend one second has passed
    cam.processKeyboard(CameraMovement::Forward, dt);

    glm::vec3 end = cam.getPosition();
    glm::vec3 delta = end - start;

    // It should have moved somewhere.
    EXPECT_GT(glm::length(delta), 0.0f);

    // For the default yaw/pitch, movement should be purely along -Z.
    EXPECT_NEAR(end.x, start.x, 1e-4f);
    EXPECT_NEAR(end.y, start.y, 1e-4f);

    // movementSpeed is 15.0f in the class, so |delta| should be ~15.
    EXPECT_NEAR(glm::length(delta), 15.0f, 1e-3f);
}

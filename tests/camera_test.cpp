// camera_test.cpp
#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"

// 1) Check that yaw/pitch produce a sensible orientation and an orthonormal basis.
TEST(CameraTest, OrientationFromYawPitch) {
    Camera cam(
        glm::vec3(0.0f, 0.0f, 0.0f),   // starts at origin
        glm::vec3(0.0f, 1.0f, 0.0f),   // world up
        0.0f,                          // yaw
        0.0f,                          // pitch
        45.0f,                         // fov
        0.1f,                          // near
        100.0f                         // far
    );

    // With yaw=0, pitch=0, this implementation should look roughly along +X.
    EXPECT_NEAR(cam.front.x, 1.0f, 1e-4f);
    EXPECT_NEAR(cam.front.y, 0.0f, 1e-4f);
    EXPECT_NEAR(cam.front.z, 0.0f, 1e-4f);

    // The camera basis vectors should be unit length.
    EXPECT_NEAR(glm::length(cam.front), 1.0f, 1e-4f);
    EXPECT_NEAR(glm::length(cam.right), 1.0f, 1e-4f);
    EXPECT_NEAR(glm::length(cam.up),    1.0f, 1e-4f);

    // And they should all be perpendicular to each other.
    EXPECT_NEAR(glm::dot(cam.front, cam.right), 0.0f, 1e-4f);
    EXPECT_NEAR(glm::dot(cam.front, cam.up),    0.0f, 1e-4f);
    EXPECT_NEAR(glm::dot(cam.right, cam.up),    0.0f, 1e-4f);
}

// 2) Check that "move forward" actually moves along the front vector.
TEST(CameraTest, MovesForwardAlongFront) {
    Camera cam(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        0.0f, 0.0f,
        45.0f,
        0.1f, 100.0f
    );

    glm::vec3 start = cam.position;

    float dt = 1.0f;  // pretend one second passed
    cam.processKeyboard(CameraMovement::Forward, dt);

    glm::vec3 delta = cam.position - start;

    // It should have moved somewhere.
    EXPECT_GT(glm::length(delta), 0.0f);

    // The movement direction should line up with the current front vector.
    float cosAngle = glm::dot(glm::normalize(delta), glm::normalize(cam.front));
    EXPECT_NEAR(cosAngle, 1.0f, 1e-3f);
}

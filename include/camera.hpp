#ifndef SRC_CAMERA_HPP
#define SRC_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement {
    Forward,
    Backward,
    Left,
    Right
};

class Camera {
public:
    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw          = -90.0f,
        float pitch        = 0.0f,
        float fov          = 45.0f,
        float nearPlane    = 0.1f,
        float farPlane     = 100.0f
    );

    // Call when the window is created/resized
    void setAspectRatio(float aspect);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void processKeyboard(CameraMovement dir, float deltaTime);
    void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);

    const glm::vec3& getPosition() const { return position; }
    float getFov() const { return fov; }

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float fov;
    float nearPlane;
    float farPlane;
    float aspectRatio;

    float movementSpeed   = 5.0f;
    float mouseSensitivity = 0.1f;

    void updateCameraVectors();
};

#endif

#include "../include/camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch,
               float fov, float nearPlane, float farPlane)
    : position(position),
      worldUp(up),
      yaw(yaw),
      pitch(pitch),
      fov(fov),
      nearPlane(nearPlane),
      farPlane(farPlane),
      aspectRatio(16.0f / 9.0f)
{
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    updateCameraVectors();
}

void Camera::setAspectRatio(float aspect) {
    aspectRatio = aspect;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::processKeyboard(CameraMovement dir, float deltaTime) {
    float velocity = movementSpeed * deltaTime;

    if (dir == CameraMovement::Forward)
        position += front * velocity;
    if (dir == CameraMovement::Backward)
        position -= front * velocity;
    if (dir == CameraMovement::Left)
        position -= right * velocity;
    if (dir == CameraMovement::Right)
        position += right * velocity;
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw   += xOffset;
    pitch += yOffset;

    if (constrainPitch) {
        if (pitch > 89.0f)  pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 f;
    f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    f.y = sin(glm::radians(pitch));
    f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(f);

    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
}


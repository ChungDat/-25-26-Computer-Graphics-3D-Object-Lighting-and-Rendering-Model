#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../constants.h"

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera(glm::vec3, float, float);
    Camera(float, float, float, float, float);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();
    // processes input received from any keyboard-like input system. 
    // Accepts input parameter in the form of camera defined ENUM 
    // (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement, float);
    // processes input received from a mouse input system. 
    // Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float, float, GLboolean);
    // processes input received from a mouse scroll-wheel event. 
    // Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float);
    virtual ~Camera();
private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void UpdateCameraVectors();
};


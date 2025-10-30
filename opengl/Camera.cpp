#include "Camera.h"

Camera::Camera(glm::vec3 position, float yaw = YAW, float pitch = PITCH) :
	WorldUp(glm::vec3(0.0, 1.0, 0.0)), Front(glm::vec3(0.0, 0.0, -1.0)), 
	MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float yaw, float pitch) :
	WorldUp(glm::vec3(0.0, 1.0, 0.0)), Front(glm::vec3(0.0, 0.0, -1.0)),
	MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) 
{
	Position = glm::vec3(posX, posY, posZ);
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD) Position += Front * velocity;
	if (direction == BACKWARD) Position -= Front * velocity;
	if (direction == RIGHT) Position += Right * velocity;
	if (direction == LEFT) Position -= Right * velocity;
	if (direction == UP) Position += WorldUp * velocity;
	if (direction == DOWN) Position -= WorldUp * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
	Zoom -= (float)yoffset;
	if (Zoom < 5.0f)
		Zoom = 5.0f;
	if (Zoom > 70.0f)
		Zoom = 70.0f;
}

void Camera::UpdateCameraVectors() {
	// calculate new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

	Front = glm::normalize(front);

	// re - calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}

Camera::~Camera() {}
#include "Camera.h"

Camera::Camera()
{

}

Camera::Camera( glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed )
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3( 0.0f, 0.0f, -1.0f );

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	Update();
}

Camera::~Camera()
{

}

void Camera::KeyControl( bool* keys, GLfloat deltaTime )
{
	GLfloat velocity = moveSpeed * deltaTime;

	if( keys[GLFW_KEY_W] )
	{
		position += front * velocity;
	}

	if( keys[GLFW_KEY_S] )
	{
		position -= front * velocity;
	}

	if( keys[GLFW_KEY_A] )
	{
		position -= right * velocity;
	}

	if( keys[GLFW_KEY_D] )
	{
		position += right * velocity;
	}
}

void Camera::MouseControl( GLfloat xChange, GLfloat yChange )
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if( pitch > 89.0f )
	{
		pitch = 89.0f;
	}

	if( pitch < -89.0f )
	{
		pitch = -89.0f;
	}

	Update();
}

glm::vec3 Camera::GetCameraPosition()
{
	return position;
}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt( position, position + front, up );
}

void Camera::Update()
{
	front.x = cos( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
	front.y = sin( glm::radians( pitch ) );
	front.z = sin( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
	// todo test and remove this normalize to see if there's a difference
	front = glm::normalize( front );

	// cross product finds what the right angle is to two other angles  
	right = glm::normalize( glm::cross( front, worldUp ) );
	up = glm::normalize( glm::cross( right, front ) );
}
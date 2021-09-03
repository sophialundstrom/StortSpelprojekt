#include "Camera.h"

#include "Settings.h"
#include "Time.h"
#include "Event.h"

#include <algorithm>

Camera::Camera() {}

Camera::Camera(float FOV, float aspectRatio, float nearZ, float farZ, float rotationSpeed, float moveSpeed, Vector3 position, Vector3 forward, Vector3 up)
	:position(position), forward(forward), direction(forward), up(up), moveSpeed(moveSpeed), rotationSpeed(rotationSpeed), pitch(0), yaw(0)
{
	this->viewMatrix = Matrix::CreateLookAt(position, forward, up);
	this->perspectiveMatrix = Matrix::CreatePerspectiveFieldOfView(FOV, aspectRatio, nearZ, farZ);
}

void Camera::MoveRight(int sign)
{
	Vector3 forwardVec;
	Vector3::Transform(forward, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f), forwardVec);

	Vector3 rightVec = up.Cross(forwardVec);
	rightVec.Normalize();

	position += rightVec * Time::GetDelta() * moveSpeed * (float)sign;
}

void Camera::MoveForward(int sign)
{
	Vector3 forwardVec;
	Vector3::Transform(forward, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f), forwardVec);
	forwardVec.Normalize();

	position += forwardVec * Time::GetDelta() * moveSpeed * (float)sign;
}

void Camera::Rotate(float dx, float dy)
{
	//YAW
	yaw = (yaw + dx * Time::GetDelta() * rotationSpeed);

	if (yaw < 0.0f)
		yaw += PI * 2.0f;
	if (yaw > PI * 2.0f)
		yaw -= PI * 2.0f;

	//PITCH
	pitch = std::clamp(pitch + dy * Time::GetDelta() * rotationSpeed, 0.995f * -PI_DIV2, 0.995f * PI_DIV2);
}

void Camera::Update()
{
	if (Event::KeyIsPressed('Q'))
		Rotate(0, 1);

	if (Event::KeyIsPressed('E'))
		Rotate(0, -1);

	if (Event::KeyIsPressed('W'))
		MoveForward();

	if (Event::KeyIsPressed('A'))
		MoveRight(-1);

	if (Event::KeyIsPressed('S'))
		MoveForward(-1);

	if (Event::KeyIsPressed('D'))
		MoveRight();

	if (Event::KeyIsPressed(32)) //SPACE
		position.y += moveSpeed * Time::GetDelta();

	if (Event::KeyIsPressed(16)) //SHIFT
		position.y -= moveSpeed * Time::GetDelta();

	Vector3 lookAt;
	Vector3::Transform(forward, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f), lookAt);
	lookAt.Normalize();

	direction = lookAt;

	const Vector3 target = position + lookAt;

	viewMatrix = Matrix::CreateLookAt(position, target, up);
}
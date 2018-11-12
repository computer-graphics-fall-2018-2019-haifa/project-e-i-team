#define _USE_MATH_DEFINES

#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Camera::Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up) :
	zoom(1.0)
{
	SetCameraLookAt(eye, at, up);
}

Camera::~Camera()
{
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	glm::vec4 eye4 = glm::vec4(eye[0], eye[1], eye[2],1);
	glm::vec4 at4 = glm::vec4(at[0], at[1], at[2], 1);
	glm::vec4 up4 = glm::vec4(up[0], up[1], up[2], 1);
	
	
	glm::vec4 z = normalize(eye4 - at4);
	//glm::vec4 x = cross(up4, z);
	//glm::vec4 y =
	//glm::vec4 t =
	//glm::vec4 c =
}

void Camera::SetOrthographicProjection(
	const float height,
	const float aspectRatio,
	const float near,
	const float far)
{

}

void Camera::SetPerspectiveProjection(
	const float fovy,
	const float aspectRatio,
	const float near,
	const float far)
{

}

void Camera::SetZoom(const float zoom)
{

}
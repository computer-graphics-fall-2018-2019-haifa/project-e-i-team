#define _USE_MATH_DEFINES

#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define PI 3.14159265


Camera::Camera(std::shared_ptr<MeshModel> model,const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up) :
	projectionTransformation(glm::mat4x4(1)),
	zoom(1.0),
	transType(0),
	ffovy(1.0f), fnear(1.0f), ffar(1.0f),
	MeshModel(model)
{
	SetCameraLookAt(eye, at, up);
}

Camera::~Camera()
{
}

//Elias emplementation: // glm::cross(v0,v1)
glm::vec4 Camera::cross(glm::vec4 vec0, glm::vec4 vec1) {
	float x = vec0.x * (vec1.x + vec1.y + vec1.z + vec1.w);
	float y = vec0.y * (vec1.x + vec1.y + vec1.z + vec1.w);
	float z = vec0.z * (vec1.x + vec1.y + vec1.z + vec1.w);
	float w = vec0.w * (vec1.x + vec1.y + vec1.z + vec1.w);
	return glm::vec4(x, y, z, w);
}

//Elias emplementation:
float Camera::aucDistance(glm::vec4 vec) {
	float sum = pow(vec.x,2) + pow(vec.y, 2) + pow(vec.z, 2) + pow(vec.w, 2);
	return sqrt(sum);
}

//Elias emplementation:
void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	glm::vec4 eye4 = glm::vec4(eye[0], eye[1], eye[2], 1);
	glm::vec4 at4 = glm::vec4(at[0], at[1], at[2], 1);
	glm::vec4 up4 = glm::vec4(up[0], up[1], up[2], 1);

	glm::vec4 n = normalize(eye4 - at4);
	glm::vec4 u = normalize(cross(up4 , n));
	glm::vec4 v = normalize(cross(n, u));
	glm::vec4 t = glm::vec4(0, 0, 0, 1);
	glm::mat4 c = glm::mat4(u, v, n, t);

	//Translate ( -eye )
	glm::vec4 v1 = glm::vec4(1, 0, 0, 0);
	glm::vec4 v2 = glm::vec4(0, 1, 0, 0);
	glm::vec4 v3 = glm::vec4(0, 0, 1, 0);
	glm::vec4 v4 = glm::vec4(-eye4.x, -eye4.y, -eye4.z, 1);


	viewTransformation = c * glm::mat4(v1, v2, v3, v4);
}


//Elias emplementation:
//aspectRatio = width / height
void Camera::SetOrthographicProjection(
	const float height,
	const float aspectRatio,
	const float near,
	const float far)
{
	float top = height / 2;
	float botton = -1 * top;
	float right = (aspectRatio * height) / 2;
	float left = -right;


	float x = -1 * ((right + left) / (right - left));
	float y = -1 * ((top + botton) / (top - botton));
	float z = -1 * ((far + near) / (far - near));

	glm::vec4 v1 = glm::vec4(2 / (right - left) , 0, 0, 0);
	glm::vec4 v2 = glm::vec4(0 , 2/(top - botton), 0, 0);
	glm::vec4 v3 = glm::vec4(0, 0, 2/(near - far), 0);
	glm::vec4 v4 = glm::vec4(x, y, z, 1);

	projectionTransformation = glm::mat4(v1 ,v2 ,v3 ,v4);
}


//Elias emplementation:
//fovy = the angle alfa
//aspectRatio = width / height
void Camera::SetPerspectiveProjection(
	const float fovy,
	const float aspectRatio,
	const float near,
	const float far)
{
	float top = tan((fovy / 2) * PI / 180.0) * near;
	float botton = -1 * top;
	float right = aspectRatio * top;
	float left = -right;


	float x = (right + left) / (right - left);
	float y = (top + botton) / (top - botton);
	float z = -1 * ((far + near) / (far - near));

	glm::vec4 v1 = glm::vec4( (2*near) / (right - left), 0, 0, 0);
	glm::vec4 v2 = glm::vec4(0, (2 * near) / (top - botton), 0, 0);
	glm::vec4 v3 = glm::vec4(x, y, z, -1);
	glm::vec4 v4 = glm::vec4(0, 0, -1 * ((2*far*near) / (far - near)), 0);

	projectionTransformation = glm::mat4(v1, v2, v3, v4);
}


//Elias emplementation:
void Camera::SetZoom(const float zoom)
{
	this->zoom = zoom;
}
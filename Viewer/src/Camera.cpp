#define _USE_MATH_DEFINES

#include "Trans.h"
#include "Camera.h"
#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


#define PI 3.14159265


Camera::Camera(std::shared_ptr<MeshModel> model,const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up) :
	viewTransformation(glm::mat4x4(1)),
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
void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	glm::vec3 n = glm::normalize(eye - at);
	glm::vec3 u = glm::normalize(glm::cross(up,n));
	glm::vec3 v = glm::normalize(glm::cross(n,u));
	
	glm::vec4 n4(n.x, n.y, n.z, 1);
	glm::vec4 u4(u.x, u.y, u.z, 1);
	glm::vec4 v4(v.x, v.y, v.z, 1);

	glm::vec4 t = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 c = glm::mat4(u4, v4, n4, t);

	glm::mat4x4 translation = Trans::getTranslate4x4(-eye.x, -eye.y, -eye.z);
	viewTransformation = c*translation;
}


//Elias emplementation:
//aspectRatio = width / height
void Camera::SetOrthographicProjection(
	const float height,
	const float aspectRatio,
	const float near,
	const float far)
{
//	float top = height / 2;
//	float botton = -1 * top;
//	float right = (aspectRatio * height) / 2;
//	float left = -right;
//
//
//	float x = -1 * ((right + left) / (right - left));
//	float y = -1 * ((top + botton) / (top - botton));
//	float z = -1 * ((far + near) / (far - near));
//
//	glm::vec4 v1 = glm::vec4(2 / (right - left) , 0, 0, 0);
//	glm::vec4 v2 = glm::vec4(0 , 2/(top - botton), 0, 0);
//	glm::vec4 v3 = glm::vec4(0, 0, 2/(near - far), 0);
//	glm::vec4 v4 = glm::vec4(x, y, z, 1);
//
//	projectionTransformation = glm::mat4(v1 ,v2 ,v3 ,v4);
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
	//float top = tan((fovy / 2) * PI / 180.0) * near;
	//float botton = -1 * top;
	//float right = aspectRatio * top;
	//float left = -right;


	//float x = (right + left) / (right - left);
	//float y = (top + botton) / (top - botton);
	//float z = -1 * ((far + near) / (far - near));

	//glm::vec4 v1 = glm::vec4( (2*near) / (right - left), 0, 0, 0);
	//glm::vec4 v2 = glm::vec4(0, (2 * near) / (top - botton), 0, 0);
	//glm::vec4 v3 = glm::vec4(x, y, z, -1);
	//glm::vec4 v4 = glm::vec4(0, 0, -1 * ((2*far*near) / (far - near)), 0);

	//projectionTransformation = glm::mat4(v1, v2, v3, v4);
}


//Elias emplementation:
void Camera::SetZoom(const float zoom)
{
	this->zoom = zoom;
}
#define _USE_MATH_DEFINES

#include "Trans.h"
#include "Camera.h"
#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Camera::Camera(std::shared_ptr<MeshModel> model,const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up, const float frameBufferWidth) :
	viewTransformation(glm::mat4x4(1)),
	projectionTransformation(glm::mat4x4(1)),
	transType(0),
	ffovy(FFOVY_BASIC_UNIT), fnear(MIN_FNEAR), ffar(MIN_FFAR), fleft(-frameBufferWidth / 2), fright(frameBufferWidth / 2), ftop(frameBufferWidth / 2), fbottom(-frameBufferWidth / 2),
	worldfRotatex(0.0f), worldfRotatey(0.0f), worldfRotatez(0.0f),
	MeshModel(model)
{
	SetCameraLookAt(eye, at, up);
	origin_eye = glm::vec3(eye.x, eye.y, eye.z);
}

Camera::~Camera(){}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	glm::vec3 z = glm::normalize(eye - at);
	glm::vec3 x = glm::normalize(glm::cross(up,z));
	glm::vec3 y = glm::normalize(glm::cross(z,x));
	glm::mat4 mat(1);
	mat[0][0] = x.x; mat[0][1] = y.x; mat[0][2] = z.x; mat[3][0] = glm::dot(x, eye);
	mat[1][0] = x.y; mat[1][1] = y.y; mat[1][2] = z.y; mat[3][1] = glm::dot(y, eye);
	mat[2][0] = x.z; mat[2][1] = y.z; mat[2][2] = z.z; mat[3][2] = glm::dot(z, eye);

	glm::vec3 v = at - eye;
	float dx = v.y / v.z;
	float dy = v.y / v.x;
	float xteta = atanf(dx);
	float yteta = atanf(dy);
	glm::mat4x4 Tunex = Trans::getxRotate4x4(xteta);
	glm::mat4x4 Tuney = Trans::getyRotate4x4(yteta);
	if (dx > 1.0f) { Tunex = Trans::getxRotate4x4(M_PI - xteta); } 
	else if (dy > 1.0f) { Tuney = Trans::getyRotate4x4(M_PI - yteta); }

	viewTransformation = mat; //Tuney * Tunex * mat;
	SetWorldTransformation(glm::inverse(mat));
}


//Elias emplementation:
//aspectRatio = width / height
void Camera::SetOrthographicProjection(
	float fovy,
	float aspectRatio,
	float pnear,
	float pfar,
	float pleft,
	float pright,
	float ptop,
	float pbottom)
{
	/*
	*	This projection is about to project the 3D Model to some hyperplane as 2D - zoom in is steps along z-axis
	*/

	glm::mat4x4 P(
		glm::vec4(1.0f / (pright - pleft), 0.0f, 0.0f, 0.0f),
		glm::vec4(0, 1.0f / (ptop - pbottom), 0.0f, 0.0f),
		glm::vec4(0, 0, -1.0f, 0.0f),
		glm::vec4(-(pright + pleft) / (pright - pleft), -(ptop + pbottom) / (ptop - pbottom), 0.0f, 1.0f)
	);

	projectionTransformation = P;
}

// Itay's Implementation
void Camera::SetPerspectiveProjection(
	float fovy,
	float aspectRatio,
	float pnear,
	float pfar,
	float pleft,
	float pright,
	float ptop,
	float pbottom)
{
	/*
	*	This projection is up to the gap between far hyperplane to near hyperplace which is parallel to y hyperplace
	*	=> cannot remain space to normals to be shown using very small gap [|near - far| < some epsilon]
	*/

#define e 1.0f / tanf((M_PI/180) * (fovy / 2.0f))
#define a e * aspectRatio

	//pnear = 1.0f, pfar = -1.0f;
	//pright = 1.0f, pleft = -1.0f, ptop = -1.0f, pbottom = 1.0f;

	glm::mat4x4 P(
		glm::vec4(e, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, a, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, (pfar + pnear) / (pfar - pnear), -1.0f),
		glm::vec4(0.0f, 0.0f, 2 * pfar * pnear / (pnear - pfar), 0.0f)
	);

	projectionTransformation = P;
}
#define _USE_MATH_DEFINES

#include "Trans.h"
#include "Camera.h"
#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Camera::Camera(std::shared_ptr<MeshModel> model,const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up) :
	viewTransformation(glm::mat4x4(1)),
	projectionTransformation(glm::mat4x4(1)),
	transType(0),
	ffovy(FFAR_DEF), fnear(FNEAR_DEF), ffar(FFAR_DEF), pleft(FLEFT_DEF), pright(FRIGHT_DEF), ptop(FTOP_DEF), pbottom(FBOTTOM_DEF),
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
	glm::vec3 x = glm::normalize(glm::cross(z,up));
	glm::vec3 y = glm::normalize(glm::cross(x,z));
	glm::mat4x4 lookAt(
		glm::vec4(x.x,x.y,x.z,0.0f),
		glm::vec4(y.x,y.y,y.z,0.0f),
		glm::vec4(-z.x,-z.y,-z.z,0.0f),
		glm::vec4(glm::dot(eye,x), glm::dot(eye,y), glm::dot(eye,z),1.0f)
	);

	viewTransformation = lookAt;
	SetWorldTransformation(glm::inverse(lookAt));
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
	float pbottom,
	float frameWidth)
{
	/*
	*	This projection is about to project the 3D Model to some hyperplane as 2D - zoom in is steps along z-axis
	*/
	float top = tanf(0.5f * fovy) * pnear;
	float botton = -1.0f * top;
	float right = aspectRatio * top;
	float left = -right;

	float S_x = 2.0f / (right - left);
	float S_y = 2.0f / (top - botton);
	float S_z = 2.0f / (pnear - pfar);
	float x = -1.0f * ((right + left) / (right - left));
	float y = -1.0f * ((top + botton) / (top - botton));
	float z = -1.0f * ((pfar + pnear) / (pfar - pnear));

	glm::mat4x4 P(
		glm::vec4(S_x, 0.0f, 0.0f, 0.0f),
		glm::vec4(0, S_y, 0.0f, 0.0f),
		glm::vec4(0, 0, S_z, 0.0f),
		glm::vec4(x, y, z, 1.0f)
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
	float pbottom,
	float frameWidth)
{
	/*
	*	This projection is up to the gap between far hyperplane to near hyperplace which is parallel to y hyperplace
	*	=> cannot remain space to normals to be shown using very small gap [|near - far| < some epsilon]
	*/

	ptop = tanf(glm::radians(0.5f * fovy)) * pnear;
	pbottom = -ptop;
	pright = ptop;
	pleft = -ptop * aspectRatio;

	glm::mat4x4 P(
		glm::vec4(2.0f * pnear / (pright - pleft), 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 2.0f * pnear / (ptop - pbottom), 0.0f, 0.0f),
		glm::vec4((pright + pleft) / (pright - pleft), (ptop + pbottom) / (ptop - pbottom), -(pfar + pnear) / (pfar - pnear), -1.0f),
		glm::vec4(0.0f, 0.0f, -2.0f * pfar * pnear / (pfar - pnear), 0.0f)
	);

	projectionTransformation = P;
}
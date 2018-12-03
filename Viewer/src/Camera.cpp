#define _USE_MATH_DEFINES

#include "Trans.h"
#include "Camera.h"
#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Camera::Camera(std::shared_ptr<MeshModel> model,const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up,glm::vec3& massCenter) :
	viewTransformation(glm::mat4x4(1)),
	projectionTransformation(glm::mat4x4(1)),
	transType(0),
	ffovy(FFAR_DEF), fnear(FNEAR_DEF), ffar(FFAR_DEF), left(FLEFT_DEF), right(FRIGHT_DEF),top(FTOP_DEF),bottom(FBOTTOM_DEF),
	worldfRotatex(0.0f), worldfRotatey(0.0f), worldfRotatez(0.0f), lrotatex(0.0f), lrotatey(0.0f), lrotatez(0.0f),
	MeshModel(model)
{
	SetCameraLookAt(eye, at, up);
	origin_eye = glm::vec3(eye.x, eye.y, eye.z);
	origin_at = glm::vec3(at.x, at.y, at.z);
	origin_up = glm::vec3(up.x, up.y, up.z);
	
	glm::vec3 fixed_sight = massCenter - origin_at;
	glm::vec3 vertical = origin_eye - fixed_sight;
	float theta = atanf(glm::radians(glm::length(vertical) / glm::length(fixed_sight)));
	UpdateworldTransform(Trans::get2InitAxis4x4(vertical, Trans::getyRotate4x4(theta)));
}

Camera::~Camera(){}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up,bool recalculate)
{
	glm::vec3 z, x, y;
	if (recalculate) {
		// change direct angle
		glm::vec3 direct = eye - at;
		z = glm::normalize(direct); // cameraDirection
		x = glm::normalize(glm::cross(up, z)); // cameraRight
		y = glm::normalize(glm::cross(z, x)); // cameraUp
		origin_u = x;
		origin_v = y;
		origin_n = z;
	} else {
		origin_eye = eye;
		origin_at = at;
		origin_up = up;
		x = origin_u;
		y = origin_v;
		z = origin_n;
	}
	glm::mat4x4 lookAt(
		glm::vec4(x.x,x.y,x.z,0.0f),
		glm::vec4(y.x,y.y,y.z,0.0f),
		glm::vec4(z.x,z.y,z.z,0.0f),
		glm::vec4(-glm::dot(eye,x), -glm::dot(eye,y), -glm::dot(eye,z),1.0f)
	);

	viewTransformation = lookAt;
	SetWorldTransformation(glm::inverse(lookAt));
}

// mat - rotation matrix
void Camera::UpdateCameraView(float angle) {
	//roll(angle);
}

//Elias emplementation:
//aspectRatio = width / height
void Camera::SetOrthographicProjection(
	float fovy,
	float aspectRatio,
	float pnear,
	float pfar,
	float left,
	float right,
	float top,
	float bottom,
	float frameWidth)
{
	/*
	*	This projection is about to project the 3D Model to some hyperplane as 2D - zoom in is steps along z-axis
	*/
	float ptop = tanf(0.1f * glm::radians(fovy)) * pnear;
	float pbottom = -1.0f * top;
	float pright = aspectRatio * top;
	float pleft = -right;

	float S_x = 2.0f / (pright - pleft);
	float S_y = 2.0f / (ptop - pbottom);
	float S_z = 2.0f / (pnear - pfar);
	float x = -((pright + pleft) / (pright - pleft));
	float y = -((ptop + pbottom) / (ptop - pbottom));
	float z = -((pfar + pnear) / (pfar - pnear));

	glm::mat4x4 P(
		glm::vec4(S_x, 0.0f, 0.0f, 0.0f),
		glm::vec4(0, S_y, 0.0f, 0.0f),
		glm::vec4(0, 0, S_z, 0.0f),
		glm::vec4(x, y, z, 1.0f)
	);

	projectionTransformation = P;
}


void Camera::SetPerspectiveProjection(
	float fovy,
	float aspectRatio,
	float pnear,
	float pfar,
	float left,
	float right,
	float top,
	float bottom,
	float frameWidth)
{
	/*
	*	This projection is up to the gap between far hyperplane to near hyperplace which is parallel to y hyperplace
	*	=> cannot remain space to normals to be shown using very small gap [|near - far| < some epsilon]
	*/
	/*cout << "fovy = " << fovy << endl;
	cout << "radi = " << glm::radians(fovy) << endl;*/
	float ptop = tanf(0.1f * glm::radians(fovy)) * pnear;
	float pbottom = -ptop;
	float pright = ptop * aspectRatio;
	float pleft = -pright;

	
	glm::mat4x4 P(
		glm::vec4(2.0f * pnear / (pright - pleft), 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 2.0f * pnear / (ptop - pbottom), 0.0f, 0.0f),
		glm::vec4((pright + pleft) / (pright - pleft), (ptop + pbottom) / (ptop - pbottom), -(pfar + pnear) / (pfar - pnear), -1.0f),
		glm::vec4(0.0f, 0.0f, -2.0f * pfar * pnear / (pfar - pnear), 0.0f)
	);

	projectionTransformation = P;
}

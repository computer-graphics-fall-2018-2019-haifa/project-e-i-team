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
	ofovy(OFOVY_DEF),pfovy(PFOVY_DEF), fnear(FNEAR_DEF), ffar(FFAR_DEF), left(FLEFT_DEF), right(FRIGHT_DEF),top(FTOP_DEF),bottom(FBOTTOM_DEF),
	worldfRotatex(0.0f), worldfRotatey(0.0f), worldfRotatez(0.0f), lrotatex(0.0f), lrotatey(0.0f), lrotatez(0.0f),
	MeshModel(model)
{
	SetCameraLookAt(eye, at, up);
	origin_eye = glm::vec3(eye.x, eye.y, eye.z);
	origin_at = glm::vec3(at.x, at.y, at.z);
	origin_up = glm::vec3(up.x, up.y, up.z);
	
	// visual camera tuning:
	glm::vec3 fixed_sight = massCenter - origin_at;
	glm::vec3 vertical = origin_eye - fixed_sight;
	float theta = atanf(glm::radians(glm::length(vertical) / glm::length(fixed_sight)));
	UpdateworldTransform(Trans::get2InitAxis4x4(vertical, Trans::getyRotate4x4(theta)));

	FrustrumType = 1;
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

	viewTransformation = glm::inverse(lookAt);
	SetWorldTransformation((lookAt));
}
void Camera::UpdateviewTransformation(glm::mat4x4 matrix) {
	viewTransformation = viewTransformation * glm::inverse(matrix);
}


//aspectRatio = width / height
void Camera::SetOrthographicProjection(	float aspectRatio,float frameWidth)
{
	float ptop, pbottom, pright, pleft;
	if (FrustrumType) {
		//TODO: elias
		/*ptop = top;
		pbottom = -1.0f * ptop;
		pright = aspectRatio * ptop;
		pleft = -pright;*/
		ptop = top;
		pbottom = bottom;
		pright = right;
		pleft = left;
	} else {
		ptop = tanf(0.1f * glm::radians(ofovy)) * fnear;
		pbottom = -1.0f * ptop;
		pright = aspectRatio * ptop;
		pleft = -pright;
	}
	
	float S_x = 2.0f / (pright - pleft);
	float S_y = 2.0f / (ptop - pbottom);
	float S_z = 2.0f / (fnear - ffar);
	float x = -((pright + pleft) / (pright - pleft));
	float y = -((ptop + pbottom) / (ptop - pbottom));
	float z = -((ffar + fnear) / (ffar - fnear));

	glm::mat4x4 P(
		glm::vec4(S_x, 0.0f, 0.0f, 0.0f),
		glm::vec4(0, S_y, 0.0f, 0.0f),
		glm::vec4(0, 0, S_z, 0.0f),
		glm::vec4(x, y, z, 1.0f)
	);

	projectionTransformation = P;
}



void Camera::SetPerspectiveProjection(float aspectRatio,float frameWidth)
{
	/*cout << "fovy = " << fovy << endl;
	cout << "radi = " << glm::radians(fovy) << endl;*/
	float ptop, pbottom, pright, pleft;
	if (FrustrumType) {
		//TODO: elias
		/*ptop = top;
		pbottom = -1.0f * ptop;
		pright = aspectRatio * ptop;
		pleft = -pright;*/
		ptop = top;
		pbottom = bottom;
		pright = right;
		pleft = left;
	}
	else {
		ptop = tanf(0.1f * glm::radians(pfovy)) * fnear;
		pbottom = -1.0f * ptop;
		pright = aspectRatio * ptop;
		pleft = -pright;
		//cout << "ptop = " << ptop << endl;
	}

	/*float ptop = tanf(0.1f * glm::radians(ffovy)) * fnear;
	float pbottom = -ptop;
	float pright = ptop * aspectRatio;
	float pleft = -pright;*/

	glm::mat4x4 P(
		glm::vec4(2.0f * fnear / (pright - pleft), 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 2.0f * fnear / (ptop - pbottom), 0.0f, 0.0f),
		glm::vec4((pright + pleft) / (pright - pleft), (ptop + pbottom) / (ptop - pbottom), -(ffar + fnear) / (ffar - fnear), -1.0f),
		glm::vec4(0.0f, 0.0f, -2.0f * ffar * fnear / (ffar - fnear), 0.0f)
	);

	projectionTransformation = P;
}

void Camera::roll(float angle) { // roll the camera through angle degrees
	float cs = cosf(angle);
	float sn = sinf(angle);
	glm::vec3 t = origin_u; // remember old u
	//right
	origin_u = glm::vec3(
		cs * t.x + -sn * origin_v.x,
		cs * t.y + -sn * origin_v.y,
		cs * t.z + -sn * origin_v.z
	);
	//up
	origin_v = glm::vec3(
		sn * t.x + cs * origin_v.x,
		sn * t.y + cs * origin_v.y,
		sn * t.z + cs * origin_v.z
	);
	SetCameraLookAt(origin_eye, origin_at, origin_up, false);
}

void Camera::slide(float delU, float delV, float delN) {
	origin_eye.x += delU * origin_u.x + delV * origin_v.x + delN * origin_n.x;
	origin_eye.y += delU * origin_u.y + delV * origin_v.y + delN * origin_n.y;
	origin_eye.z += delU * origin_u.z + delV * origin_v.z + delN * origin_n.z;
	SetCameraLookAt(origin_eye, origin_at, origin_up, false);
}

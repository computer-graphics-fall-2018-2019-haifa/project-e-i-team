#define _USE_MATH_DEFINES

#include "Trans.h"
#include "Camera.h"
#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Camera::Camera(std::shared_ptr<MeshModel> model,const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up,glm::vec3 massCenter) :
	viewTransformation(glm::mat4x4(1)),
	projectionTransformation(glm::mat4x4(1)),
	transType(0),
	ffovy(FFAR_DEF), fnear(FNEAR_DEF), ffar(FFAR_DEF), yaw(0.0f), pitch(0.0f), left(FLEFT_DEF), right(FRIGHT_DEF),top(FTOP_DEF),bottom(FBOTTOM_DEF),
	worldfRotatex(0.0f), worldfRotatey(0.0f), worldfRotatez(0.0f), selffRotatex(0.0f), selffRotatey(0.0f), selffRotatez(0.0f),
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

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{

	// glm::vec3 z = glm::normalize(eye - at);
	// glm::vec3 x = glm::normalize(glm::cross(up,z));
	// glm::vec3 y = glm::normalize(glm::cross(x,z));
	// glm::mat4 mat(1);
	// mat[0][0] = x.x; mat[0][1] = y.x; mat[0][2] = z.x; //mat[3][0] = glm::dot(x, eye);
	// mat[1][0] = x.y; mat[1][1] = y.y; mat[1][2] = z.y; //mat[3][1] = glm::dot(y, eye);
	// mat[2][0] = x.z; mat[2][1] = y.z; mat[2][2] = z.z; //mat[3][2] = glm::dot(z, eye);

	// glm::mat4x4 trans
	// { 1	,	0	,	0	,	eye.x,
	// 	0	,	1	,	0	,	eye.y,
	// 	0	,	0	,	1	,	eye.z,
	// 	0	,	0	,	0	,	1 };

	// viewTransformation = mat * glm::transpose(trans);
	// SetWorldTransformation(glm::inverse(viewTransformation));

	glm::vec3 z = glm::normalize(eye - at); // cameraDirection
	glm::vec3 x = glm::normalize(glm::cross(up,z)); // cameraRight
	glm::vec3 y = glm::normalize(glm::cross(z,x)); // cameraUp

	glm::mat4x4 lookAt(
		glm::vec4(x.x,x.y,x.z,0.0f),
		glm::vec4(y.x,y.y,y.z,0.0f),
		glm::vec4(z.x,z.y,z.z,0.0f),
		glm::vec4(-glm::dot(eye,x), -glm::dot(eye,y), -glm::dot(eye,z),1.0f)
	);

	viewTransformation = lookAt;
	SetWorldTransformation(glm::inverse(lookAt));
}


void Camera::UpdateCameraView(glm::mat4x4& rotMat) {
	glm::mat4x4 toZero = Trans::getTranslate4x4(-origin_eye.x, -origin_eye.y, -origin_eye.z);
	glm::mat4x4 toOrigin = Trans::getTranslate4x4(origin_eye.x, origin_eye.y, origin_eye.z);
	viewTransformation = toOrigin * rotMat * toZero * viewTransformation;
}

/*
void glhLookAtf2(float *matrix, float *eyePosition3D,
	float *center3D, float *upVector3D)
{
	float forward[3], side[3], up[3];
	float matrix2[16], resultMatrix[16];
	// --------------------
	forward[0] = center3D[0] - eyePosition3D[0];
	forward[1] = center3D[1] - eyePosition3D[1];
	forward[2] = center3D[2] - eyePosition3D[2];
	NormalizeVector(forward);
	// --------------------
	// Side = forward x up
	ComputeNormalOfPlane(side, forward, upVector3D);
	NormalizeVector(side);
	--------------------
		// Recompute up as: up = side x forward
		ComputeNormalOfPlane(up, side, forward);
	// --------------------
	matrix2[0] = side[0];
	matrix2[4] = side[1];
	matrix2[8] = side[2];
	matrix2[12] = 0.0;
	// --------------------
	matrix2[1] = up[0];
	matrix2[5] = up[1];
	matrix2[9] = up[2];
	matrix2[13] = 0.0;
	// --------------------
	matrix2[2] = -forward[0];
	matrix2[6] = -forward[1];
	matrix2[10] = -forward[2];
	matrix2[14] = 0.0;
	// --------------------
	matrix2[3] = matrix2[7] = matrix2[11] = 0.0;
	matrix2[15] = 1.0;
	// --------------------
	MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, matrix2);
	glhTranslatef2(resultMatrix,
		-eyePosition3D[0], -eyePosition3D[1], -eyePosition3D[2]);
	// --------------------
	memcpy(matrix, resultMatrix, 16 * sizeof(float));
}
*/

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
	float yaw,
	float pitch,
	float frameWidth)
{

	// float top = tan(fovy / 2) * snear;
	// float botton = -1 * top;
	// float right = aspectRatio * top;
	// float left = -right;

	// float S_x = 2.0f / (right - left);
	// float S_y = 2.0f / (top - botton);
	// float S_z = 2.0f / (snear - sfar);
	// float x = -1.0f * ((right + left) / (right - left));
	// float y = -1.0f * ((top + botton) / (top - botton));
	// float z = -1.0f * ((sfar + snear) / (sfar - snear));

	// glm::vec4 v1 = glm::vec4(S_x, 0.0f, 0.0f, 0.0f);
	// glm::vec4 v2 = glm::vec4(0 , S_y, 0.0f, 0.0f);
	// glm::vec4 v3 = glm::vec4(0, 0, S_z, 0.0f);
	// glm::vec4 v4 = glm::vec4(x, y, z, 1.0f);

	// projectionTransformation = Trans::getScale4x4(zoom) * glm::mat4(v1 ,v2 ,v3 ,v4);

	/*
	*	This projection is about to project the 3D Model to some hyperplane as 2D - zoom in is steps along z-axis
	*/
	float ptop = tanf(0.5f * fovy) * pnear;
	float pbottom = -1.0f * top * bottom;
	float pright = aspectRatio * top * right;
	float pleft = -right * left;

	float S_x = 2.0f / (pright - pleft);
	float S_y = 2.0f / (ptop - pbottom);
	float S_z = 2.0f / (pnear - pfar);
	float x = -1.0f * ((pright + pleft) / (pright - pleft));
	float y = -1.0f * ((ptop + pbottom) / (ptop - pbottom));
	float z = -1.0f * ((pfar + pnear) / (pfar - pnear));

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
	float yaw,
	float pitch,
	float frameWidth)
{

	/*
		float n = pnear;
		float f = pfar;
		float t = tan(fovy / 2) * pnear;
		float b = -1 * t;
		float r = aspectRatio * t;
		float l = -r;

		glm::vec4 v1((2 * n) / (r - l)	, 0					, 0					, 0);
		glm::vec4 v2(0					, (2*n)/(t-b)		, 0					, 0);
		glm::vec4 v3((r+l)/(r-l)		, (t + b) / (t - b)	, -(f + n) / (f - n), -1);
		glm::vec4 v4(0					, 0					, -(2*f*n)/(f-n)	, 0);

		projectionTransformation = glm::mat4(v1, v2, v3, v4);
	}

	/*
		float pneardef = 1.0f, pfardef = -1.0f; //  use as constant because of the lecture explainations - can put it as parameterized changing
		float wright = frameBufferWidth, wleft = -frameBufferWidth / 2, wtop = frameBufferWidth / 2, wbottom = -frameBufferWidth / 2;
	*/

	/*
	*	This projection is up to the gap between far hyperplane to near hyperplace which is parallel to y hyperplace
	*	=> cannot remain space to normals to be shown using very small gap [|near - far| < some epsilon]
	*/

	float ptop = tanf(glm::radians(0.5f * fovy)) * pnear;
	float pbottom = -ptop * bottom;
	float pright = ptop * right;
	float pleft = -ptop * aspectRatio * left;

	//glm::mat4x4 P(
	//	glm::vec4(1 / pright, 0.0f, 0.0f, 0.0f),
	//	glm::vec4(0.0f, 1 / ptop, 0.0f, 0.0f),
	//	glm::vec4(0.0f, 0.0f, -2 / (pfar - pnear), 0.0f),
	//	glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	//);

	glm::mat4x4 P(
		glm::vec4(2.0f * pnear / (pright - pleft), 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 2.0f * pnear / (ptop - pbottom), 0.0f, 0.0f),
		glm::vec4((pright + pleft) / (pright - pleft), (ptop + pbottom) / (ptop - pbottom), -(pfar + pnear) / (pfar - pnear), -1.0f),
		glm::vec4(0.0f, 0.0f, -2.0f * pfar * pnear / (pfar - pnear), 0.0f)
	);

	projectionTransformation = P;
}
}

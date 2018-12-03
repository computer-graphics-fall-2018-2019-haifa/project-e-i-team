#pragma once
#include "MeshModel.h"
#include <memory>
#include <glm/glm.hpp>

#define FFOVY_DEF	1.0f
#define FNEAR_DEF	-1.0f
#define FFAR_DEF	1.0f
#define BOX_BOUNDERY_RANGE	100.0f

//By the book parameters value:
//==============================
#define FLEFT_DEF	-1.0f
#define FRIGHT_DEF	1.0f
#define FTOP_DEF	1.0f
#define FBOTTOM_DEF -1.0f

/*
* Pseudo Algorithm of the camera behavior in transformation's relationship:
* ========================================================================
* if Orthogonal:
*	* M_projectionTransformation = SetOrthographicProjection()
* if perspective:
*	* M_projectionTransformation = SetPerspectiveProjection()
* M_viewTransformation = SetCameraLookAt()
* newVector = M_projectionTransformation*M_viewTransformation*M_worldTransform*vector
* newVector -> newVector/newVector[w]
* newVector = (x/w , y/w , z/w , 1)
* remove z/w and 1
* newVector = (x/w , y/w)
* bresenham(x/w , y/w)
*/
/*
 * Camera class. This class takes care of all the camera transformations and manipulations.
 *
 * Implementation suggestion:
 * --------------------------
 * Make the Camera class be a subclass of MeshModel, so you can easily and elegantly render 
 * the cameras you have added to the scene.
 */
class Camera : public MeshModel
{
private:
	glm::mat4x4 viewTransformation; // Mc
	glm::mat4x4 projectionTransformation; // Mp
	glm::vec3 origin_u, origin_v, origin_n;
public:
	float worldfRotatex, worldfRotatey, worldfRotatez, lrotatex, lrotatey, lrotatez;
	glm::vec3 origin_eye,origin_at,origin_up;
	int transType;
	float ffovy, fnear, ffar, left, right, top, bottom;
	Camera(std::shared_ptr<MeshModel> model,const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up, glm::vec3& massCenter);
	~Camera();
	
	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up, bool recalculate=true);
	void Camera::UpdateCameraView(float angle);

	// Working great!
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
		SetCameraLookAt(origin_eye, origin_at, origin_up,false);
	}

	// Not working right!
	void Camera::yaw(float angle) { // roll the camera through angle degrees
		float cs = cosf(angle);
		float sn = sinf(angle);
		glm::vec3 t = origin_n; // remember old u
		//up
		origin_v = cs * t + -sn * origin_v;
		//up
		origin_v = glm::vec3(
			sn * t.x + cs * origin_v.x,
			sn * t.y + cs * origin_v.y,
			sn * t.z + cs * origin_v.z
		);
		SetCameraLookAt(origin_eye, origin_at, origin_up, false);
	}

	// Not working right!
	void Camera::pitch(float angle) { // roll the camera through angle degrees
		float cs = cosf(angle);
		float sn = sinf(angle);
		glm::vec3 t = origin_n; // remember old u
		//dir
		origin_n = glm::vec3(
			cs * t.x + -sn * origin_u.x,
			cs * t.y + -sn * origin_u.y,
			cs * t.z + -sn * origin_u.z
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
		SetCameraLookAt(origin_eye, origin_at, origin_up,false);
	}

	void SetOrthographicProjection(
		float fovy,
		float aspectRatio,
		float pnear,
		float far,
		float left,
		float right,
		float top,
		float bottom,
		float frameWidth);

	void SetPerspectiveProjection(
		float fovy,
		float aspectRatio,
		float pnear,
		float pfar,
		float left,
		float right,
		float top,
		float bottom,
		float frameWidth);

	glm::mat4x4 Getview() { return viewTransformation; }
	void Updateview(glm::mat4x4& rotateView) { viewTransformation = rotateView * viewTransformation; }
	void UpdateProjection(glm::mat4x4& rotateView) { projectionTransformation = rotateView * projectionTransformation; }
	glm::mat4x4 GetProjection() { return projectionTransformation;  }
};

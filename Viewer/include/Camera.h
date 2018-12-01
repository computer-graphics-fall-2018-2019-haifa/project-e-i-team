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

public:
	float worldfRotatex, worldfRotatey, worldfRotatez, selffRotatex, selffRotatey, selffRotatez;
	glm::vec3 origin_eye,origin_at,origin_up;
	int transType;
	float ffovy, fnear, ffar, yaw, pitch, left, right, top, bottom;
	Camera(std::shared_ptr<MeshModel> model,const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up);
	~Camera();
	void Camera::setMassCenter(glm::vec3& massCenter);
	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	void Camera::UpdateCameraView(glm::mat4x4& mat);

	void SetOrthographicProjection(
		float fovy,
		float aspectRatio,
		float pnear,
		float far,
		float left,
		float right,
		float top,
		float bottom,
		float yaw,
		float pitch,
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
		float yaw,
		float pitch,
		float frameWidth);

	glm::mat4x4 Getview() { return viewTransformation; }
	void Updateview(glm::mat4x4& rotateView) { viewTransformation = rotateView * viewTransformation; }
	void UpdateProjection(glm::mat4x4& rotateView) { projectionTransformation = rotateView * projectionTransformation; }
	glm::mat4x4 GetProjection() { return projectionTransformation;  }
};

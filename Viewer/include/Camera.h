#pragma once
#include "MeshModel.h"
#include <memory>
#include <glm/glm.hpp>


#define PFOVY_DEF	1.0f
#define OFOVY_DEF	200.0f


#define FNEAR_DEF	-1.0f
#define FFAR_DEF	1.0f
#define BOX_BOUNDERY_RANGE	100.0f

/*
By the book parameters value:
==============================
*/
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
	int transType,FrustrumType;
	float ofovy, pfovy, fnear, ffar, left, right, top, bottom;

	Camera(std::shared_ptr<MeshModel> model,const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up, glm::vec3& massCenter);
	~Camera();
	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up, bool recalculate=true);
	void Camera::roll(float angle);

	// void Camera::yaw(float angle);		// Not working right
	// void Camera::pitch(float angle);		// Not working right

	void Camera::slide(float delU, float delV, float delN);
	void Camera::UpdateviewTransformation(glm::mat4x4 matrix);
	/*
	*	this projection is about to project the 3D Model to some hyperplane as 2D - zoom in is steps along z-axis
	*/
	//void SetOrthographicProjection(	float aspectRatio, float frameWidth);
	void Camera::SetOrthographicProjection(
		const float height,
		const float aspectRatio,
		const float zNear,
		const float zFar);
	/*
	*	this projection is up to the gap between far hyperplane to near hyperplace which is parallel to y hyperplace
	*	=> cannot remain space to normals to be shown using very small gap [|near - far| < some epsilon]
	*/
	//void SetPerspectiveProjection(float aspectRatio, float frameWidth);
	void Camera::SetPerspectiveProjection(
		const float fovy,
		const float aspectRatio,
		const float zNear,
		const float zFar);
	
	glm::mat4x4 Getview() { return viewTransformation; }
	void Updateview(glm::mat4x4& rotateView) { viewTransformation = rotateView * viewTransformation; }
	void UpdateProjection(glm::mat4x4& rotateView) { projectionTransformation = rotateView * projectionTransformation; }
	glm::mat4x4 GetProjection() { return projectionTransformation;  }
};

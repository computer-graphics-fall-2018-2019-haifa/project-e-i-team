#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "MeshModel.h"

#define MIN_FFOVY 0.0f
#define MAX_FFOVY 3.142f

#define MIN_FNEAR 1.0f
#define MAX_FNEAR 10.0f

#define MIN_FFAR 1.0f
#define MAX_FFAR 10.0f

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
	/*
	Pseudo:
	if Orthogonal:
		M_projectionTransformation = SetOrthographicProjection()
	if perspective:
		M_projectionTransformation = SetPerspectiveProjection()
	M_viewTransformation = SetCameraLookAt()
	newVector = M_projectionTransformation*M_viewTransformation*M_worldTransform*vector
	newVector -> newVector/newVector[w]
	newVector = (x/w , y/w , z/w , 1)
	remove z/w and 1
	newVector = (x/w , y/w)
	bresenham(x/w , y/w)
	*/
	glm::mat4x4 viewTransformation; // Mc
	glm::mat4x4 projectionTransformation; // Mp
	float zoom;

public:
	glm::vec3 origin_eye;
	int transType;
	float ffovy,fnear,ffar;
	Camera(std::shared_ptr<MeshModel> model,const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up);
	~Camera();

	


	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	void SetOrthographicProjection(
		const float height,
		const float aspectRatio,
		const float near,
		const float far);

	void SetPerspectiveProjection(
		const float fovy,
		const float aspect,
		const float near,
		const float far);


	glm::mat4x4 Getview() { return viewTransformation; }

	glm::mat4x4 GetProjection() { return projectionTransformation;  }

	void SetZoom(const float zoom);

	// Add more methods/functionality as needed...
};

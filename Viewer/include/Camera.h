#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "MeshModel.h"

/*
 * Camera class. This class takes care of all the camera transformations and manipulations.
 *
 * Implementation suggestion:
 * --------------------------
 * Make the Camera class be a subclass of MeshModel, so you can easily and elegantly render 
 * the cameras you have added to the scene.
 */
class Camera
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
	int transType;
	float ffovy,fnear,ffar;
	Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up);
	~Camera();


	glm::vec4 Camera::cross(glm::vec4 vec0, glm::vec4 vec1);
	float Camera::aucDistance(glm::vec4 vec);

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

	void SetZoom(const float zoom);

	// Add more methods/functionality as needed...
};

#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "MeshModel.h"
/*
 * Camera class. This class takes care of all the camera transformations and manipulations.
 */
class Camera
{
private:
	glm::mat4x4 viewTransformation;
	glm::mat4x4 projectionTransformation;

	glm::vec3 eye;
	glm::vec3 up;
	glm::vec3 at;

	glm::vec3 x;
	glm::vec3 y;
	glm::vec3 z;

	

public:
	float zoom;
	float fovy;
	float height;
	float zNear;
	float zFar;
	float aspectRatio;

	bool prespective;


	Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up, const float aspectRatio);
	~Camera();

	void SetOrthographicProjection(
		const float height,
		const float aspectRatio,
		const float zNear,
		const float zFar);

	void SetPerspectiveProjection(
		const float fovy,
		const float aspect,
		const float zNear,
		const float zFar);

	void UpdateProjectionMatrix();

	void SetNear(const float zNear);

	void SetFar(const float zFar);

	void SetFovy(const float fovy);

	void SetHeight(const float height);

	void Zoom(const float factor);

	void SphericalRotate(const glm::vec2& sphericalDelta);

	const glm::mat4x4& GetProjectionTransformation() const;

	const glm::mat4x4& GetViewTransformation() const;

	void SetAspectRatio(float aspectRatio);

	void SwitchToPrespective();
	void SwitchToOrthographic();

	float GetNear();

	float GetFar();

	float GetFovy();

	float GetHeight();

	bool IsPrespective();

	const glm::vec3& GetEye() const;


    void TranslateModel(const glm::vec3& translationVector);
    void TranslateWorld(const glm::vec3& translationVector);

    void RotateXModel(double angle);
    void RotateYModel(double angle);
    void RotateZModel(double angle);
    void ScaleXModel(double factor);
    void ScaleYModel(double factor);
    void ScaleZModel(double factor);
    void ScaleModel(double factor);

    void RotateXWorld(double angle);
    void RotateYWorld(double angle);
    void RotateZWorld(double angle);
    void ScaleXWorld(double factor);
    void ScaleYWorld(double factor);
    void ScaleZWorld(double factor);
    void ScaleWorld(double factor);
};

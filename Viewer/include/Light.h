#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
#include <memory>
#include "MeshModel.h"
#include "Face.h"

class Light
{
public:
	Light();
	Light(const glm::vec3& color);
	virtual ~Light();

	const glm::vec3& GetColor()  const;
	void SetColor(const glm::vec3& color);

	//float WScale, WRotatex, WRotatey, WRotatez, MScale, MRotatex, MRotatey, MRotatez;
	//float WTranslatex, WTranslatey, WTranslatez;

	const glm::mat4x4& GetWorldTransformation() const;
	const glm::mat4x4& GetModelTransformation() const;

	void SetWorldTransformation(const glm::mat4x4& worldTransform);
	void SetModelTransformation(const glm::mat4x4& modelTransform);


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

protected:
	glm::vec3 color;
	glm::mat4x4 modelTransform;
	glm::mat4x4 worldTransform;
};

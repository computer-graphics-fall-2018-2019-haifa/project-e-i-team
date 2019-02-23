#include "Light.h"
#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <glm/gtc/matrix_transform.hpp>

Light::Light() :
	color(glm::vec3(1.0, 1.0, 1.0)), modelTransform(1),	worldTransform(1)
{
}

Light::Light(const glm::vec3& color) :
	color(color), modelTransform(1), worldTransform(1)
{

}


Light::~Light()
{
}

const glm::vec3& Light::GetColor() const
{
	return color;
}

void Light::SetColor(const glm::vec3& color)
{
	this->color = color;
}




void Light::SetWorldTransformation(const glm::mat4x4& worldTransform)
{
	this->worldTransform = worldTransform;
}

const glm::mat4x4& Light::GetWorldTransformation() const
{
	return worldTransform;
}

void Light::SetModelTransformation(const glm::mat4x4& worldTransform)
{
	this->modelTransform = worldTransform;
}

const glm::mat4x4& Light::GetModelTransformation() const
{
	return modelTransform;
}

void Light::TranslateModel(const glm::vec3& translationVector)
{
	modelTransform = Utils::TranslationMatrix(translationVector) * modelTransform;
}

void Light::TranslateWorld(const glm::vec3& translationVector)
{
	worldTransform = Utils::TranslationMatrix(translationVector) * worldTransform;
}

void Light::RotateXModel(double angle)
{
	modelTransform = Utils::XRotationMatrix(angle) * modelTransform;
}

void Light::RotateYModel(double angle)
{
	modelTransform = Utils::YRotationMatrix(angle) * modelTransform;
}

void Light::RotateZModel(double angle)
{
	modelTransform = Utils::ZRotationMatrix(angle) * modelTransform;
}

void Light::ScaleXModel(double factor)
{
	modelTransform = Utils::XScalingMatrix(factor) * modelTransform;
}

void Light::ScaleYModel(double factor)
{
	modelTransform = Utils::YScalingMatrix(factor) * modelTransform;
}

void Light::ScaleZModel(double factor)
{
	modelTransform = Utils::ZScalingMatrix(factor) * modelTransform;
}

void Light::ScaleModel(double factor)
{
	modelTransform = glm::scale(glm::mat4(1), glm::vec3(factor, factor, factor)) * modelTransform;
}

void Light::RotateXWorld(double angle)
{
	worldTransform = Utils::XRotationMatrix(angle) * worldTransform;
}

void Light::RotateYWorld(double angle)
{
	worldTransform = Utils::YRotationMatrix(angle) * worldTransform;
}

void Light::RotateZWorld(double angle)
{
	worldTransform = Utils::ZRotationMatrix(angle) * worldTransform;
}

void Light::ScaleXWorld(double factor)
{
	worldTransform = Utils::XScalingMatrix(factor) * worldTransform;
}

void Light::ScaleYWorld(double factor)
{
	worldTransform = Utils::YScalingMatrix(factor) * worldTransform;
}

void Light::ScaleZWorld(double factor)
{
	worldTransform = Utils::ZScalingMatrix(factor) * worldTransform;
}

void Light::ScaleWorld(double factor)
{
	worldTransform = glm::scale(glm::mat4(1), glm::vec3(factor, factor, factor)) * worldTransform;
}

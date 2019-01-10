#pragma once
#include "MeshModel.h"
#include <memory>
#include <glm/glm.hpp>



class AmbientLight : public MeshModel
{
private:
	glm::vec3 base;
public:
	float Ka,La;
    AmbientLight::AmbientLight(glm::vec3 base);
    AmbientLight::~AmbientLight();
	glm::vec3 AmbientLight::GetBaseVector();
	glm::vec3 AmbientLight::GetLocationAfterTrans();
};

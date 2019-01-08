#pragma once
#include "MeshModel.h"
#include <memory>
#include <glm/glm.hpp>



class ParallelLight : public MeshModel
{
private:
	glm::vec3 from;
	glm::vec3 to;
public:
	float Ld, Ls;
    ParallelLight::ParallelLight(glm::vec3 to);
    ParallelLight::~ParallelLight();
	glm::vec3& ParallelLight::GetToVector();
	glm::vec3& ParallelLight::GetfromVector();
    glm::vec3 ParallelLight::GetLocationAfterTrans();
    glm::vec3 ParallelLight::GetDirectionAfterTrans();
};

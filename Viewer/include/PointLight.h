#pragma once
#include "MeshModel.h"
#include <memory>
#include <glm/glm.hpp>



class PointLight : public MeshModel
{

public:
	glm::vec3 Center;
	float Ld, Ls;
    PointLight::PointLight(std::shared_ptr<MeshModel> model);
	glm::vec3 PointLight::GetLocationAfterTrans(glm::mat4x4 camTrans);
    glm::vec3 PointLight::GetLocationAfterTrans();
    PointLight::~PointLight();

};

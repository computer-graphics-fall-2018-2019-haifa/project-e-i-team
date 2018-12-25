#pragma once
#include "MeshModel.h"
#include <memory>
#include <glm/glm.hpp>


class PointLight : public MeshModel
{

public:
	glm::vec3 Center;
	float La, Ld, Ls;
	PointLight(std::shared_ptr<MeshModel> model);
	glm::vec3 GetLocationAfterTrans();
	~PointLight();

};

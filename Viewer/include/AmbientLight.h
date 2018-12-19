#pragma once
#include "MeshModel.h"
#include <memory>
#include <glm/glm.hpp>



class AmbientLight : public MeshModel
{
private:
	glm::vec3 base;
public:
	AmbientLight(glm::vec3 base);
	~AmbientLight();
	glm::vec3 GetBaseVector();
};

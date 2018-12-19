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
	ParallelLight(glm::vec3 to);
	~ParallelLight();
	glm::vec3 GetToVector();
	glm::vec3 GetfromVector();

};

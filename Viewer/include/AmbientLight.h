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
    glm::vec3 ambientLight;
	AmbientLight(glm::vec3 base);
	~AmbientLight();
	glm::vec3 GetBaseVector();
	glm::vec3 GetLocationAfterTrans();
    float AmbientLight::estAmbientColor(float K, float L);
};

#pragma once

#include <glm/glm.hpp>
//#include <memory>
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(const glm::vec3& position, const glm::vec3& color);
	virtual ~PointLight();
	glm::vec3& GetPosition();

	glm::vec3 PointLight::GetPositionAfterTrans();

private:
	glm::vec3 position;
};



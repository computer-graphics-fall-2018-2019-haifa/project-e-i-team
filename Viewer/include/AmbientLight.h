#pragma once

#include <glm/glm.hpp>
#include "Light.h"

class AmbientLight : public Light 
{
public:
	AmbientLight(const glm::vec3& position, const glm::vec3& color);
	virtual ~AmbientLight();
	glm::vec3& GetPosition();

	glm::vec3 GetPositionAfterTrans();

private:
	glm::vec3 position;

};

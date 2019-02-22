#pragma once

#include <glm/glm.hpp>
//#include <memory>
#include "Light.h"

class Parallellight : public Light
{
public:
	Parallellight(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color);
	virtual ~Parallellight();
	//glm::vec3& GetPosition();

	glm::vec3 GetDirectionAfterTrans();

private:
	glm::vec3 from;
	glm::vec3 to;
};
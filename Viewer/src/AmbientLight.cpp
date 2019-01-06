#define _USE_MATH_DEFINES

#include "Trans.h"
#include "AmbientLight.h"
#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

glm::vec3& AmbientLight::GetBaseVector() {
	return base;
}

AmbientLight::AmbientLight(glm::vec3 base) :
    MeshModel(),
    base(base),
    Ka(1.0f), La(1.0f)
{
	resetModel(200, false, false, true, glm::vec4(0, 0, 0, 1), glm::vec4(0, 0, 0, 1),&glm::vec3(0, 0, 0), 20, 20);
}

glm::vec3& AmbientLight::GetLocationAfterTrans() {
	glm::vec4 base4(base.x, base.y, base.z, 1);
	glm::vec4 location = GetWorldTransformation() * base4;
	return glm::vec3(location.x, location.y, location.z);
}

AmbientLight::~AmbientLight() {}
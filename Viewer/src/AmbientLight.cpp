#define _USE_MATH_DEFINES

#include "Trans.h"
#include "AmbientLight.h"
#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

glm::vec3 AmbientLight::GetBaseVector() {
	return base;
}

AmbientLight::AmbientLight(glm::vec3 base) :
	base(base)
{
	resetModel(200, false, false, true, glm::vec4(0, 0, 0, 1), glm::vec4(0, 0, 0, 1),
		&glm::vec3(0, 0, 0), 20, 20);
}

AmbientLight::~AmbientLight() {}
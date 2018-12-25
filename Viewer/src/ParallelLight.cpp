

#define _USE_MATH_DEFINES

#include "Trans.h"
#include "ParallelLight.h"
#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>



glm::vec3 ParallelLight::GetToVector() {
	return to;
}

glm::vec3 ParallelLight::GetfromVector() {
	return from;
}

ParallelLight::ParallelLight(glm::vec3 to) :
	to(to),
	from(glm::vec3(0, 0, 0)),
    La(0.8f), Ld(0.8f), Ls(0.8f)
{
	resetModel(200, false, false, true, glm::vec4(0, 0, 0, 1), glm::vec4(0, 0, 0, 1), &glm::vec3(0, 0, 0), 20, 20);
}

glm::vec3 ParallelLight::GetLocationAfterTrans() {
	glm::vec4 from4(from.x, from.y, from.z, 1);
	glm::vec4 location = GetWorldTransformation() * from4;
	return glm::vec3(location.x, location.y, location.z);
}

ParallelLight::~ParallelLight() {}
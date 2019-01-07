

#define _USE_MATH_DEFINES

#include "Trans.h"
#include "ParallelLight.h"
#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>



glm::vec3& ParallelLight::GetToVector() {
	return to;
}

glm::vec3& ParallelLight::GetfromVector() {
	return from;
}

ParallelLight::ParallelLight(glm::vec3 to) :
    MeshModel(),
    to(to),
    from(glm::vec3(0, 0, 0)),
    Ld(1.0f), Ls(1.0f)
{
	resetModel(200, false, false, true, glm::vec4(0, 0, 0, 1), glm::vec4(0, 0, 0, 1), &glm::vec3(0, 0, 0), 20, 20);
}

glm::vec3& ParallelLight::GetLocationAfterTrans() {
	glm::vec4 from4(from, 1);
	glm::vec4 location = GetWorldTransformation() * from4;
	return glm::vec3(location.x, location.y, location.z) / location.w;
}

glm::vec3& ParallelLight::GetDirectionAfterTrans() {
	//cout << "to = (" << to.x << "," << to.y << "," << to.z << ")" << endl;
	//cout << "from = (" << from.x << "," << from.y << "," << from.z << ")" << endl;
	glm::vec3 direction = to - from;

	glm::vec4 direction4(direction, 1);
	direction4 = GetWorldTransformation() * direction4;
	direction4 = direction4 / direction4.w;
	
	return glm::vec3(direction4.x, direction4.y, direction4.z);
}

ParallelLight::~ParallelLight() {}
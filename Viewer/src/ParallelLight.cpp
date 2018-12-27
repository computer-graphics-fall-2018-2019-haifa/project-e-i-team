

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
	Ld(1.0f), Ls(1.0f)
{
	resetModel(200, false, false, true, glm::vec4(0, 0, 0, 1), glm::vec4(0, 0, 0, 1), &glm::vec3(0, 0, 0), 20, 20);

	/*
	glm::vec3 zero(0, 0, 0);
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices , normals;
	vertices.push_back(from);
	vertices.push_back(to);
	MeshModel({}, vertices, {}, zero, zero, zero, "Parallel Light");*/
}

glm::vec3 ParallelLight::GetLocationAfterTrans() {
	glm::vec4 from4(from.x, from.y, from.z, 1);
	glm::vec4 location = GetWorldTransformation() * from4;
	return glm::vec3(location.x, location.y, location.z);
}

glm::vec3 ParallelLight::GetDirectionAfterTrans() {
	glm::vec4 from4(from.x, from.y, from.z, 1);
	glm::vec4 from_location = GetWorldTransformation() * from4;

	glm::vec4 to4(to.x, to.y, to.z, 1);
	glm::vec4 to_location = GetWorldTransformation() * to4;

	glm::vec4 direction = to_location - from_location;

	return glm::vec3(direction.x, direction.y, direction.z);
}

ParallelLight::~ParallelLight() {}
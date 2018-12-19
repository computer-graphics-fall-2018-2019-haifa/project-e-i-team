

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
	from(glm::vec3(0, 0, 0))
{
	SetWorldTransformation(glm::mat4(1));

	/*
	glm::vec3 zero(0, 0, 0);
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices , normals;
	vertices.push_back(from);
	vertices.push_back(to);
	MeshModel({}, vertices, {}, zero, zero, zero, "Parallel Light");*/
}

ParallelLight::~ParallelLight() {}
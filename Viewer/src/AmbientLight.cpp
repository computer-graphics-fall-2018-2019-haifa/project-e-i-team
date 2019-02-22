#include "AmbientLight.h"
#include "PointLight.h"
#include <algorithm>

#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

#include <nfd.h>

#include <stdio.h>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <sstream>
#include <stdlib.h>



AmbientLight::AmbientLight(const glm::vec3& position, const glm::vec3& color) :
	position(position), Light(color)
{
}


AmbientLight::~AmbientLight()
{
}

glm::vec3& AmbientLight::GetPosition()
{
	return position;
}


glm::vec3 AmbientLight::GetPositionAfterTrans() {
	glm::vec4 Pos(position, 1.0f);
	Pos = GetWorldTransformation() * GetModelTransformation() * Pos;
	return (glm::vec3(Pos) / Pos.w);
}
#include "Parallellight.h"
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



Parallellight::Parallellight(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color) :
	from(from), to(to), Light(color)
{
}


Parallellight::~Parallellight()
{
}



glm::vec3 Parallellight::GetDirectionAfterTrans() {
	glm::vec4 from4(from, 1);
	glm::vec4 from_location = GetWorldTransformation() * GetModelTransformation() * from4;
	from_location = from_location / from_location.w;

	glm::vec4 to4(to, 1);
	glm::vec4 to_location = GetWorldTransformation() * GetModelTransformation() * to4;
	to_location = to_location / to_location.w;

	glm::vec4 direction = to_location - from_location;
	//cout << "dir para:" << direction.x << "," << direction.y << "," << direction.z << endl;
	return glm::vec3(direction);
}
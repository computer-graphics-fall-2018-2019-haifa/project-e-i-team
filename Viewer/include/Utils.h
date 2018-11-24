#pragma once
#include <glm/glm.hpp>
#include <string>
#include "MeshModel.h"
#include <direct.h>
#include <windows.h>

#define PROJECT_NAME "project-e-i-team"

#define Get_Root_Project_Dir(X) std::string(_getcwd(NULL, 0)).substr(0, std::string(_getcwd(NULL, 0)).find(PROJECT_NAME) + sizeof(PROJECT_NAME)) + X;

/*
 * Utils class.
 * This class is consisted of static helper methods that can have many clients across the code.
 */
class Utils
{
public:
	static std::string Utils::GetRootProjectDir();
	static glm::vec3 Vec3fFromStream(std::istream& issLine);
	static glm::vec2 Vec2fFromStream(std::istream& issLine);
	static MeshModel LoadMeshModel(const std::string& filePath);
	static MeshModel Utils::LoadGridModel();
	// Add here more static utility functions...
	// For example:
	//	1. function that gets an angle, and returns a rotation matrix around a certian axis
	//	2. function that gets a vector, and returns a translation matrix
	//	3. anything else that may be useful in global scope

private:
	static std::string GetFileName(const std::string& filePath);
};

#include "Utils.h"
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

MeshModel Utils::LoadGridModel() {

	std::vector<Face> grid_faces;
	std::vector<glm::vec3> grid_vertices;

	int counter = 0;
	for (int y = -10; y < 10; y++) {
		for (int x = -9; x < 11; x++) {

			glm::vec3 v1 = glm::vec3(x - 1, y, 0);
			glm::vec3 v2 = glm::vec3(x, y, 0);
			glm::vec3 v3 = glm::vec3(x - 1, y + 1, 0);
			glm::vec3 v4 = glm::vec3(x, y + 1, 0);

			v1 *= 30;
			v2 *= 30;
			v3 *= 30;
			v4 *= 30;


			grid_vertices.push_back(v1);
			grid_vertices.push_back(v2);
			grid_vertices.push_back(v3);
			grid_vertices.push_back(v4);

			std::vector<int> vertexIndices = { 1 + counter  , 2 + counter ,3 + counter ,4 + counter };

			grid_faces.push_back(vertexIndices);

			counter += 4;
		}
	}
	return MeshModel(grid_faces, grid_vertices, grid_vertices, "Grid");
}


MeshModel Utils::LoadMeshModel(const std::string& filePath)
{
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::ifstream ifile(filePath.c_str());
	

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		std::getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		
		// based on the type parse data
		if (lineType == "v")
		{
			vertices.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vn")
		{
			// Add the required code here...
			normals.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			// Texture coordinates
		}
		else if (lineType == "f")
		{
			faces.push_back(Face(issLine));
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}


	return MeshModel(faces, vertices, normals, Utils::GetFileName(filePath));
}

std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}
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
	for (int y = -9; y < 9; y++) {
		for (int x = -8; x < 10; x++) {

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
	return MeshModel(grid_faces, grid_vertices, grid_vertices,glm::vec3(0,0,0) , glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), "Grid");
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


	

	float min_x = 2000, min_y = 2000, min_z = 2000;
	float max_x = -2000, max_y = -2000, max_z = -2000;

	for (auto face = faces.begin(); face != faces.end(); ++face) {
		glm::vec3 modelVec;
		for (int i = 0; i < 3; i++) {

			int v = face->GetVertexIndex(i) - 1;
			modelVec = vertices[v];
			
			float x = modelVec.x;
			float y = modelVec.y;
			float z = modelVec.z;

			if (x < min_x) min_x = x;
			if (y < min_y) min_y = y;
			if (z < min_z) min_z = z;

			if (x > max_x) max_x = x;
			if (y > max_y) max_y = y;
			if (z > max_z) max_z = z;
		}
	}
	glm::vec3 BoundMin(min_x , min_y , min_z);
	glm::vec3 BoundMax(max_x , max_y , max_z);
	glm::vec3 BoundMiddle( (min_x + max_x) / 2, (min_y + max_y) / 2, (min_z + max_z) / 2);
	

	return MeshModel(faces, vertices, normals, BoundMin, BoundMax, BoundMiddle, Utils::GetFileName(filePath));
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
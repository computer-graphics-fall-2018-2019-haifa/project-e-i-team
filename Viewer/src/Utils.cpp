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
    std::vector<glm::vec2> textureCoords;

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
	return MeshModel(grid_faces, grid_vertices, grid_vertices, textureCoords,glm::vec3(0 ,0 ,0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), "Grid");
}

MeshModel Utils::LoadLightSource() {
	static int Light_counter = -1;
	std::vector<Face> Light_faces;
	std::vector<glm::vec3> Light_vertices;
	std::vector<glm::vec3> Light_normals;
    std::vector<glm::vec2> textureCoords;
	int d = 1;
	int n = 2 * d;

	Light_vertices.push_back(glm::vec3(d, d, d));	//1
	Light_vertices.push_back(glm::vec3(d, d, -d));	//2
	Light_vertices.push_back(glm::vec3(d, -d, d));	//3
	Light_vertices.push_back(glm::vec3(d, -d, -d));	//4
	Light_vertices.push_back(glm::vec3(-d, d, d));	//5
	Light_vertices.push_back(glm::vec3(-d, d, -d));	//6
	Light_vertices.push_back(glm::vec3(-d, -d, d));	//7
	Light_vertices.push_back(glm::vec3(-d, -d, -d));//8

	Light_normals.push_back(glm::vec3(n, n, n));	//1
	Light_normals.push_back(glm::vec3(n, n, -n));	//2
	Light_normals.push_back(glm::vec3(n, -n, n));	//3
	Light_normals.push_back(glm::vec3(n, -n, -n));	//4
	Light_normals.push_back(glm::vec3(-n, n, n));	//5
	Light_normals.push_back(glm::vec3(-n, n, -n));	//6
	Light_normals.push_back(glm::vec3(-n, -n, n));	//7
	Light_normals.push_back(glm::vec3(-n, -n, -n));//8
	
	std::vector<int> vertexIndices;

	
	vertexIndices = { 1 , 2 , 6};
	Face f1(vertexIndices, vertexIndices);
	Light_faces.push_back(f1);
	vertexIndices = { 1  , 5   , 6};
	Face f2(vertexIndices, vertexIndices);
	Light_faces.push_back(f2);

	vertexIndices = { 5    , 6 , 8 };
	Face f3(vertexIndices, vertexIndices);
	Light_faces.push_back(f3);
	vertexIndices = { 5 , 7  , 8 };
	Face f4(vertexIndices, vertexIndices);
	Light_faces.push_back(f4);

	vertexIndices = { 1 , 5  , 7 };
	Face f5(vertexIndices, vertexIndices);
	Light_faces.push_back(f5);
	vertexIndices = { 1  , 3 , 7 };
	Face f6(vertexIndices, vertexIndices);
	Light_faces.push_back(f6);

	vertexIndices = { 1  , 2  , 4 };
	Face f7(vertexIndices, vertexIndices);
	Light_faces.push_back(f7);
	vertexIndices = { 1 , 3 , 4 };
	Face f8(vertexIndices, vertexIndices);
	Light_faces.push_back(f8);

	vertexIndices = { 2 , 4  , 6 };
	Face f9(vertexIndices, vertexIndices);
	Light_faces.push_back(f9);
	vertexIndices = { 4, 6  , 8 };
	Face f10(vertexIndices, vertexIndices);
	Light_faces.push_back(f10);

	vertexIndices = { 3   , 4  , 7 };
	Face f11(vertexIndices, vertexIndices);
	Light_faces.push_back(f11);
	vertexIndices = { 4   , 7  , 8 };
	Face f12(vertexIndices, vertexIndices);
	Light_faces.push_back(f12);

	glm::vec3 BoundMin(-d, -d, -d);
	glm::vec3 BoundMax(d, d, d);
	glm::vec3 BoundMiddle(0, 0, 0);

	
	Light_counter++;
	return MeshModel(Light_faces, Light_vertices, Light_normals, textureCoords, BoundMin, BoundMax, BoundMiddle, "Light Source "+ Light_counter);
}


MeshModel Utils::LoadMeshModel(const std::string& filePath)
{
	std::vector<Face> faces;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textureCoords;
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
            textureCoords.push_back(Utils::Vec2fFromStream(issLine));
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
	
	return MeshModel(faces, vertices, normals, textureCoords,BoundMin, BoundMax, BoundMiddle, Utils::GetFileName(filePath));
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
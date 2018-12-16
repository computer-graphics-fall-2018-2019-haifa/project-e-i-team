#pragma once
#include <glm/glm.hpp>

class Trans {
	float deg2rad(float degrees) { return degrees * 0.0174532925; }

public:
	static glm::mat4x4 getScale4x4(float factor) {
		glm::mat4x4 m = glm::mat4x4(factor);
		m[3][3] = 1;
		return m;
	}
	static glm::mat4x4 getZoom(float factor) {
		return glm::mat4x4(glm::highp_mat4::col_type(1.0f, 0.0f, 0.0f, 0.0f),
			glm::highp_mat4::col_type(0.0f, 1.0f, 0.0f, 0.0f),
			glm::highp_mat4::col_type(0.0f, 0.0f, factor, 0.0f),
			glm::highp_mat4::col_type(0.0f, 0.0f, 0.0f, 1.0f));
	}
	static glm::mat2x2 getxRotate2x2(float teta) {
		return glm::mat2x2(glm::highp_mat2::col_type(cos(teta), (-1)*sin(teta)),
			glm::highp_mat2::col_type(sin(teta), cos(teta)));
	}
	static glm::mat4x4 getxRotate4x4(float teta) {
		return glm::mat4x4(glm::highp_mat4::col_type(1.0f, 0.0f, 0.0f, 0.0f),
			glm::highp_mat4::col_type(0.0f, cos(teta), (-1)*sin(teta), 0.0f),
			glm::highp_mat4::col_type(0.0f, sin(teta), cos(teta), 0.0f),
			glm::highp_mat4::col_type(0.0f, 0.0f, 0.0f, 1.0f));
	}
	static glm::mat4x4 getyRotate4x4(float teta) {
		return glm::mat4x4(glm::highp_mat4::col_type(cos(teta), 0.0f, (-1)*sin(teta), 0.0f),
			glm::highp_mat4::col_type(0.0f, 1.0f, 0.0f, 0.0f),
			glm::highp_mat4::col_type(sin(teta), 0.0f, cos(teta), 0.0f),
			glm::highp_mat4::col_type(0.0f, 0.0f, 0.0f, 1.0f));
	}
	static glm::mat4x4 getzRotate4x4(float teta) {
		return glm::mat4x4(glm::highp_mat4::col_type(cos(teta), (-1)*sin(teta), 0.0f, 0.0f),
			glm::highp_mat4::col_type(sin(teta), cos(teta), 0.0f, 0.0f),
			glm::highp_mat4::col_type(0.0f, 0.0f, 1.0f, 0.0f),
			glm::highp_mat4::col_type(0.0f, 0.0f, 0.0f, 1.0f));
	}
	static glm::mat4x4 getTranslate4x4(float tx, float ty,float tz) {
		return glm::mat4x4(glm::highp_mat4::col_type(1.0f, 0.0f, 0.0f, 0.0f),
			glm::highp_mat4::col_type(0.0f, 1.0f, 0.0f, 0.0f),
			glm::highp_mat4::col_type(0.0f, 0.0f, 1.0f, 0.0f),
			glm::highp_mat4::col_type(tx,ty, tz, 1.0f));
	}
	static glm::mat4x4& get2InitAxis4x4(glm::vec3& massCenter,glm::mat4x4 T) {
		glm::mat4x4 toZero = Trans::getTranslate4x4(-massCenter.x, -massCenter.y, -massCenter.z);
		glm::mat4x4 toOrigin = Trans::getTranslate4x4(massCenter.x, massCenter.y, massCenter.z);
		return (toOrigin * T * toZero);
	}
};

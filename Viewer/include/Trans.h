#pragma once
#include <glm/glm.hpp>
#include <stdio.h>
#include <iostream>
#include <vector>

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
    static void buildGaussianKernel(float kernel[5][5],int m,int n,int radius) {
        int _M = -(m - 1) / 2;
        int M = (m - 1) / 2;
        int _N = -(n - 1) / 2;
        int N = (n - 1) / 2;
        for (int i = _M; i <= M;i++) {
            for (int j = _N; j <= N; j++) {
                kernel[i + M][j + N] = expf((-pow(i, 2) - pow(j, 2)) / (2.0f * pow(radius,2)));
            }
        }
    }
    static int INDEXCOLOR(int width, int x, int y, int c) {
        return ((x)+(y)*(width)) * 3 + (c);
    }
    static void convolve(float* image ,int viewportWidth,int viewportHeight, float kernel[5][5],int m,int n) {
        for (int i = 0; i < viewportWidth; i++) {
            for (int j = 0; j < viewportHeight; j++) {
                int indexR = INDEXCOLOR(viewportWidth, i, j, 0),indexG = INDEXCOLOR(viewportWidth, i, j, 1), indexB = INDEXCOLOR(viewportWidth, i, j, 2);
                float convRSummation = 0.0f, convGSummation = 0.0f, convBSummation = 0.0f;
                for (int r0 = 0; r0 <= n; r0++) {
                    for (int r1 = 0; r1 <= m; r1++) {
                        float gaussValue = 0;
                        int disRow = (i - r0), disCol = (j - r1);
                        if ((i - (m - 1) / 2) >= 0 && (i + (m - 1) / 2) < viewportWidth &&
                            (j - (n - 1) / 2) >= 0 && (j + (n - 1) / 2) < viewportHeight) {
                            gaussValue = kernel[r0][r1];
                            int R, G, B;
                            if (disRow >= 0 && disCol >= 0) {
                                R = INDEXCOLOR(viewportWidth, i + disRow, j + disCol, 0);
                                B = INDEXCOLOR(viewportWidth, i + disRow, j + disCol, 1);
                                G = INDEXCOLOR(viewportWidth, i + disRow, j + disCol, 2);
                            }
                            else if (disRow < 0 && disCol < 0) {
                                R = INDEXCOLOR(viewportWidth, i - disRow, j - disCol, 0);
                                B = INDEXCOLOR(viewportWidth, i - disRow, j - disCol, 1);
                                G = INDEXCOLOR(viewportWidth, i - disRow, j - disCol, 2);
                            }
                            else if (disRow < 0) {
                                R = INDEXCOLOR(viewportWidth, i - disRow, j + disCol, 0);
                                B = INDEXCOLOR(viewportWidth, i - disRow, j + disCol, 1);
                                G = INDEXCOLOR(viewportWidth, i - disRow, j + disCol, 2);
                            }
                            else if (disCol < 0) {
                                R = INDEXCOLOR(viewportWidth, i + disRow, j - disCol, 0);
                                B = INDEXCOLOR(viewportWidth, i + disRow, j - disCol, 1);
                                G = INDEXCOLOR(viewportWidth, i + disRow, j - disCol, 2);
                            }
                            convRSummation += gaussValue * R;
                            convGSummation += gaussValue * G;
                            convBSummation += gaussValue * B;
                        }
                    }
                }
                image[indexR] = convRSummation;
                image[indexG] = convGSummation;
                image[indexB] = convBSummation;
            }
        }
    }
    static void thresh(float* image, int viewportWidth, int viewportHeight,int th){
        for (int i = 0; i < viewportWidth;i++) {
            for (int j = 0; j < viewportHeight;j++) {
                if (image[INDEXCOLOR(viewportWidth, i, j, 0)] < th) {
                    image[INDEXCOLOR(viewportWidth, i, j, 0)] = 0.0f;
                }
                if (image[INDEXCOLOR(viewportWidth, i, j, 1)] < th) {
                    image[INDEXCOLOR(viewportWidth, i, j, 1)] = 0.0f;
                }
                if (image[INDEXCOLOR(viewportWidth, i, j, 2)] < th) {
                    image[INDEXCOLOR(viewportWidth, i, j, 2)] = 0.0f;
                }
            }

        }
    }
};

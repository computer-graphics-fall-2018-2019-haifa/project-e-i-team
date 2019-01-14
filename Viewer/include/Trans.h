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
    static void buildGaussianKernel3x3(float kernel[3][3], int m, int n, int radius) {
        int _M = -(m - 1) / 2;
        int M = (m - 1) / 2;
        int _N = -(n - 1) / 2;
        int N = (n - 1) / 2;
        float sum = 0.0f;
        for (int i = _N; i <= N; i++) {
            for (int j = _M; j <= M; j++) {
                kernel[i + M][j + N] = expf((-pow(i, 2) - pow(j, 2)) / (2.0f * pow(radius, 2)));
                sum += kernel[i + M][j + N];
            }
        }
        for (int i = _N; i <= N; i++) {
            for (int j = _M; j <= M; j++) {
                kernel[i + M][j + N] = (1.0f / sum)*kernel[i + N][j + M];
            }
        }
    }
    static void buildGaussianKernel5x5(float kernel[5][5],int m,int n,int radius) {
        int _M = -(m - 1) / 2;
        int M = (m - 1) / 2;
        int _N = -(n - 1) / 2;
        int N = (n - 1) / 2;
        float sum = 0.0f;
        for (int i = _N; i <= N;i++) {
            for (int j = _M; j <= M; j++) {
                kernel[i + M][j + N] = expf((-pow(i, 2) - pow(j, 2)) / (2.0f * pow(radius,2)));
                sum += kernel[i + M][j + N];
            }
        }
        for (int i = _N; i <= N; i++) {
            for (int j = _M; j <= M; j++) {
                kernel[i + M][j + N] = (1.0f / sum)*kernel[i + N][j + M];
            }
        }
    }
    static void buildGaussianKernel10x10(float kernel[10][10], int m, int n, int radius) {
        int _M = -(m - 1) / 2;
        int M = (m - 1) / 2;
        int _N = -(n - 1) / 2;
        int N = (n - 1) / 2;
        float sum = 0.0f;
        for (int i = _N; i <= N; i++) {
            for (int j = _M; j <= M; j++) {
                kernel[i + M][j + N] = expf((-pow(i, 2) - pow(j, 2)) / (2.0f * pow(radius, 2)));
                sum += kernel[i + M][j + N];
            }
        }
        for (int i = _N; i <= N; i++) {
            for (int j = _M; j <= M; j++) {
                kernel[i + M][j + N] = (1.0f / sum)*kernel[i + N][j + M];
            }
        }
    }
    static int INDEXCOLOR(int width, int x, int y, int c) {
        return ((x)+(y)*(width)) * 3 + (c);
    }
    static void convolve3x3(float* image, int viewportWidth, int viewportHeight, float kernel[3][3], int m, int n) {
        for (int width = 0; width < viewportWidth; width++) {
            for (int height = 0; height < viewportHeight; height++) {
                float convRSum = 0.0f, convGSum = 0.0f, convBSum = 0.0f;
                for (int col = 0; col < n; col++) {
                    for (int row = 0; row < m; row++) {
                        int wLoc = width + n - col - ((n - 1) / 2), hLoc = height + m - row - ((m - 1) / 2);
                        if ((wLoc >= 0) && (wLoc < viewportWidth) && (hLoc >= 0) && (hLoc < viewportHeight)) {
                            float R = kernel[col][row] * image[INDEXCOLOR(viewportWidth, wLoc, hLoc, 0)];
                            float G = kernel[col][row] * image[INDEXCOLOR(viewportWidth, wLoc, hLoc, 1)];
                            float B = kernel[col][row] * image[INDEXCOLOR(viewportWidth, wLoc, hLoc, 2)];
                            convRSum += R;
                            convGSum += G;
                            convBSum += B;
                        }
                    }
                }
                image[INDEXCOLOR(viewportWidth, width, height, 0)] = convRSum;
                image[INDEXCOLOR(viewportWidth, width, height, 1)] = convGSum;
                image[INDEXCOLOR(viewportWidth, width, height, 2)] = convBSum;
            }
        }
    }
    static void convolve5x5(float* image ,int viewportWidth,int viewportHeight, float kernel[5][5],int m,int n) {
        for (int width = 0; width < viewportWidth; width++) {
            for (int height = 0; height < viewportHeight; height++) {
                float convRSum = 0.0f, convGSum = 0.0f, convBSum = 0.0f;
                for (int col = 0; col < n; col++) {
                    for (int row = 0; row < m; row++) {
                        int wLoc = width + n - col - ((n - 1) / 2), hLoc = height + m - row - ((m - 1) / 2);
                        if ((wLoc >= 0) && (wLoc < viewportWidth) && (hLoc >= 0) && (hLoc < viewportHeight)) {
                            float R = kernel[col][row] * image[INDEXCOLOR(viewportWidth, wLoc, hLoc, 0)];
                            float G = kernel[col][row] * image[INDEXCOLOR(viewportWidth, wLoc, hLoc, 1)];
                            float B = kernel[col][row] * image[INDEXCOLOR(viewportWidth, wLoc, hLoc, 2)];
                            convRSum += R;
                            convGSum += G;
                            convBSum += B;
                        }
                    }
                }
                image[INDEXCOLOR(viewportWidth, width, height, 0)] = convRSum;
                image[INDEXCOLOR(viewportWidth, width, height, 1)] = convGSum;
                image[INDEXCOLOR(viewportWidth, width, height, 2)] = convBSum;
            }
        }
    }
    static void convolve10x10(float* image, int viewportWidth, int viewportHeight, float kernel[10][10], int m, int n) {
        for (int width = 0; width < viewportWidth; width++) {
            for (int height = 0; height < viewportHeight; height++) {
                float convRSum = 0.0f, convGSum = 0.0f, convBSum = 0.0f;
                for (int col = 0; col < n; col++) {
                    for (int row = 0; row < m; row++) {
                        int wLoc = width + n - col - ((n - 1) / 2), hLoc = height + m - row - ((m - 1) / 2);
                        if ((wLoc >= 0) && (wLoc < viewportWidth) && (hLoc >= 0) && (hLoc < viewportHeight)) {
                            float R = kernel[col][row] * image[INDEXCOLOR(viewportWidth, wLoc, hLoc, 0)];
                            float G = kernel[col][row] * image[INDEXCOLOR(viewportWidth, wLoc, hLoc, 1)];
                            float B = kernel[col][row] * image[INDEXCOLOR(viewportWidth, wLoc, hLoc, 2)];
                            convRSum += R;
                            convGSum += G;
                            convBSum += B;
                        }
                    }
                }
                image[INDEXCOLOR(viewportWidth, width, height, 0)] = convRSum;
                image[INDEXCOLOR(viewportWidth, width, height, 1)] = convGSum;
                image[INDEXCOLOR(viewportWidth, width, height, 2)] = convBSum;
            }
        }
    }
    static void thresh(float* image, int viewportWidth, int viewportHeight,int th){
        for (int i = 0; i < viewportWidth;i++) {
            for (int j = 0; j < viewportHeight;j++) {
                if (image[INDEXCOLOR(viewportWidth, i, j, 0)] < th * 256) {
                    image[INDEXCOLOR(viewportWidth, i, j, 0)] = 0.0f;
                }
                if (image[INDEXCOLOR(viewportWidth, i, j, 1)] < th * 256) {
                    image[INDEXCOLOR(viewportWidth, i, j, 1)] = 0.0f;
                }
                if (image[INDEXCOLOR(viewportWidth, i, j, 2)] < th * 256) {
                    image[INDEXCOLOR(viewportWidth, i, j, 2)] = 0.0f;
                }
            }

        }
    }
};

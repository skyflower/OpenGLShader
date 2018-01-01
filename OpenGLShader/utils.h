#pragma once
#include <fstream>
#include <cstring>
#include <iostream>

#define STATIC_GLEW
#include <GL/glew.h>

#define GLUT_BUILDING_LIB
#include <GL/glut.h>

#include <functional>
#include "common.h"

namespace utils
{
	float randf(); // [0, 1]
	float srandf();// [-1, 1]

	//CTexture* CreateTexture(size_t nSize);
	//  error: return nullptr,Length reutrn -1
	//  success: return file content,Length
	char *LoadFileContent(const char *FilePath, size_t &Length);

	GLuint CaptureScreen(int width, int height, std::function<void()> foo);

	//GLuint CreateGPUProgram(const char*vsPath, const char*fsPath);

	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, std::string Msg);

	GLuint CreateBufferObject(GLenum bufferType, size_t Size, void *data, GLenum usage = GL_STATIC_DRAW);


	GLuint CreateVertexArrayObj(std::function<void()> settings);

	GLuint CreateFrameBufferObj(GLuint &colorBuffer, int width, int height, GLuint &depthBuffer, GLuint *colorBufferTwo = nullptr);

	void OutputToFile(std::fstream &log, mat4f & ks, glm::mat4 &hs);

	void TestMatrixGLM();

	GLuint CreateComputeProgram(const char*computeShaderPath);

	bool CreateComputeTexture(const char*shaderPath, const char*imagePath, GLuint &inputTexture, GLuint &outputTexture);

	double *Logistic(float a, float x, size_t N);

	GLuint CreateTexture3D(int w, int h, int d);

	GLuint CreateTFOProgram(const char*vsFile, const char*attrib[], int nCount, GLenum memoryFormat, const char *gsFile = nullptr);

	GLuint CreateTransformFeedBack(GLuint &tfoBuffer, size_t Size, void *data, GLenum usage = GL_STATIC_DRAW);
	
	void CheckError(const char*file, const char *func, const int line);

	bool CutLogBySpace(const char*begin, const char *end, int *tmpArray, size_t &nCount, char cutChar);
}


//GLuint colorBuffer = -1;
//GLuint depthBuffer = -1;
//GLuint colorBufferTwo = -1;

//GLuint fbo = utils::CreateFrameBufferObj(colorBuffer, viewWidth, viewHeight, depthBuffer, nullptr);
//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//glBindFramebuffer(GL_FRAMEBUFFER, 0);


/*glUseProgram(program);
glBindImageTexture(0, mainTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
glBindImageTexture(1, computeDstTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
glDispatchCompute(imageWidth / 16, imageHeight / 16, 1);
glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);*/


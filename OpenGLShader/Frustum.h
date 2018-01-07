#pragma once


#include <iostream>

#define STATIC_GLEW
#include <GL/glew.h>

#define GLUT_BUILDING_LIB
#include <GL/glut.h>

#include "./common/VertexData.h"
#include <vector>
#include "./common/common.h"
#include "./common/utils.h"
#include "./common/shader.h"
#include "Drawable.h"
#include "C3DModel.h"

class CFrustum : public C3DModel
{
public:
	CFrustum();
	~CFrustum();
	void Init(double alpha, double ratio, double zNear, double zFar);
	void Init(vec4f &param);
	void Init(float left, float right, float bottom, float top, float zNear, float zFar);
	void Init(CVector<6, float> &rhs);
	virtual void Draw();
	virtual void Update(float duration);
	
	void TestShader(mat4f model);
private:
	//void InitShaderProgram();
	void InitShaderProgram(float *points, size_t PointCount, GLubyte *index, size_t indexLength);
	//GLuint m_nProgram;
	//GLuint m_nPosLocation;
	CShader *m_pShader;
	//GLuint m_nMVPLocation;
	GLuint m_nVertexBufferID;
	GLuint m_nIndexBufferID;
	//mat4f m_fMVPMatrix;
	//vec3f m_fTrans[3];
	size_t m_nCount;
	//std::vector<float> *m_pData;
	//std::vector<GLubyte> *m_pIndex;
};


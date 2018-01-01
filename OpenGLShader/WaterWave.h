#pragma once

#include <iostream>

#define STATIC_GLEW
#include <GL/glew.h>

#define GLUT_BUILDING_LIB
#include <GL/glut.h>

#include "VertexData.h"
#include <vector>
#include "common.h"
#include "utils.h"
#include "Texure.h"
#include "shader.h"
#include "Drawable.h"
#include "C3DModel.h"


class CWaterWave : public C3DModel
{
public:
	CWaterWave();
	~CWaterWave();
	
	void Init();
	void InitTexture(const char*pTextureFile);
	void InitDisplayBuffer();
	virtual void Draw();
	virtual void Update(float duration);
	//void SetTranslate(float x, float y, float z);
	//void SetScale(float x, float y, float z);
	//void SetRotate(float AngleX, float AngleY, float AngleZ);
	//void SetMatrix(mat4f& ModelMatrix, mat4f& ViewMatrix, mat4f& ProjectionMatrix);
	//mat4f GetModel();
	GLuint GetShaderProgram();
	void TestShader(mat4f model);
	
private:
	void ResetState();
	void SetVertexAttrib(AttribType type);
	std::vector<CVertexData> *m_data;
	std::vector<unsigned int> *m_pIndex;

	CShader* m_pShader;


	CTexture *m_pTexture;
	GLuint m_nParamLocation;
	vec4f m_fParam;
	GLuint m_nModelMatrixLocation;
	GLuint m_nViewMatrixLocation;
	

	GLuint m_nVertexBufferObj;
	GLuint m_nIndexBufferObj;
	
	
	//bool m_bInstanced;
};


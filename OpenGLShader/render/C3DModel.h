#pragma once
#include "../graphics/Drawable.h"
#include "../common/common.h"
#include <iostream>

#include "../common/Texure.h"
#include "../common/shader.h"
#include "../common/VertexData.h"
#include <memory>


class C3DModel :
	public CDrawable
{
public:
	C3DModel();
	virtual ~C3DModel();

	virtual void Draw() = 0;
	virtual void Update(float Duration) = 0;
	virtual void SetMatrix(mat4f & ViewMatrix, mat4f & ProjectionMatrix);

	virtual void SetScale(float x, float y, float z);
	virtual void SetRotate(float AngleX, float AngleY, float AngleZ);
	virtual void SetTranslate(float x, float y, float z);
	virtual void AddScale(float x, float y, float z);
	virtual void AddRotate(float AngleX, float AngleY, float AngleZ);
	virtual void AddTranslate(float x, float y, float z);

	virtual vec3f GetScale();
	virtual vec3f GetRotate();
	virtual vec3f GetTranslate();
	virtual void SetModelMatrix(mat4f &modelMatrix);

	void ResetStatus();
	void SetShaderProgram(GLuint program);
	void SetTransformMatrix();

	mat4f GetModel();
	mat4f GetViewMatrix();
	mat4f GetProjectionMatrix();
	mat4f GetMVPMatrix();

private:
	//GLuint m_nMVPLocation;
	GLuint m_nProjectionMatrixLocation;
	mat4f m_fProjectionMatrix;

	GLuint m_nModelMatrixLocation;
	GLuint m_nViewMatrixLocation;

	GLuint m_nNormalMatrixLocation;
	mat3f m_fNormalMatrix;

	//GLuint m_nRotateMatrix;
	//GLuint m_nTranslateMatrix;
	//GLuint m_nScaleMatrix;
	
	mat4f m_fViewMatrix;
	mat4f m_fModelMatrix;

	vec3f m_fTrans[3];	// unused

	GLuint m_nShaderProgram;
};


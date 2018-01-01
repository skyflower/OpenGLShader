#pragma once
#include "Drawable.h"
#include "common.h"
#include <iostream>

#include "Texure.h"
#include "shader.h"
#include "VertexData.h"


class C3DModel :
	public CDrawable
{
public:
	C3DModel();
	virtual ~C3DModel();

	virtual void Draw() = 0;
	virtual void Update(float Duration) = 0;
	virtual void SetMatrix(mat4f & ModelMatrix, mat4f & ViewMatrix, mat4f & ProjectionMatrix);

	virtual void SetScale(float x, float y, float z);
	virtual void SetRotate(float AngleX, float AngleY, float AngleZ);
	virtual void SetTranslate(float x, float y, float z);
	virtual void AddScale(float x, float y, float z);
	virtual void AddRotate(float AngleX, float AngleY, float AngleZ);
	virtual void AddTranslate(float x, float y, float z);
	virtual vec3f GetScale();
	virtual vec3f GetRotate();
	virtual vec3f GetTranslate();

	mat4f GetModel();
protected:
	GLuint m_nMVPLocation;
	mat4f m_fMVPMatrix;
	//mat4f m_fModelMatrix;
	//mat4f m_fViewMatrix;
	//mat4f m_fProjectionMatrix;
private:

	
	vec3f m_fTrans[3];
};


#pragma once
#include "C3DModel.h"
#include "./common/common.h"
#include "Sphere.h"
class CSkyBox :
	public C3DModel
{
public:
	CSkyBox();
	virtual ~CSkyBox();
	void InitTexture(const char * fileDir);
	virtual void Draw();
	virtual void Update(float Duration);

	GLuint GetShaderProgram();

private:
	void Init();
	void SetVertexAttrib(AttribType type);

	enum
	{
		TEXTURE_COUNT = 6,
		POINT_COUNT = 8
	};

	CTexture* m_pTexture[TEXTURE_COUNT];
	CShader* m_pShader;
	CSphere* m_pSphere;
	CVertexData m_pVertex[POINT_COUNT];
	GLuint m_nVertexBuffer;
};


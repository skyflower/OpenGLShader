#pragma once
#include "Drawable.h"
#include "common.h"
#include "Texure.h"
#include "shader.h"
#include "VertexData.h"
#include "C3DModel.h"

class CPointSprite :
	public C3DModel
{
public:
	CPointSprite();
	virtual ~CPointSprite();
	void InitTexture(const char *filepath);
	virtual void Draw();
	virtual void Update(float Duration);
	
	GLuint GetShaderProgram();
	
private:
	void Init();
	void SetVertexAttrib(AttribType type);
	void ResetState();

	CTexture* m_pTexture;
	CShader* m_pShader;

	//size_t m_nPointCount;
	GLuint m_nSSBOProgram;
	CVertex_SSBO_Two m_point;
};


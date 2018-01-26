#pragma once
#include "../render/C3DModel.h"
#include "../common/common.h"

class CGround :
	public C3DModel
{
public:
	CGround();
	
	virtual ~CGround();
	void InitTexture(const char * filepath);
	virtual void Draw();
	virtual void Update(float Duration);

	GLuint GetShaderProgram();

private:
	void Init();
	void SetVertexAttrib(AttribType type);

	CTexture* m_pTexture;
	CShader* m_pShader;
	CVertexData m_pVertex;
	GLuint m_nVertexBuffer;
};


#pragma once
#include "Drawable.h"
#include "common/common.h"
#include "common/Texure.h"
#include "common/shader.h"
#include "common/VertexData.h"
#include "C3DModel.h"

class CPolygonTess :
	public C3DModel
{
public:
	CPolygonTess();
	virtual ~CPolygonTess();
	void InitTexture(const char *filepath);
	virtual void Draw();
	virtual void Update(float Duration);
	
	GLuint GetShaderProgram();
	
private:
	void Init();
	void SetVertexAttrib(AttribType type);
	void TestShader(mat4f &matrix);

	//CTexture* m_pTexture;
	CShader* m_pShader;

	size_t m_nPointCount;
	GLuint m_nVertexBuffer;
	//GLuint m_nSSBOProgram;
	CVertex_SSBO_Two *m_pPoints;
};


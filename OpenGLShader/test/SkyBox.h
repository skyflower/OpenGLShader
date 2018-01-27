#pragma once
#include "../render/C3DModel.h"
#include "../common/common.h"
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
	void InitSphere(float, int, int);
	void SetVertexAttrib(AttribType type);
	void SetSphereVertexAttrib(AttribType type);

	enum
	{
		TEXTURE_COUNT = 6,
		POINT_COUNT = 24
	};

	CTexture* m_pTexture[TEXTURE_COUNT];
	CShader* m_pShader;
	
	std::vector<CVertex>* m_pSphereData;
	std::vector<unsigned int>* m_pSphereIndex;
	GLuint m_nSphereBuffer;
	GLuint m_nSphereIndex;

	CVertexData *m_pVertex;
	GLuint m_nVertexBuffer;
};


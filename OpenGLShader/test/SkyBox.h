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
	void DrawSkyBox();
	void DrawSphere();


	CShader* m_pShader;
	CShader* m_pBoxShader;

	CTexture* m_pCubeTexture;
	
	std::vector<CVertex>* m_pSphereData;
	std::vector<unsigned int>* m_pSphereIndex;
	GLuint m_nSphereBuffer;
	GLuint m_nSphereIndex;

	CVertexData *m_pVertex;
	size_t m_nCubicVertexCount;
	GLuint m_nVertexBuffer;
};


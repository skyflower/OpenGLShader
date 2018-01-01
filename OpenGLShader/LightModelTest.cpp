#include "LightModelTest.h"

#include <fstream>
#include <iostream>

extern std::fstream Log;

CLightTest::CLightTest() :C3DModel()
{
	m_data = nullptr;
	//m_pIndex = nullptr;
	m_pShader = nullptr;
	
	m_nVertexBufferObj = -1;
	
	m_pTexture = nullptr;
	//m_nLightModelLocation = 0;
	m_nNormalMatrixLocation = 0;
	m_nLightTypeLocation = 0;
	
	m_nIndexBufferObj = -1;
	

	//m_nSurfaceLocation = -1;
	
	Init();
}

CLightTest::~CLightTest()
{
	if (m_data != nullptr)
	{
		m_data->clear();
		delete m_data;
		m_data = nullptr;
	}
	/*if (m_pIndex != nullptr)
	{
		m_pIndex->clear();
		delete m_pIndex;
		m_pIndex = nullptr;
	}*/
	if (m_pShader != nullptr)
	{
		delete m_pShader;
		m_pShader = nullptr;
	}
	if (glIsBuffer(m_nVertexBufferObj))
	{
		glDeleteBuffers(1, &m_nVertexBufferObj);
		m_nVertexBufferObj = -1;
	}
	
	
	if (m_pTexture)
	{
		CTexture::UnLoadTexture(m_pTexture);
		m_pTexture = nullptr;
	}
}

void CLightTest::Init()
{
	size_t Length = 3;
	if (m_data == nullptr)
	{
		m_data = new std::vector<CVertexData>(Length);
	}
	CVertexData tmpData;
	//float tmpY = 1.66;
	//float tmpX = 2.21;//0.5
	tmpData.m_fPos[0] = -0.5;
	tmpData.m_fPos[1] = 0;// -0.15;// 0.5;
	tmpData.m_fPos[2] = -0.5f;
	tmpData.m_fNormal[0] = 0;
	tmpData.m_fNormal[1] = 1;
	tmpData.m_fNormal[2] = 0;

	tmpData.m_fTex[0] = 0.0f;
	tmpData.m_fTex[1] = 0.0f;
	(*m_data)[0] = tmpData;

	tmpData.m_fPos[0] = -0.5;
	tmpData.m_fPos[1] = 0;// -0.15;// 0.5;
	tmpData.m_fPos[2] = 0.5f;

	tmpData.m_fTex[0] = 0.0f;
	tmpData.m_fTex[1] = 1.0f;
	(*m_data)[1] = tmpData;

	tmpData.m_fPos[0] = 0.5;
	tmpData.m_fPos[1] = 0;// -0.15;// 0.5;
	tmpData.m_fPos[2] = -0.5f;

	tmpData.m_fTex[0] = 1.0f;
	tmpData.m_fTex[1] = 1.0f;
	(*m_data)[2] = tmpData;

	//tmpData.m_fPos[0] = 0.5;
	//tmpData.m_fPos[1] = 0;// -0.15;// 0.5;
	//tmpData.m_fPos[2] = -0.5f;
	//

	//tmpData.m_fTex[0] = 1.0f;
	//tmpData.m_fTex[1] = 0.0f;
	//(*m_data)[3] = tmpData;


	//tmpData.m_fPos[0] = -0.5;
	//tmpData.m_fPos[1] = 0;// -0.15;// 0.5;
	//tmpData.m_fPos[2] = 0.5f;

	//tmpData.m_fTex[0] = 0.0f;
	//tmpData.m_fTex[1] = 1.0f;
	//(*m_data)[4] = tmpData;

	//tmpData.m_fPos[0] = 0.5;
	//tmpData.m_fPos[1] = 0;// -0.15;// 0.5;
	//tmpData.m_fPos[2] = 0.5f;

	//tmpData.m_fTex[0] = 1.0f;
	//tmpData.m_fTex[1] = 1.0f;
	//(*m_data)[5] = tmpData;

	InitDisplayBuffer();
}

void CLightTest::InitTexture(const char * pTextureFile)
{
	m_pTexture = CTexture::LoadTexture(pTextureFile);
	SetVertexAttrib(VERTEXATTRIB);
	
}

void CLightTest::InitDisplayBuffer()
{
	size_t nCount = m_data->size();
	GLubyte *index = new GLubyte[nCount];
	for (size_t i = 0; i < nCount; ++i)
	{
		index[i] = i;
	}
	
	m_nIndexBufferObj = utils::CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * nCount, index);

	m_nVertexBufferObj = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(CVertexData) * m_data->size(), m_data->data(), GL_STATIC_DRAW);

	m_pShader = new CShader("./shader/LightModel.vs", "./shader/LightModel.fs");
	SetVertexAttrib(VERTEXATTRIB);
	
	
	m_nMVPLocation = glGetUniformLocation(m_pShader->GetProgram(), "MVP");
	m_nNormalMatrixLocation = glGetUniformLocation(m_pShader->GetProgram(), "NM");
	//m_nLightModelLocation = glGetUniformLocation(m_pShader->GetProgram(), "V_LightModel");
	// nLightType
	m_nLightTypeLocation = glGetUniformLocation(m_pShader->GetProgram(), "nLightType");

	//glBindVertexArray(0);
	delete[]index;
	index = nullptr;
}

void CLightTest::Draw()
{
	mat4f tmp = GetModel();//*  m_fMVPMatrix;
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	if (m_pShader)
	{
		m_pShader->Bind();
	}
	if (m_pTexture)
	{
		m_pTexture->Bind();
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObj);
	glUniformMatrix4fv(m_nMVPLocation, 1, GL_FALSE, &tmp[0][0]);

	//tmp = tmp * m_fMVPMatrix;
	mat4f NormalMatrix = tmp.inverse().Transposition();
	glUniformMatrix4fv(m_nNormalMatrixLocation, 1, GL_FALSE, &NormalMatrix[0][0]);
	//TestShader(NormalMatrix);
	//TestFragmentShader();
	//Log << "NormalMatrix * tmp \n";
	//Log << (NormalMatrix * tmp) << "\n";
	//glUniform1i(m_nLightModelLocation, 0xF);
	//  1 : parallel light   2 : direction light  3 : spot light
	glUniform1i(m_nLightTypeLocation, 3);
	//utils::CheckError(__FUNCTION__, __FILE__, __LINE__);
	SetVertexAttrib(ATTRIBPOINTER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObj);
	
	glDrawElements(GL_TRIANGLES, m_data->size(), GL_UNSIGNED_BYTE, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (m_pTexture)
	{
		m_pTexture->Bind(false);
	}
	
	if (m_pShader)
	{
		m_pShader->Bind(false);
	}
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
}

void CLightTest::Update(float duration)
{
	static float speed = 0.0004;
	double zTranslate = duration * speed;
	static bool initFlag = true;
	if (initFlag)
	{
		//SetScale(4.414, 3.3118, 1);
		SetScale(1.5, 1.5, 1.5);
		SetRotate(90 * 3.1415926 / 180, 0, 0);
		initFlag = false;
	}
	if (zTranslate > 2 * 3.1415026)
	{
		zTranslate = 0;
	}
	AddRotate(zTranslate, 0, 0);
	
}

GLuint CLightTest::GetShaderProgram()
{
	if (m_pShader)
	{
		return m_pShader->GetProgram();
	}
	return -1;
	
}


void CLightTest::TestShader(mat4f model)
{
	static bool bTstShader = true;
	if (!bTstShader)
	{
		return;
	}
	//Log << "GetModel << \t" << GetModel();
	//Log << "m_fMVPMatrix << \t" << m_fMVPMatrix;
	Log << "CLightTest EndMatrix\n" << model << "\n";
	//mat4f tmpModel = model.Transposition();
	for (size_t i = 0; i < m_data->size() && i < 10; ++i)
	{
		CVertexData *p = m_data->data() + i;
		vec3f tmp(p->m_fNormal, 3);
		vec4f tmpTwo(tmp, 1.0);
		tmpTwo = tmpTwo * model;
		Log << tmpTwo << "\n";
	}
	//bTstShader = false;
}

void CLightTest::TestFragmentShader()
{
	std::function<vec3f(const vec3f &label, const vec3f &rotateVec)> reflect = \
		[&](const vec3f &label, const vec3f &rotateVec)->vec3f
	{
		mat4f rotateMatrix = CMatrix<4, 4, float>::GetRotate(3.1415926, label[0], label[1], label[2]);
		mat3f tmpRotateMatrix = rotateMatrix.reduceDimension();
		//vec3f tmpLabel = static_cast<vec3f>(label).Normalize();
		//vec3f tmpRotate = static_cast<vec3f>(rotateVec).Normalize();
		//Log << "rotateMatrix\n" << rotateMatrix;
		//Log << "tmpRotateMatrix\n" << tmpRotateMatrix;
		vec3f result;// (tmpRotate, 1.0);
		
		result = const_cast<vec3f&>(rotateVec) * tmpRotateMatrix;

		//Log << "reflect Calc\n";
		//Log << rotateVec - result - 2 * label;
		//Log << (rotateVec + result) / label;
		//Log << const_cast<vec3f&>(label).dotMultiply(rotateVec) / (const_cast<vec3f&>(label).Length() * const_cast<vec3f&>(rotateVec).Length()) << "\n";
		//Log << const_cast<vec3f&>(label).dotMultiply(result) / (const_cast<vec3f&>(label).Length() * const_cast<vec3f&>(result).Length()) << "\n";

		//Log << result;
		//Log << label;
		//vec3f VertVec = CVector<3, float>::CrossProduct(rotateVec, label);
		//VertVec.dotMultiply(rotateVec);
		//Log << "dotMultiply Result\t" << VertVec.dotMultiply(rotateVec) << "\t" << VertVec.dotMultiply(label) << "\n";
		//float dotResult = result[0] * VertVec[0] + result[1] * VertVec[1] + result[2] * VertVec[2];
		//Log << label << "\tdotResult = \t" << dotResult <<"\n";
		return result;
	};
	mat4f modelMatrix = GetModel();
	mat3f NormalMatrix = modelMatrix.inverse().Transposition().reduceDimension();

	vec3f tmpRotate = GetRotate();
	mat3f tmpModelMatrix = modelMatrix.reduceDimension();
	vec4f LightPos;
	LightPos[0] = LightPos[1] = 0;
	LightPos[2] = 1;
	LightPos[3] = 1;
	for (size_t i = 0; i < m_data->size() && i < 10; ++i)
	{
		CVertexData *p = m_data->data() + i;
		//vec3f V_LightPos = LightPos * tmpModelMatrix;
		vec3f normal(p->m_fNormal, 3);
		//normal[3] = 1.0;
		vec3f V_Normal = normal * NormalMatrix;
		vec3f tmpPos(p->m_fPos, 3);
		//tmpPos[3] = 1.0f;
		vec3f V_WorldPos = tmpPos * tmpModelMatrix;

		//V_TexCoord = texCoord;
		//V_WorldPos = vec4(pos, 1.0) * MVP;
		vec3f L;
		if (LightPos[3] == 0.0)
		{
			L = LightPos.Negative();
		}
		else
		{
			L = LightPos;
			L = V_WorldPos - L;
		}
		//vec3f L = V_LightPos.Normalize();// normalize(LightPos);
	
		vec3f n = V_Normal.Normalize();// normalize(V_Normal);
		//Log << "dot(L, n) = " << L.dotMultiply(n) / (L.Length() * n.Length()) << "\n";
		vec4f SpecularLightColor(1.0, 4);
		vec4f SpecularMaterial(0.8, 4);
		vec3f reflectDir = reflect(L.Negative(), n).Normalize();
		vec3f viewDir = (vec3f(0, 3) - V_WorldPos).Normalize();
		vec3f tmpViewDir(&viewDir[0], 3);
		float cosAlpha = reflectDir.dotMultiply(tmpViewDir);
		if (cosAlpha < 0)
		{
			continue;
		}
		//if (cosAlpha < 0)
		//{
		//	cosAlpha = -cosAlpha;
		//}
		vec4f SpecularColor = SpecularLightColor * SpecularMaterial * cosAlpha;// pow(cosAlpha, 4.0f);
		//return SpecularColor;
		Log << "i = " << i << "\tSpecuColor\t" << SpecularColor << "\n";
	}
	
}

void CLightTest::ResetState()
{
}

void CLightTest::SetVertexAttrib(AttribType type)
{
	if (type == CDrawable::AttribType::VERTEXATTRIB)
	{
		if (m_pShader)
		{
			const char *tmpName[] = { "pos", "texCoord", "normal" };
			m_pShader->SetShaderAttrib(3, tmpName);
		}
	}
	else if (type == CDrawable::AttribType::ATTRIBPOINTER)
	{
		if (m_pShader)
		{
			size_t count[] = { 3, 2, 3 };
			GLuint offset[] = { 0, sizeof(float) * 6, sizeof(float) * 3 };
			m_pShader->SetAttribPointer(count, GL_FLOAT, GL_FALSE, sizeof(CVertexData), offset);
		}
	}
}

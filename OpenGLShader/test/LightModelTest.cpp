#include "LightModelTest.h"

#include <iostream>


CLightTest::CLightTest() :C3DModel(),
m_pVertexData(nullptr),
m_pShader(nullptr),
m_nVertexBufferObj(-1),
m_pTexture(nullptr),
m_nLightType(1),
m_nLightTypeLocation(0),
m_nDataLength(0)
{
	Init();
}

CLightTest::~CLightTest()
{
	if (m_pVertexData != nullptr)
	{
		delete[] m_pVertexData;
		m_pVertexData = nullptr;
	}
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
	m_pVertexData = utils::CreateCubicData(0.6, m_nDataLength);
	
	InitDisplayBuffer();
}

void CLightTest::InitTexture(const char * pTextureFile)
{
	m_pTexture = CTexture::LoadTexture(pTextureFile);
	SetVertexAttrib(VERTEXATTRIB);
}

void CLightTest::InitDisplayBuffer()
{
	m_nVertexBufferObj = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(CVertexData) * m_nDataLength, m_pVertexData, GL_STATIC_DRAW);

	m_pShader = new CShader("./shader/LightModel/LightModel.vs", "./shader/LightModel/LightModel.fs");
	SetVertexAttrib(VERTEXATTRIB);
	
	SetShaderProgram(m_pShader->GetProgram());
	
	// nLightType
	m_nLightTypeLocation = glGetUniformLocation(m_pShader->GetProgram(), "nLightType");

}

void CLightTest::Draw()
{
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

	SetTransformMatrix();

	//glUniform1i(m_nLightModelLocation, 0xF);
	//  1 : parallel light   2 : dot light  3 : spot light
	glUniform1i(m_nLightTypeLocation, m_nLightType);
	
	SetVertexAttrib(ATTRIBPOINTER);
	
	//glDrawElements(GL_TRIANGLES, m_data->size(), GL_UNSIGNED_BYTE, 0);
	glDrawArrays(GL_QUADS, 0, m_nDataLength);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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
	static float speed = 0.0002;
	double zTranslate = duration * speed;
	static bool initFlag = true;
	if (initFlag)
	{
		//SetScale(4.414, 3.3118, 1);
		//SetScale(1.5, 1.5, 1.5);
		//SetTranslate(0, 0, -5);
		SetRotate(90 * 3.1415926 / 180, 0, 0);
		initFlag = false;
	}
	if (zTranslate > 2 * 3.1415026)
	{
		zTranslate = 0;
	}
	AddRotate(zTranslate, zTranslate, 0);
	
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
	
}

void CLightTest::SetLightType(GLuint lightType)
{
	m_nLightType = lightType;
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

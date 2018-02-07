#include "C3DModel.h"



C3DModel::C3DModel()
{
	m_fProjectionMatrix = CMatrix<4, 4, float>::GetUnit();

	m_fModelMatrix = CMatrix<4, 4, float>::GetUnit();

	m_nProjectionMatrixLocation = -1;
	m_nModelMatrixLocation = -1;
	m_nViewMatrixLocation = -1;
	m_nNormalMatrixLocation = -1;

	m_nShaderProgram = -1;

	m_fNormalMatrix = CMatrix<3, 3, float>::GetUnit();

	m_fViewMatrix;
	m_fModelMatrix;
}


C3DModel::~C3DModel()
{
}

void C3DModel::SetMatrix(mat4f & ViewMatrix, mat4f & ProjectionMatrix)
{
	m_fViewMatrix = ViewMatrix;
	m_fProjectionMatrix = ProjectionMatrix;
}

void C3DModel::SetScale(float x, float y, float z)
{
	m_fTrans[2][0] = x;
	m_fTrans[2][1] = y;
	m_fTrans[2][2] = z;
}

void C3DModel::SetRotate(float AngleX, float AngleY, float AngleZ)
{
	m_fTrans[1][0] = AngleX;
	m_fTrans[1][1] = AngleY;
	m_fTrans[1][2] = AngleZ;
}

void C3DModel::SetTranslate(float x, float y, float z)
{
	m_fTrans[0][0] = x;
	m_fTrans[0][1] = y;
	m_fTrans[0][2] = z;
}

void C3DModel::AddScale(float x, float y, float z)
{
	m_fTrans[2][0] += x;
	m_fTrans[2][1] += y;
	m_fTrans[2][2] += z;
}

void C3DModel::AddRotate(float AngleX, float AngleY, float AngleZ)
{
	std::function<double(float x, float delta)> lambda = [](float x, float delta)->double
	{
		double tmp = x + delta;
		const double Absolute = 2 * 3.1415926;
		if (tmp > Absolute)
		{
			tmp -= Absolute;
		}
		else if (tmp < 0)
		{
			tmp += Absolute;
		}
		return tmp;
	};
	m_fTrans[1][0] = lambda(m_fTrans[1][0], AngleX);
	m_fTrans[1][1] = lambda(m_fTrans[1][1], AngleY);
	m_fTrans[1][2] = lambda(m_fTrans[1][2], AngleZ);
}

void C3DModel::AddTranslate(float x, float y, float z)
{
	m_fTrans[0][0] += x;
	m_fTrans[0][1] += y;
	m_fTrans[0][2] += z;
}

vec3f C3DModel::GetScale()
{
	return m_fTrans[2];
}

vec3f C3DModel::GetRotate()
{
	return m_fTrans[1];
}

vec3f C3DModel::GetTranslate()
{
	return m_fTrans[0];
}

void C3DModel::SetModelMatrix(mat4f & modelMatrix)
{
	m_fModelMatrix = modelMatrix;
	m_fNormalMatrix = m_fModelMatrix.reduceDimension().inverse().Transposition();
}

void C3DModel::ResetStatus()
{
	m_fTrans[0] = CVector<3, float>(0, 3); // Translate
	m_fTrans[1] = CVector<3, float>(0, 3); // Rotate
	m_fTrans[2] = CVector<3, float>(1, 3); // Scale
	m_fModelMatrix = CMatrix<4, 4, float>::GetUnit();

}

void C3DModel::SetShaderProgram(GLuint program)
{
	m_nShaderProgram = program;
	m_nModelMatrixLocation = glGetUniformLocation(m_nShaderProgram, "modelMatrix");
	m_nViewMatrixLocation = glGetUniformLocation(m_nShaderProgram, "viewMatrix");
	m_nProjectionMatrixLocation = glGetUniformLocation(m_nShaderProgram, "projectionMatrix");
	m_nNormalMatrixLocation = glGetUniformLocation(m_nShaderProgram, "normalMatrix");
}

void C3DModel::SetTransformMatrix()
{
	//mat4f tmpModel = GetModel();
	glUniformMatrix4fv(m_nModelMatrixLocation, 1, GL_FALSE, &m_fModelMatrix[0][0]);
	//mat3f normalMatrix = tmpModel.reduceDimension().inverse().Transposition();
	glUniformMatrix3fv(m_nNormalMatrixLocation, 1, GL_FALSE, &m_fNormalMatrix[0][0]);

	glUniformMatrix4fv(m_nViewMatrixLocation, 1, GL_FALSE, &m_fViewMatrix[0][0]);
	
	glUniformMatrix4fv(m_nProjectionMatrixLocation, 1, GL_FALSE, &m_fProjectionMatrix[0][0]);
}

mat4f C3DModel::GetModel()
{
	return m_fModelMatrix;

	mat4f tmp = CMatrix<4, 4, float>::GetUnit();
	if (!m_fTrans[0].IsZero())
	{
		tmp = CMatrix<4, 4, float>::GetTranslate<float>(m_fTrans[0]);
		//tmp = tmp.Transposition();
	}
	
	if (m_fTrans[1][0] != float(0.0f))
	{
		tmp = tmp * CMatrix<4, 4, float>::GetRotate(m_fTrans[1][0], 1.0f, 0.0f, 0.0f);
	}
	if (m_fTrans[1][1] != float(0.0f))
	{
		tmp = tmp * CMatrix<4, 4, float>::GetRotate(m_fTrans[1][1], 0.0f, 1.0f, 0.0f);
	}
	if (m_fTrans[1][2] != float(0.0f))
	{
		tmp = tmp * CMatrix<4, 4, float>::GetRotate(m_fTrans[1][2], 0.0f, 0.0f, 1.0f);
	}
	if (m_fTrans[2] != CVector<3, float>(1.0f, 3))
	{
		tmp = tmp * CMatrix<4, 4, float>::GetScale<float>(m_fTrans[2]);
	}
	
	//tmp = tmp * m_fMVPMatrix;
	return tmp;
}

mat4f C3DModel::GetViewMatrix()
{
	return m_fViewMatrix;
}

mat4f C3DModel::GetProjectionMatrix()
{
	return m_fProjectionMatrix;
}

mat4f C3DModel::GetMVPMatrix()
{
	mat4f tmp = GetModel();
	tmp = tmp * m_fViewMatrix * m_fProjectionMatrix;
	return tmp;
}

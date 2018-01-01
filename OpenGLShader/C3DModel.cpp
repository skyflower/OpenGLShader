#include "C3DModel.h"



C3DModel::C3DModel()
{
	m_fMVPMatrix = CMatrix<4, 4, float>::GetUnit();

	m_fTrans[0] = CVector<3, float>(0, 3); // Translate
	m_fTrans[1] = CVector<3, float>(0, 3); // Rotate
	m_fTrans[2] = CVector<3, float>(1, 3); // Scale

	m_nMVPLocation = -1;
}


C3DModel::~C3DModel()
{
}

void C3DModel::SetMatrix(mat4f & ModelMatrix, mat4f & ViewMatrix, mat4f & ProjectionMatrix)
{
	m_fMVPMatrix = ModelMatrix * ViewMatrix * ProjectionMatrix;
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
		const double Absolute = 2 * 3.141;
		if (tmp > Absolute)
		{
			tmp -= Absolute;
		}
		else if (tmp < -Absolute)
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

mat4f C3DModel::GetModel()
{
	mat4f tmp = CMatrix<4, 4, float>::GetUnit();
	if (!m_fTrans[0].IsZero())
	{
		tmp = tmp *	CMatrix<4, 4, float>::GetTranslate(m_fTrans[0]);
	}
	
	if (m_fTrans[1][0] != float(0.0f))
	{
		tmp = tmp * CMatrix<4, 4, float>::GetRotate(m_fTrans[1][0], 1, 0, 0);
	}
	if (m_fTrans[1][1] != float(0.0f))
	{
		tmp = tmp * CMatrix<4, 4, float>::GetRotate(m_fTrans[1][1], 0, 1, 0);
	}
	if (m_fTrans[1][2] != float(0.0f))
	{
		tmp = tmp * CMatrix<4, 4, float>::GetRotate(m_fTrans[1][2], 0, 0, 1);
	}
	if (m_fTrans[2] != CVector<3, float>(1.0f, 3))
	{
		tmp = tmp * CMatrix<4, 4, float>::GetScale(m_fTrans[2]);
	}
	
	//tmp = tmp * m_fMVPMatrix;
	return tmp;
}

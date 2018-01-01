#include "shader.h"

#include <fstream>
#include <iostream>


extern std::fstream Log;


GLuint CShader::CreateShader(const char*filepath, GLenum CShaderType)
{
	
    GLuint CShader = glCreateShader(CShaderType);
	//utils::CheckError(__FILE__, __FUNCTION__, __LINE__);
    if(CShader == 0)
    {
        Log << "Error : CShader Create " << CShaderType << std::endl;
		return -1;
    }
	size_t CShaderSourceLength = -1;
    const GLchar *CShaderSource = utils::LoadFileContent(filepath, CShaderSourceLength);
	if (CShaderSource == nullptr)
	{
		Log << __FUNCTION__ << "  " << __LINE__ << "\tLoadFile " << filepath << "\tFailed\n";
		return -1;
	}
    //const GLchar *text = nullptr;
	
	GLint textLength = CShaderSourceLength;
    glShaderSource(CShader, 1, &CShaderSource, &textLength);
	//glShaderSource(CShader, 1, &CShaderSource, &textLength);
	//utils::CheckError(__FILE__, __FUNCTION__, __LINE__);
    glCompileShader(CShader);
	
    CheckShaderError(CShader, GL_SHADER_COMPILER, false, "Compiler Error :");
	delete []CShaderSource;
	CShaderSource = nullptr;
    return CShader;
}


CShader::CShader(const char*vsfileName, const char*fsfileName, const char *gsfile)
{
	m_nProgram = -1;
	m_nValueNum = 0;
	m_pLocation = nullptr;
	m_pName = nullptr;
	memset(m_shaders, 0, sizeof(GLuint) * NUM_SHADERS);
	
	CreateProgram(vsfileName, fsfileName, gsfile);
}
CShader::CShader(const char * fileVS, const char * attrib[], int nCount, GLenum memoryFormat, const char * gsFile)
{
	m_nProgram = glCreateProgram();
	memset(m_shaders, 0xFF, sizeof(m_shaders));
	m_shaders[0] = CreateShader(fileVS, GL_VERTEX_SHADER);
	//m_shaders[1] = -1;
	//m_shaders[2] = -1;
	if (gsFile != nullptr)
	{
		m_shaders[2] = CreateShader(gsFile, GL_GEOMETRY_SHADER);
	}
	for (size_t i = 0; i < NUM_SHADERS; ++i)
	{
		if ((m_shaders[i] != -1) && (m_shaders[i] != 0))
		{
			glAttachShader(m_nProgram, m_shaders[i]);
		}
	}

	glTransformFeedbackVaryings(m_nProgram, nCount, attrib, memoryFormat);
	glLinkProgram(m_nProgram);
	glValidateProgram(m_nProgram);

	for (size_t i = 0; i < NUM_SHADERS; ++i)
	{
		if ((m_shaders[i] != -1) && (m_shaders[i] != 0))
		{
			glDetachShader(m_nProgram, m_shaders[i]);
			glDeleteShader(m_shaders[i]);
		}
	}
}
CShader::CShader(int nCount, char * path[])
{
	m_nProgram = -1;
	m_nValueNum = 0;
	m_pLocation = nullptr;
	m_pName = nullptr;
	memset(m_shaders, 0, sizeof(GLuint) * NUM_SHADERS);
	if (nCount == 0)
	{
		return;
	}
	
	m_nProgram = glCreateProgram();
	
	m_shaders[0] = CreateShader(path[0], GL_VERTEX_SHADER);

	m_shaders[1] = CreateShader(path[1], GL_FRAGMENT_SHADER);

	if (path[2] != nullptr)
	{
		m_shaders[2] = CreateShader(path[2], GL_GEOMETRY_SHADER);
	}
	if (path[3] != nullptr)
	{
		m_shaders[3] = CreateShader(path[3], GL_TESS_CONTROL_SHADER);
	}
	if (path[4] != nullptr)
	{
		m_shaders[4] = CreateShader(path[4], GL_TESS_EVALUATION_SHADER);
	}


	for (size_t i = 0; i < NUM_SHADERS; ++i)
	{
		if (glIsShader(m_shaders[i]))
		{
			glAttachShader(m_nProgram, m_shaders[i]);

		}
	}
	//utils::CheckError(__FILE__, __FUNCTION__, __LINE__);
	glLinkProgram(m_nProgram);
	CheckShaderError(m_nProgram, GL_LINK_STATUS, true, "Error : Link Program");
	glValidateProgram(m_nProgram);
	CheckShaderError(m_nProgram, GL_VALIDATE_STATUS, true, "Error : Validate Program");

	for (size_t i = 0; i < NUM_SHADERS; ++i)
	{
		if (glIsShader(m_shaders[i]))
		{
			glDetachShader(m_nProgram, m_shaders[i]);
			glDeleteShader(m_shaders[i]);
		}
	}
}
void CShader::CreateProgram(const char*vsfileName, const char*fsfileName, const char *gsfile)
{
	m_nProgram = glCreateProgram();
	//utils::CheckError(__FILE__, __FUNCTION__, __LINE__);

	m_shaders[0] = CreateShader(vsfileName, GL_VERTEX_SHADER);
	
	m_shaders[1] = CreateShader(fsfileName, GL_FRAGMENT_SHADER);
	
	if (gsfile != nullptr)
	{
		m_shaders[2] = CreateShader(gsfile, GL_GEOMETRY_SHADER);
	}
    for(size_t i = 0; i < NUM_SHADERS; ++i)
    {
		if ((m_shaders[i] != -1) && (m_shaders[i] != 0))
		{
			glAttachShader(m_nProgram, m_shaders[i]);
			
		}
    }
	//utils::CheckError(__FILE__, __FUNCTION__, __LINE__);
    glLinkProgram(m_nProgram);
    CheckShaderError(m_nProgram, GL_LINK_STATUS, true, "Error : Link Program");
    glValidateProgram(m_nProgram);
	CheckShaderError(m_nProgram, GL_VALIDATE_STATUS, true, "Error : Validate Program");

	for (size_t i = 0; i < NUM_SHADERS; ++i)
	{
		if ((m_shaders[i] != -1) && (m_shaders[i] != 0))
		{
			glDetachShader(m_nProgram, m_shaders[i]);
			glDeleteShader(m_shaders[i]);
		}
	}
}

void CShader::Bind(bool flag)
{
	if ((m_nProgram != -1) && glIsProgram(m_nProgram) && (flag == true))
	{
		glUseProgram(m_nProgram);
	}
	else
	{
		glUseProgram(0);
	}
}

void CShader::SetShaderAttrib(int count, const char * value[])
{
	if ((count <= 0) || (value == nullptr) || !glIsProgram(m_nProgram))
	{
		return;
	}
	if (m_nValueNum > 0)
	{
		for (int i = 0; i < m_nValueNum; ++i)
		{
			if (m_pName[i])
			{
				delete[]m_pName[i];
				m_pName[i] = nullptr;
			}
		}
		delete[] m_pName;
		m_pName = nullptr;
		m_nValueNum = 0;
		if (m_pLocation)
		{
			delete[]m_pLocation;
			m_pLocation = nullptr;
		}
	}
	
	m_nValueNum = count;
	m_pLocation = new GLuint[m_nValueNum];
	memset(m_pLocation, 0xFF, sizeof(GLuint) * m_nValueNum);
	m_pName = new char*[m_nValueNum];
	for (int i = 0; i < m_nValueNum; ++i)
	{
		if (value[i] != nullptr)
		{
			size_t tmpSize = strlen(value[i]);
			m_pName[i] = new char[tmpSize + 1];
			memset(m_pName[i], 0, sizeof(char) * (tmpSize + 1));
			memcpy(m_pName[i], value[i], sizeof(char) * tmpSize);
			m_pLocation[i] = glGetAttribLocation(m_nProgram, m_pName[i]);
			if (m_pLocation[i] == -1)
			{
				Log << __FUNCTION__ << "  " << __LINE__ << "\tGetLocation: " << m_pName[i] << "  Failed\n";
			}
		}
	}
}

void CShader::SetAttribPointer(size_t* count, GLenum type, GLboolean flag, GLsizei stride, GLuint* offset)
{
	for (int i = 0; i < m_nValueNum; ++i)
	{
		glEnableVertexAttribArray(m_pLocation[i]);
		glVertexAttribPointer(m_pLocation[i], count[i], type, flag, stride, (GLvoid*)offset[i]);
	}
}


CShader::~CShader()
{
    for(size_t i = 0; i < NUM_SHADERS; ++i)
    {
        glDetachShader(m_nProgram, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }
    glDeleteProgram(m_nProgram);

	if (m_pLocation)
	{
		delete[]m_pLocation;
		m_pLocation = nullptr;
	}
	if (m_pName && (m_nValueNum > 0))
	{
		for (int i = 0; i < m_nValueNum; ++i)
		{
			if (m_pName[i])
			{
				delete[]m_pName[i];
				m_pName[i] = nullptr;
			}
		}
		delete[] m_pName;
		m_pName = nullptr;
	}
}

void CShader::CheckShaderError(GLuint CShader, GLuint flag, bool isProgram, std::string Msg)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };
	if (isProgram)
	{
		glGetProgramiv(CShader, flag, &success);
	}
	else
	{
		glGetShaderiv(CShader, flag, &success);
	}
	if (success == GL_FALSE)
	{
		if (isProgram)
		{
			glGetProgramInfoLog(CShader, sizeof(error), NULL, error);
		}
		else
		{
			glGetShaderInfoLog(CShader, sizeof(error), NULL, error);
		}
		Log << Msg.c_str() << error << std::endl;
	}
}

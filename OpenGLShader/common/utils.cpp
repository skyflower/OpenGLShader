#include "utils.h"
#include <SOIL\SOIL.h>
#include <fstream>


//using namespace std;


char *utils::LoadFileContent(const char *FilePath, size_t &Length)
{
	std::ifstream fs(FilePath, std::ios::beg | std::ios::in | std::ios::binary);
	
	if (!fs.good())
	{
		return nullptr;
	}
	
	fs.seekg(0, std::ios::beg);
	fs.seekg(0, std::ios::end);
	
	//Length;
	Length = fs.tellg();
	char *Array = new char[Length + 1];

	memset(Array, 0, sizeof(char) * (Length + 1));
	fs.seekg(0, std::ios::beg);
	//fs.read()
	fs.read(Array, Length);
	fs.close();
	return Array;

	/*FILE *fp = nullptr;
	fopen_s(&fp, FilePath, "r+");
	if (fp == nullptr)
	{
		return nullptr;
	}
	_fseeki64(fp, 0, SEEK_SET);
	_fseeki64(fp, 0, SEEK_END);
	Length = _ftelli64(fp);
	if (Length == 0)
	{
		Length = -1;
		return nullptr;
	}
	_fseeki64(fp, 0, SEEK_SET);
	char *tmpArray = nullptr;
	tmpArray = new char[Length + 1];
	memset(tmpArray, 0, sizeof(char) * (Length + 1));
	if (tmpArray == nullptr)
	{
		fclose(fp);
		Length = -1;
		return nullptr;
	}
	memset(tmpArray, 0, sizeof(char) * (Length + 1));
	fread_s(tmpArray, Length, sizeof(char), Length, fp);
	fclose(fp);
	return tmpArray;*/
}

GLuint utils::CaptureScreen(int width, int height, std::function<void()> foo)
{
	foo();
	//std::function
	unsigned char*pScreenPixel = new unsigned char[width * height * 3];
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pScreenPixel);
	GLuint id = -1;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pScreenPixel);
	glBindTexture(GL_TEXTURE_2D, 0);
	return id;
}


float utils::randf()
{
	return std::rand() / (double)RAND_MAX;
}

float utils::srandf()
{
	return (utils::randf() - 0.5f) * 2;
}

//
//GLuint utils::CreateGPUProgram(const char*vsPath, const char*fsPath)
//{
//	GLuint vsShader = glCreateShader(GL_VERTEX_SHADER);
//	GLuint fsShader = glCreateShader(GL_FRAGMENT_SHADER);
//	
//	size_t vsCodeLength = 0;
//	const char*vsCode = utils::LoadFileContent(vsPath, vsCodeLength);
//
//	size_t fsCodeLength = 0;
//	const char*fsCode = utils::LoadFileContent(fsPath, fsCodeLength);
//	glShaderSource(vsShader, 1, &vsCode, nullptr);
//	
//	glShaderSource(fsShader, 1, &fsCode, nullptr);
//	
//	glCompileShader(vsShader);
//	CheckShaderError(vsShader, GL_SHADER_COMPILER, false, "vsShader Compiler:\n");
//
//
//	glCompileShader(fsShader);
//	CheckShaderError(fsShader, GL_SHADER_COMPILER, false, "fsShader Compiler:\n");
//
//	
//
//	GLuint program = glCreateProgram();
//	glAttachShader(program, vsShader);
//	glAttachShader(program, fsShader);
//
//	glLinkProgram(program);
//	CheckShaderError(program, GL_LINK_STATUS, true, "Link Program:\n");
//
//	glValidateProgram(program);
//	CheckShaderError(program, GL_VALIDATE_STATUS, true, "Validate Program:\n");
//
//
//	glDetachShader(program, vsShader);
//	glDetachShader(program, fsShader);
//
//	glDeleteShader(fsShader);
//	glDeleteShader(vsShader);
//
//	delete[]vsCode;
//	vsCode = nullptr;
//	delete[]fsCode;
//	fsCode = nullptr;
//
//	return program;
//}


void utils::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, std::string Msg)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };
	if (isProgram)
	{
		glGetProgramiv(shader, flag, &success);
	}
	else
	{
		glGetShaderiv(shader, flag, &success);
	}
	if (success == GL_FALSE)
	{
		if (isProgram)
		{
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		}
		else
		{
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		}
		WriteError(" %s, %s", Msg.c_str(), error);
	}
}



GLuint utils::CreateBufferObject(GLenum bufferType, size_t Size, void *data, GLenum usage /* GL_STATIC_DRAW */)
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(bufferType, vbo);
	glBufferData(bufferType, Size, data, usage);
	glBindBuffer(bufferType, 0);
	return vbo;
}


GLuint utils::CreateVertexArrayObj(std::function<void()> settings)
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	settings();
	glBindVertexArray(0);
	return vao;
}

GLuint utils::CreateFrameBufferObj(GLuint & colorBuffer, int width, int height, GLuint & depthBuffer, GLuint *colorBufferTwo)
{
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);

	if (colorBufferTwo != nullptr)
	{
		glGenTextures(1, colorBufferTwo);
		glBindTexture(GL_TEXTURE_2D, *colorBufferTwo);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, *colorBufferTwo, 0);
		GLenum buffer[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, buffer);
	}


	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		WriteError("Create Frame Buffer Failed");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return fbo;
}

GLuint utils::CreateFrameBuffer(GLuint &DepthBuffer, GLuint &Texture, size_t sizeX, size_t sizeY)
{
	GLuint FrameBuffer;
	//GLuint DepthBuffer;
	//GLuint Texture;
	glGenFramebuffers(1, &FrameBuffer);
	glGenRenderbuffers(1, &DepthBuffer);
	glGenTextures(1, &Texture);
	// set up the size for the rendered texture
	//int sizeX = 2048;
	//int sizeY = 2048;
	// Bind the offscreen framebuffer to be the current output
	// display
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);
	// Bind the Depth Buffer to the context, allocate its storage,
	// and attach it to the Framebuffer
	glBindRenderbuffer(GL_RENDERBUFFER, DepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER,
		GL_DEPTH_COMPONENT, sizeX, sizeY);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, DepthBuffer);
	// Bind the Texture to the Context
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set up a NULL texture of the size you want to render into
	// and set its properties
	glTexImage2D(GL_TEXTURE_2D, 0, 4, sizeX, sizeY, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
			GL_REPLACE);

	// Tell OpenGL that you are going to render into the color
	// planes of the Texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, Texture, 0);
	// see if OpenGL thinks the framebuffer is complete enough to
	// use:
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		WriteError("FrameBuffer is not complete");
	}
	return FrameBuffer;
}

void utils::OutputToFile(mat4f & ks, glm::mat4 & hs)
{
	WriteInfo("UserDefine Matrix======================");
	WriteInfo("\n%s", ks.FormatToString().c_str());

	std::stringbuf tmpStr;
	std::iostream outStream(&tmpStr);

	outStream << "GLM Matrix======================\n";
	for (size_t i = 0; i < 4; ++i)
	{
		for (size_t j = 0; j < 4; ++j)
		{
			outStream << hs[i][j] << "\t";
		}
		outStream << "\n";
	}
	outStream << "\n";
	outStream.flush();
	WriteInfo("%s", tmpStr.str().c_str());
}

void utils::TestMatrixGLM()
{
	
	//WriteInfo("=========================");
	//mat4f translate = CMatrix<4, 4, float>::GetTranslate(0.5, 0.6, 0.38);
	//glm::mat4 glmTranslateMatrix = glm::translate(glmTranslate);
	//utils::OutputToFile(translate, glmTranslateMatrix);
}

GLuint utils::CreateComputeProgram(const char*computeShaderPath)
{
	GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
	
	size_t computeCodeLength = 0;
	const char*computeCode = utils::LoadFileContent(computeShaderPath, computeCodeLength);

	glShaderSource(computeShader, 1, &computeCode, nullptr);
	
	glCompileShader(computeShader);
	CheckShaderError(computeShader, GL_SHADER_COMPILER, false, "computeShader Compiler:\n");

	GLuint program = glCreateProgram();
	glAttachShader(program, computeShader);
	
	glLinkProgram(program);
	CheckShaderError(program, GL_LINK_STATUS, true, "Link Program:\n");

	glValidateProgram(program);
	CheckShaderError(program, GL_VALIDATE_STATUS, true, "Validate Program:\n");

	glDetachShader(program, computeShader);
	
	glDeleteShader(computeShader);
	
	delete[]computeCode;
	computeCode = nullptr;
	
	return program;
}

bool utils::CreateComputeTexture(const char*shaderPath, const char*imagePath, GLuint &inputTexture, GLuint &outputTexture)
{
	int imageWidth = 0;
	int imageHeight = 0;
	int iBits = 0;
	unsigned char*pImageData = SOIL_load_image(imagePath, &imageWidth, &imageHeight, &iBits, 4);
	if (pImageData == nullptr)
	{
		return false;
	}
	GLuint program = utils::CreateComputeProgram(shaderPath);
	//GLuint computeDstTexture;
	glGenTextures(1, &outputTexture);
	glBindTexture(GL_TEXTURE_2D, outputTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, imageWidth, imageHeight);
	glBindTexture(GL_TEXTURE_2D, 0);

	//GLuint mainTexture;
	glGenTextures(1, &inputTexture);
	glBindTexture(GL_TEXTURE_2D, inputTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImageData);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, imageWidth, imageHeight);
	//glTextureSubImage2DEXT(mainTexture, 0, 0, 0, 0, imageWidth, imageHeight, GL_RGBA8, GL_UNSIGNED_BYTE, pImageData);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, imageWidth, imageHeight, GL_RGBA, GL_UNSIGNED_BYTE, pImageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(pImageData);
	pImageData = nullptr;
	return true;
}

double * utils::Logistic(float a, float x, size_t N)
{
	if ((a < 3.57 || a >= 4.0) || (x <= 0) || (x >= 1.0) || (N <= 1))
	{
		return nullptr;
	}
	const size_t L = 200;
	double *Array = new double[N];
	memset(Array, 0, sizeof(double) * N);
	for (size_t i = 0; i < L; ++i)
	{
		x = a * x * (1 - x);
	}
	Array[0] = x;
	for (size_t i = 1; i < N; ++i)
	{
		Array[i] = a * Array[i - 1] * (1 - Array[i - 1]);
	}
	return Array;

}

mat4f utils::glmToMatrix(const glm::mat4 &hs, bool bTranspose)
{
	glm::mat4 rh = hs;
	if (bTranspose)
	{
		rh = glm::transpose(hs);
	}
	mat4f result;
	for (size_t i = 0; i < 4; ++i)
	{
		for (size_t j = 0; j < 4; ++j)
		{
			result[i][j] = rh[i][j];
		}
	}
	return result;
}

glm::mat4 utils::MatrixToglm(mat4f &matrix, bool bTranspose)
{
	mat4f tmp = matrix;
	if (bTranspose)
	{
		tmp = matrix.Transposition();
	}
	glm::mat4 result;
	for (size_t i = 0; i < 4; ++i)
	{
		for (size_t j = 0; j < 4; ++j)
		{
			result[i][j] = tmp[i][j];
		}
	}
	return result;
}

GLuint utils::CreateTexture3D(int w, int h, int d)
{
	char *pData = new char[w * h * d * 4];
	char *tmp = pData;
	for (size_t i = 0; i < w; ++i)
	{
		for (size_t j = 0; j < h; ++j)
		{
			for (size_t k = 0; k < d; ++k)
			{
				*tmp++ = std::rand() & 0xFF;
				*tmp++ = std::rand() & 0xFF;
				*tmp++ = std::rand() & 0xFF;
				*tmp++ = std::rand() & 0xFF;
			}
		}
	}
	GLuint textureID = -1;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_3D, textureID);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8_SNORM, w, h, d, 0, GL_RGBA, GL_BYTE, pData);

	glBindTexture(GL_TEXTURE_3D, 0);
	delete[]pData;
	pData = nullptr;
	return textureID;
}

GLuint utils::CreateTFOProgram(const char * fileVS, const char * attrib[], int nCount, GLenum memoryFormat, const char * gsFile)
{
	GLuint nTfoProgram = glCreateProgram();

	GLint vsShader = glCreateShader(GL_VERTEX_SHADER);
	size_t CShaderSourceLength = -1;
	const GLchar *CShaderSource = utils::LoadFileContent(fileVS, CShaderSourceLength);

	GLint textLength = CShaderSourceLength;
	glShaderSource(vsShader, 1, &CShaderSource, &textLength);
	glCompileShader(vsShader);
	delete[]CShaderSource;
	CShaderSource = nullptr;

	GLuint gsShader = 0;
	if (gsFile != nullptr)
	{

		gsShader = glCreateShader(GL_GEOMETRY_SHADER);
		CShaderSource = utils::LoadFileContent(gsFile, CShaderSourceLength);

		GLint textLength = CShaderSourceLength;
		glShaderSource(gsShader, 1, &CShaderSource, &textLength);
		glCompileShader(gsShader);
		delete[]CShaderSource;
		CShaderSource = nullptr;

		glAttachShader(nTfoProgram, gsShader);
	}

	glAttachShader(nTfoProgram, vsShader);
	//glAttachShader(nTfoProgram, fsShader);
	//const char *Attribs[] = { "gl_Position" };
	glTransformFeedbackVaryings(nTfoProgram, nCount, attrib, memoryFormat);
	glLinkProgram(nTfoProgram);
	glValidateProgram(nTfoProgram);

	glDetachShader(nTfoProgram, vsShader);
	glDeleteShader(vsShader);

	if (glIsShader(gsShader))
	{
		glDetachShader(nTfoProgram, gsShader);
		glDeleteShader(gsShader);
	}

	return nTfoProgram;
}

GLuint utils::CreateTransformFeedBack(GLuint &tfoBuffer, size_t Size, void *data, GLenum usage /* GL_STATIC_DRAW */)
{
	GLuint bufferID = 0;
	glGenTransformFeedbacks(1, &bufferID);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, bufferID);
	tfoBuffer = utils::CreateBufferObject(GL_ARRAY_BUFFER, Size, data, usage);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	return bufferID;
}

void utils::CheckError(const char*file, const char *func, const int line)
{
	GLenum error;
	error = glGetError();
	if (error != GLEW_OK)
	{
		WriteInfo("%s, %s, %d", file, func, line);
	}
}

void utils::CreateGroundData(int width, std::vector<CVertexData>*& pVertex)
{
	int tmpWidth = width;
	CVertexData pData[] = { { tmpWidth, 0, tmpWidth, 0, 1, 0, 0, 0},
	{ tmpWidth, 0, -tmpWidth, 0, 1, 0, 0, 1},
	{-tmpWidth, 0, -tmpWidth, 0, 1, 0, 1, 1},
	{-tmpWidth, 0, tmpWidth, 0, 1, 0, 1, 0} };
	if (pVertex == nullptr)
	{
		pVertex = new std::vector<CVertexData>(4);
	}
	if (pVertex->size() >= 4)
	{
		memcpy(pVertex->data(), pData, sizeof(CVertexData) * 4);
	}
}

CVertexData * utils::CreateCubicData(float scale, size_t & Length)
{
	GLfloat tmpPoints[] =
	{ 1, 1, 1,	1, 1, -1,	-1, 1, -1,	 -1, 1, 1,
		1, -1, 1,	1, -1, -1,	-1,	-1, -1,		-1, -1, 1 };
	//GLfloat tmpScale = 0.6;
	for (size_t i = 0; i < sizeof(tmpPoints) / sizeof(GLfloat); ++i)
	{
		tmpPoints[i] = scale * tmpPoints[i];
	}
	GLfloat tmpNormal[] = {
		0, 1, 0,	0, -1, 0,
		1, 0, 0,	0, 0, -1,
		-1, 0, 0,	0, 0, 1
	};
	GLfloat tmpTexCoord[] = { 0, 0, 1, 0, 1, 1, 0, 1 };
	
	GLubyte tmpIndex[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 5, 4, 1, 2, 6, 5, 2, 3, 7, 6, 0, 3, 7, 4 };
	Length = sizeof(tmpIndex);
	CVertexData* pVertex = new CVertexData[Length];
	memset(pVertex, 0, sizeof(CVertexData) * Length);
	for (size_t i = 0; i < Length; ++i)
	{
		memcpy(pVertex[i].m_fPos, &tmpPoints[tmpIndex[i] * 3], sizeof(float) * 3);
		memcpy(pVertex[i].m_fTex, &tmpTexCoord[(i % 4) * 2], sizeof(float) * 2);
		memcpy(pVertex[i].m_fNormal, &tmpNormal[(i / 4) * 3], sizeof(float) * 3);
	}
	return pVertex;
}

void utils::CreateSphereData(float radis, int slide, int stacks, std::vector<CVertex>*& pVertexVec, std::vector<unsigned int>*& pIndexVec)
{
	if ((radis < 0.00001) && (slide < 2))
	{
		return;
	}
	size_t Length = slide * stacks + 2;
	if (pVertexVec == nullptr)
	{
		pVertexVec = new std::vector<CVertex>(Length);
	}
	if (pIndexVec == nullptr)
	{
		//(slide - 1) * 2 * stacks 
		pIndexVec = new std::vector<unsigned int>(slide * stacks * 6);
	}
	double radisStep = 2 * radis / (slide + 1);
	unsigned int dataIndex = 0;
	unsigned int indexIndex = 0;
	CVertex *pVertex = pVertexVec->data();
	pVertex[0].mPos[0] = -radis;
	pVertex[0].mPos[1] = 0;
	pVertex[0].mPos[2] = 0;
	dataIndex++;

	unsigned int *pIndex = pIndexVec->data();
	for (int i = 0; i < slide; ++i)
	{
		double distanceX = (i + 1) * radisStep;

		double tmpCos = sqrt(distanceX * 2 * radis);
		double tmp = sqrt(tmpCos * tmpCos - distanceX * distanceX);
		double nextX = -radis + distanceX;
		//double tmp = sqrt(radis * radis - nextX * nextX);
		double alphaStep = (2 * 3.1415926) / (stacks);
		unsigned int tmpBeginIndex = dataIndex;
		//WriteInfo("distanceX = %f, tmpCos = %f, tmp = %f, nextX = %f", distanceX, tmpCos, tmp, nextX);
		for (int j = 0; j < stacks; ++j)
		{
			pVertex[dataIndex].mPos[0] = nextX;
			pVertex[dataIndex].mPos[1] = tmp * std::sin(alphaStep * j);
			pVertex[dataIndex].mPos[2] = tmp * std::cos(alphaStep * j);

			dataIndex++;

		}
		if (i == 0)
		{
			for (int j = 0; j < stacks; ++j)
			{
				pIndex[indexIndex++] = 0;
				pIndex[indexIndex++] = j + 1;
				pIndex[indexIndex++] = (j + 1) % stacks + 1;

			}
		}
		if (i > 0)
		{
			unsigned int preBeginIndex = tmpBeginIndex - stacks;
			for (int j = 0; j < stacks; ++j)
			{
				pIndex[indexIndex++] = preBeginIndex + j;
				pIndex[indexIndex++] = preBeginIndex + (j + 1) % stacks;
				pIndex[indexIndex++] = tmpBeginIndex + j;

				pIndex[indexIndex++] = tmpBeginIndex + j;
				pIndex[indexIndex++] = preBeginIndex + (j + 1) % stacks;
				pIndex[indexIndex++] = tmpBeginIndex + (j + 1) % stacks;

			}
		}
		if (i == slide - 1)
		{
			pVertex[dataIndex].mPos[0] = radis;
			pVertex[dataIndex].mPos[1] = 0;
			pVertex[dataIndex].mPos[2] = 0;
			for (int j = 0; j < stacks; ++j)
			{
				pIndex[indexIndex++] = dataIndex;
				pIndex[indexIndex++] = j + tmpBeginIndex;
				pIndex[indexIndex++] = (j + 1) % stacks + tmpBeginIndex;
			}
			dataIndex++;
		}
	}
	
	for (size_t i = 0; i < Length; ++i)
	{
		memcpy(pVertex[i].mColor, pVertex[i].mPos, sizeof(CVertex::mPos));
	}

}



bool utils::CutLogBySpace(const char * begin, const char * end, int *tmpArray, size_t & nCount, char cutChar)
{
	std::function<char*(const char*content, const char *end)> FindNotSpace = [&](const char*begin, const char *end)->char*
	{
		for (size_t i = 0; begin + i < end; ++i)
		{
			if (begin[i] != cutChar)
			{
				return (char*)begin + i;
			}
		}
		return nullptr;
	};
	if ((begin == nullptr) || (end == nullptr) || (tmpArray == nullptr))
	{
		return false;
	}
	nCount = 0;
	size_t i = 0;
	while (begin + i < end)
	{
		char *pointer = FindNotSpace(begin + i, end);
		if ((pointer == nullptr) || (pointer >= end))
		{
			break;
		}
		nCount += 1;
		pointer = strchr(pointer + 1, cutChar);
		if ((pointer == NULL) || (pointer >= end))
		{
			break;
		}
		i = pointer - begin + 1;
	}
	//int *Pointer = new int[nCount];
	i = 0;
	size_t index = 0;
	while (begin + i < end)
	{
		char *pointer = FindNotSpace(begin + i, end);
		if ((pointer == NULL) || (pointer >= end))
		{
			break;
		}
		//nCount += 1;
		tmpArray[index++] = pointer - begin;
		pointer = strchr(pointer + 1, cutChar);
		if ((pointer == NULL) || (pointer >= end))
		{
			break;
		}
		i = pointer - begin + 1;
	}
	tmpArray[index] = end - begin;
	return true;
}

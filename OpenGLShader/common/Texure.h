#pragma once
#include <iostream>

#define STATIC_GLEW
#include <GL/glew.h>

#define GLUT_BUILDING_LIB
#include <GL/glut.h>

#include <unordered_map>



class CTexture
{
public:
	
	
	void Bind(bool flag = true);
	void UnBind();
	
	static CTexture *LoadTexture(const char *filepath);
	static CTexture *LoadCubeMapTexture(const char*PosX, const char* NegX,
		const char *PosY, const char *NegY,
		const char *PosZ, const char *NegZ);
	static void UnLoadTexture(const CTexture *pTexture);
	static CTexture* CreateTextureFromData(const unsigned char*pData, size_t width, size_t height, GLenum colorType);
	inline GLuint GetTextureID() { return m_TextureID;	}
	inline size_t GetWidth() { return m_nWidth; }
	inline size_t GetHeight() { return m_nHeight; }
private:
	CTexture();
	~CTexture();
	bool InitByFilePath(const char*filepath);
	GLuint InitTextureByData(const unsigned char * pData, size_t width, size_t height, GLenum colorType);

	GLuint m_TextureID;
	GLenum m_nTexType;
	//GLuint m_nShaderProgram;
	//GLuint m_nTexLocation;
	std::string m_strPath;
	

	int m_nWidth; // texture org width | picture width
	int m_nHeight;	// texture org height | picture height
	// std::string : image file name
	// std::pair<引用计数, CTexture指针>
	static std::unordered_map<std::string, std::pair<unsigned int, CTexture*>> *m_pCacheTexture;
};


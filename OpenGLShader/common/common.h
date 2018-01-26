#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <iostream>

#include "../math/CVector.h"

#include "../math/CMatrix.h"
#include <fstream>
#include "Log.h"
#include "../graphics/Drawable.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>



typedef CVector<3, float> vec3f;
typedef CVector<2, float> vec2f;
typedef CVector<4, float> vec4f;

typedef CVector<3, int> vec3i;
typedef CVector<2, int> vec2i;
typedef CVector<4, int> vec4i;

typedef CVector<3, unsigned int> vec3u;
typedef CVector<2, unsigned int> vec2u;
typedef CVector<4, unsigned int> vec4u;

typedef CVector<3, unsigned char> vec3b;
typedef CVector<2, unsigned char> vec2b;
typedef CVector<4, unsigned char> vec4b;

typedef CVector<3, double> vec3d;
typedef CVector<2, double> vec2d;
typedef CVector<4, double> vec4d;


typedef CMatrix<3, 3, float> mat3f;
typedef CMatrix<2, 2, float> mat2f;
typedef CMatrix<4, 4, float> mat4f;

typedef CMatrix<3, 3, int> mat3i;
typedef CMatrix<2, 2, int> mat2i;
typedef CMatrix<4, 4, int> mat4i;


typedef CMatrix<3, 3, unsigned int> mat3u;
typedef CMatrix<2, 2, unsigned int> mat2u;
typedef CMatrix<4, 4, unsigned int> mat4u;


typedef CMatrix<3, 3, double> mat3d;
typedef CMatrix<2, 2, double> mat2d;
typedef CMatrix<4, 4, double> mat4d;







#endif // COMMON_H_INCLUDED

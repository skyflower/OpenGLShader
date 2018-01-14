#version 430
attribute vec3 pos;
attribute vec2 texCoord;
attribute vec3 normal;

uniform mat4 MVP;
uniform mat4 modelMatrix;

varying vec2 V_TexCoord;
varying vec3 scopePos;
varying vec3 V_Normal;

void main()
{
	int id = 0;
	
    gl_Position = vec4(pos, 1.0) * modelMatrix;
   
	V_TexCoord = vec2(texCoord.x, 1 - texCoord.y);
	scopePos = (vec4(pos, 1.0) * modelMatrix).xyz;
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	V_Normal = normal.xyz * normalMatrix;
}
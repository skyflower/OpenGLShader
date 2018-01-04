#version 430
attribute vec3 pos;
attribute vec3 normal;
attribute vec2 texCoord;
attribute vec3 color;

uniform mat4 MVP;
uniform mat4 modelMatrix;

varying vec3 V_Normal;
varying vec4 V_WorldPos;
varying vec2 V_TexCoord;
varying vec4 V_LightPos;
varying vec3 V_LightDirection;
varying float V_SpotAngle;

void main()
{
	vec4 LightPos = vec4(0.0, 0.0, 2.0, 1.0);
	vec4 tmpLightPos = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 tmpLightDirection = tmpLightPos - LightPos;
	V_LightDirection = -tmpLightDirection.xyz;
	V_SpotAngle = 50 * 3.1415926 / 180;
	mat3 NM = transpose(inverse(mat3(modelMatrix)));
    V_Normal = normal * NM;
    gl_Position = vec4(pos, 1.0) * MVP;
    
	V_TexCoord = texCoord;
	V_WorldPos = vec4(pos, 1.0) * modelMatrix;
   
	V_LightPos = LightPos;
}
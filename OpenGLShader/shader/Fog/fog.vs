#version 430
attribute vec3 pos;
attribute vec4 normal;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;

varying vec3 v_normal;
varying vec4 v_pos;
varying vec4 v_lightPos;
varying vec4 v_eyePos;
varying float v_fogBegin;
varying float v_fogEnd;
varying vec3 v_fogColor;


void main()
{
	//mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	v_normal = normal.xyz * normalMatrix;
	
    //vec4 tmpPosition = projectionMatrix*viewMatrix * modelMatrix * vec4(pos.x, pos.y, pos.z, 1.0);
	vec4 tmpPosition = vec4(pos.x, pos.y, pos.z, 1.0) * modelMatrix * viewMatrix * projectionMatrix;
	gl_Position = tmpPosition; //vec4(tmpPosition.x /tmpPosition.w, tmpPosition.y / tmpPosition.w, tmpPosition.z / tmpPosition.w);
	v_pos = vec4(pos.x, pos.y, pos.z ,1.0) * modelMatrix;
	vec4 tmpLightPos = vec4(1.0, 1.0, 1.0, 1.0);
	v_lightPos = tmpLightPos;
	v_eyePos = vec4(pos, 1.0) * modelMatrix * viewMatrix;
	v_fogBegin = 0;
	v_fogEnd = 16;
	v_fogColor = vec3(0.1, 0.4, 0.7);
}
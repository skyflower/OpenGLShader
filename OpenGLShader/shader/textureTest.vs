#version 430
attribute vec3 pos;
attribute vec2 texCoord;
attribute vec3 normal;


uniform mat4 MVP;
uniform mat4 modelMatrix;

varying vec2 vST;
varying vec3 v_pos;
varying vec3 v_normal;

void main()
{
	mat3 normalMatrix = inverse(transpose(mat3(modelMatrix)));
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0) * MVP;
	vST = vec2(texCoord.x, 1 - texCoord.y);
	v_pos = pos;
	v_normal = normal * normalMatrix;
}
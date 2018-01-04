
attribute vec3 pos;
attribute vec3 normal;

uniform mat4 MVP;
uniform mat4 modelMatrix;

varying vec3 v_normal;
varying vec3 v_pos;

void main()
{
	mat3 NM = transpose(inverse(mat3(modelMatrix)));
	v_normal = normal * NM;
	
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0)* MVP;
	v_pos = vec4(pos.x, pos.y, pos.z ,1.0) * modelMatrix;
}
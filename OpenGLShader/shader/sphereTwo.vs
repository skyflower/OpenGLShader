
attribute vec3 pos;
attribute vec4 normal;

uniform mat4 MVP;
uniform mat4 modelMatrix;

varying vec3 v_normal;
varying vec3 v_pos;
varying vec4 v_lightPos;


void main()
{
	mat3 NM = transpose(inverse(mat3(modelMatrix)));
	//mat3 NM = inverse(transpose(mat3(modelMatrix)));
	//mat3 NM = transpose(inverse(mat3(modelMatrix)));
	v_normal = normal.xyz * NM;
	
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0)* modelMatrix;
	v_pos = vec4(pos.x, pos.y, pos.z ,1.0) * modelMatrix;
	vec4 tmpLightPos = vec4(1.0, 1.0, 1.0, 1.0);
	v_lightPos = tmpLightPos;// * modelMatrix;
	
}
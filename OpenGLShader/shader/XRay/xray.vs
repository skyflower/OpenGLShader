

attribute vec3 pos;
attribute vec4 normal;

uniform mat4 MVP;
uniform mat4 modelMatrix;

varying vec4 v_worldPos;
varying vec3 v_normal;

void main()
{
	v_worldPos = vec4(pos, 1.0) * modelMatrix;
	gl_Position = vec4(pos, 1.0) * MVP;
	
	mat3 NM = transpose(inverse(mat3(modelMatrix)));
	v_normal = normal.xyz * NM;
}

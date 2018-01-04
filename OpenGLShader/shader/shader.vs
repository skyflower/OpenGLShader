
attribute vec3 pos;
attribute vec3 normal;
attribute vec2 texCoord;
attribute vec3 color;

uniform mat4 MVP;
uniform mat4 modelMatrix;
//uniform mat4 NM;

//varying vec4 V_Color;
varying vec3 V_Normal;
varying vec4 V_WorldPos;
varying vec2 V_TexCoord;

void main()
{
	mat3 NM = transpose(inverse(mat3(modelMatrix)));
	
    V_Normal = normal * NM;
	
	gl_Position = vec4(pos.x, pos.y, pos.z - 1, 1.0) * modelMatrix;//MVP;
    V_TexCoord = texCoord;
	V_WorldPos = vec4(pos, 1.0) * modelMatrix;//MVP;
    
}
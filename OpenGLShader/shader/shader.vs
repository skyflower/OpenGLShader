
attribute vec3 pos;
attribute vec3 normal;
attribute vec2 texCoord;
attribute vec3 color;

uniform mat4 MVP;
uniform mat4 NM;

//varying vec4 V_Color;
varying vec3 V_Normal;
varying vec4 V_WorldPos;
varying vec2 V_TexCoord;

void main()
{

    vec4 tmp = NM * vec4(normal.x, normal.y, normal.z, 1.0);
	V_Normal = tmp.xyz;
	//V_Normal = mat3(NM) * normal;
    gl_Position = vec4(pos.x, pos.y, pos.z - 1, 1.0) * MVP;
    V_TexCoord = texCoord;
	V_WorldPos = vec4(pos, 1.0) * MVP;
    
}
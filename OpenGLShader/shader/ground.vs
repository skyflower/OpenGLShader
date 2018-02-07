
attribute vec3 pos;
attribute vec2 texCoord;
attribute vec3 normal;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

varying vec2 V_TexCoord;

void main()
{
	int id = 0;
	vec2 texcoord = vec2((((gl_VertexID - 1) & 2) >> 1), ((gl_VertexID & 2) >> 1));
	vec2 Extexcoord = vec2(texcoord.x - 0.5, texcoord.y - 0.5);
	
	vec4 fixPos = vec4(pos.xyz, 1.0);
	vec2 tmpPos = vec2(Extexcoord.x * 4, Extexcoord.y * 4);
    gl_Position = (fixPos + vec4(tmpPos.x, 0, tmpPos.y, 1.0)) * modelMatrix - vec4(0, 1, 0, 0);
   
	V_TexCoord = vec2(texcoord.x, 1 - texcoord.y);
}
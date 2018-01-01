#version 430

struct Vertex
{
	vec4 pos;
};

layout(std140, binding=0)buffer Mesh
{
	Vertex vertexes[];
}mesh;

varying vec2 V_TexCoord;
uniform mat4 MVP;


void main()
{
	
	int id = 0; //gl_VertexID
	vec2 texcoord = vec2((((gl_VertexID - 1) & 2) >> 1), ((gl_VertexID & 2) >> 1));
	vec2 Extexcoord = vec2(texcoord.x - 0.5, texcoord.y - 0.5);
	//float tmpSize = 4.32;
	//4.41828, 3.3137
	vec4 fixPos = vec4(mesh.vertexes[0].pos.xyz, 1.0);
	vec2 tmpPos = vec2(Extexcoord.x * 4.41828, Extexcoord.y * 3.3137);
    gl_Position = (fixPos + vec4(tmpPos, 0, 1.0)) * MVP;
   
	V_TexCoord = vec2(texcoord.x, 1 - texcoord.y);
}
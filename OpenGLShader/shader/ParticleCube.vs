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
	int tmpID = gl_VertexID; //gl_VertexID   // gl_GlobalInvocationID
	int spriteID = (tmpID >> 2);
	vec4 spritePos = vec4(mesh.vertexes[spriteID].pos.xyz, 1.0);
	vec4 spritePosInView = spritePos;
	
	vec2 texcoord = vec2((((tmpID - 1) & 2) >> 1), ((tmpID & 2) >> 1));
	float spriteSize = 0.02;
	vec4 fixPos = spritePosInView + vec4(vec2(texcoord * 2.0 - 1.0) * spriteSize, 0, 0);
	V_TexCoord = texcoord;
	
	gl_Position = fixPos * MVP;
	
}
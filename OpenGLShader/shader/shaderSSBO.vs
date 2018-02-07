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
uniform mat4 projectionMatrix;


void main()
{
	
	int id = 0;
	vec2 texcoord = vec2(gl_VertexID & 1, (gl_VertexID & 2) >> 1);
	vec2 Extexcoord = vec2(texcoord.x - 0.5, texcoord.y - 0.5);
	float tmpSize = 0.5;
	
	vec4 fixPos = vec4(mesh.vertexes[0].pos.xyz, 1.0);
	vec2 tmpPos = tmpSize * Extexcoord;
    gl_Position = fixPos * projectionMatrix + vec4(tmpPos, 0, 1.0);
    
	V_TexCoord = texcoord;
}
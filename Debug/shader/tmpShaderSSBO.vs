#version 430

struct Vertex
{
	vec4 pos;
	vec4 normal;
	vec4 texcoord;
};

layout(std140, binding=0)buffer Mesh
{
	Vertex vertexes[];
}mesh;

varying vec2 V_TexCoord;
uniform mat4 MVP;

double Logistic(double a, double x, int k)
{
	int i = 0;
	for(i = 0; i < 200 + k; ++i)
	{
		x = a * x * (1 - x);
	}
	return x;
}


void main()
{
	//gl_PointSize = 200;
	int id = 0;
	vec2 texcoord = vec2(gl_VertexID & 1, (gl_VertexID & 2) >> 1);
	vec2 Extexcoord = vec2(texcoord.x - 0.5, texcoord.y - 0.5);
	float tmpSize = 0.5;
	//double randValueX = Logistic(3.99999, 0.1234, gl_VertexID);
	//double randValueY = Logistic(3.99999, randValueX, gl_VertexID);
	vec4 fixPos = vec4(mesh.vertexes[0].pos.xyz, 1.0);
	vec2 tmpPos = tmpSize * Extexcoord;
    gl_Position = fixPos * MVP + vec4(tmpPos, 0, 1.0);//vec4(pos.x, pos.y, pos.z, 1.0) * MVP;
    //V_TexCoord = vec2(texCoord.x, 1 - texCoord.y);
	V_TexCoord = texcoord;//mesh.vertexes[gl_VertexID].texcoord.xy;
}
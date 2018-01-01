#version 430

struct Vertex
{
	vec4 pos;
	vec4 noise;
};

layout(std140, binding=0)buffer Mesh
{
	Vertex vertexes[];
}mesh;



varying vec2 V_TexCoord;
uniform mat4 MVP;

void main()
{
	vec4 fixPosTri = vec4(mesh.vertexes[0].pos.xyz, 1.0);

	float baseScale = 1;

	int baseID = int(gl_VertexID / 4);
	vec4 basefixPos = vec4(mesh.vertexes[baseID].noise.xyz + 0.875199999999, 0);
	basefixPos.x = basefixPos.x - int(basefixPos.x);
	basefixPos.y = basefixPos.y - int(basefixPos.y);
	basefixPos.z = basefixPos.z - int(basefixPos.z);
	
	int i = 0;
	

	if((baseID & 0x4) != 0)
	{
		basefixPos.z = -basefixPos.z;
	}
	if((baseID & 0x2) != 0)
	{
		basefixPos.x = -basefixPos.x;
	}
	if((baseID & 0x1) != 0)
	{
		basefixPos.y = -basefixPos.y;
	}

	basefixPos.x = basefixPos.x * baseScale;
	basefixPos.y = basefixPos.y * baseScale;
	basefixPos.z = basefixPos.z * baseScale;
	basefixPos.w = 0;

	float tmpScale = 0.2;
	vec4 tmpPos = vec4(1.0, 1.0, 0.0, 1.0);
	vec2 texcoord = vec2(0, 0);
	int tmpLeft = gl_VertexID - 4 * baseID;
	if((tmpLeft & 0x2) != 0)
	{
		tmpPos.y = -tmpPos.y;
		texcoord.x = 1;
	}
	if((tmpLeft & 0x1) != 0)
	{
		tmpPos.x = -tmpPos.x;
		texcoord.y = 1;
	}
	
	tmpPos.x = tmpPos.x * tmpScale;
	tmpPos.y = tmpPos.y * tmpScale;
	
	//gl_Position = fixPosTri * MVP + basefixPos + tmpPos;
	gl_Position = (fixPosTri + basefixPos) * MVP + tmpPos;
	
	//= vec2(gl_VertexID & 1, (gl_VertexID & 2) >> 1);
	//vec2 Extexcoord = vec2(texcoord.x - 0.5, texcoord.y - 0.5);
	
	//V_TexCoord = vec2(texCoord.x, 1 - texCoord.y);

	V_TexCoord = texcoord;//mesh.vertexes[gl_VertexID].texcoord.xy;
}
#version 330
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

out vec2 V_TexCoord;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

void main()
{
	vec4 pos = gl_in[0].gl_Position;
	
	float spriteSize = 1.5;
	
	gl_Position = (pos + vec4(-spriteSize, -spriteSize, 0, 1)) * modelMatrix;//projectionMatrix;
	V_TexCoord = vec2(0.0, 0.0);
	EmitVertex();
	
	gl_Position = (pos + vec4(spriteSize, -spriteSize, 0, 1)) * modelMatrix;//projectionMatrix;
	V_TexCoord = vec2(1.0, 0.0);
	EmitVertex();
	
	gl_Position = (pos + vec4(-spriteSize, spriteSize, 0, 1)) * modelMatrix;//projectionMatrix;
	V_TexCoord = vec2(0.0, 1.0);
	EmitVertex();
	
	gl_Position = (pos + vec4(spriteSize, spriteSize, 0, 1)) * modelMatrix;//projectionMatrix;
	V_TexCoord = vec2(1.0, 1.0);
	EmitVertex();
	
	EndPrimitive();
}
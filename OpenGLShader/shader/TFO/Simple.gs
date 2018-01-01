#version 330
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

out vec2 V_TexCoord;
out vec4 secondColorTwo;
in vec4 secondColor[];

void main()
{
	vec4 pos = gl_in[0].gl_Position;
	
	float spriteSize = 0.2;
	
	gl_Position = (pos + vec4(-spriteSize, -spriteSize, 0, 1));
	V_TexCoord = vec2(0.0, 0.0);
	secondColorTwo = secondColor[0];
	EmitVertex();
	
	gl_Position = (pos + vec4(spriteSize, -spriteSize, 0, 1));
	V_TexCoord = vec2(1.0, 0.0);
	secondColorTwo = secondColor[0];
	EmitVertex();
	
	gl_Position = (pos + vec4(-spriteSize, spriteSize, 0, 1));
	V_TexCoord = vec2(0.0, 1.0);
	secondColorTwo = secondColor[0];
	EmitVertex();
	
	gl_Position = (pos + vec4(spriteSize, spriteSize, 0, 1));
	V_TexCoord = vec2(1.0, 1.0);
	secondColorTwo = secondColor[0];
	EmitVertex();
	
	EndPrimitive();
}
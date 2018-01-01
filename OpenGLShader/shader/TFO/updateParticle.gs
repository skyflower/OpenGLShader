#version 330
layout(points) in;
layout(points, max_vertices = 4) out;

in vec4 MessLife[];

varying vec4 mess;


void main()
{
	if(MessLife[0].x < 5.0)
	{
		gl_Position = gl_in[0].gl_Position;
		mess = MessLife[0];
		EmitVertex();
	}
	
}
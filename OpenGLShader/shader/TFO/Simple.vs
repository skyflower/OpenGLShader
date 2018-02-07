
attribute vec4 pos;
attribute vec4 life;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

varying vec4 secondColor;

void main()
{
	secondColor = life;
    gl_Position = pos * modelMatrix;//projectionMatrix;
    
}

attribute vec4 pos;
attribute vec4 life;

varying vec4 mess;

//uniform mat4 MVP;

void main()
{
	mess = life;
    gl_Position = pos;
    
}
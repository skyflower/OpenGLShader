
attribute vec4 pos;
uniform mat4 MVP;

void main()
{
    gl_Position = pos * MVP;
}

attribute vec4 pos;
uniform mat4 projectionMatrix;

void main()
{
    gl_Position = pos * projectionMatrix;
}
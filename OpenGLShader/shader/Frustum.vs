
attribute vec3 pos;

uniform mat4 MVP;
uniform mat4 modelMatrix;

void main()
{
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0) * modelMatrix;
}
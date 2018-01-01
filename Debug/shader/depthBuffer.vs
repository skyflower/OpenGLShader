
attribute vec3 pos;
attribute vec2 texCoord;
uniform mat4 MVP;

varying vec2 V_TexCoord;

void main()
{
    gl_Position = vec4(pos, 1.0) * MVP;
	V_TexCoord = vec2(texCoord.x, 1 - texCoord.y);
}
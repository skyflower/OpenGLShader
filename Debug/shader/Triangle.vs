
attribute vec3 pos;
attribute vec2 texCoord;
attribute vec3 offset;
uniform mat4 MVP;

varying vec2 V_TexCoord;

void main()
{
	//gl_PointSize = 100.0;
    gl_Position = vec4(pos + offset, 1.0) * MVP;
    //gl_Position = vec3(tmp.xyzw);
	V_TexCoord = vec2(texCoord.x, 1 - texCoord.y);
}
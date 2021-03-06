
attribute vec3 pos;
attribute vec2 texCoord;

uniform mat4 projectionMatrix;

varying vec2 V_TexCoord;

void main()
{
	//gl_PointSize = 100.0;
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0) * projectionMatrix;
    //gl_Position = vec3(tmp.xyzw);
	V_TexCoord = vec2(texCoord.x, 1 - texCoord.y);
}
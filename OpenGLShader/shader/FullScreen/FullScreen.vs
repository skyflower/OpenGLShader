
attribute vec3 pos;
//attribute vec2 texCoord;

//uniform mat4 projectionMatrix;
//uniform mat4 modelMatrix;

uniform vec4 param;

varying vec2 V_TexCoord;

void main()
{
	vec4 worldPos = vec4(pos, 1.0);
	V_TexCoord = pos.xy + vec2(0.5, 0.5);
	worldPos.x += param.x;
	worldPos.y += param.y;
	worldPos.x *= param.z;
	worldPos.y *= param.z;
	gl_Position = worldPos;
}
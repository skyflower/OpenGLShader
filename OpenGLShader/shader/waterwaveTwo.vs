
attribute vec3 pos;
attribute vec3 normal;
attribute vec2 texCoord;

uniform mat4 MVP;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

uniform vec4 param;

varying vec3 vMCposition;
varying vec3 vECposition;
varying vec4 v_param; 
varying vec2 v_texcoord;
varying float uPd;


void main()
{
	float v_uPd = 4.0;
	vec4 tmpPos = vec4(pos, 1.0);
	tmpPos.z = param.w + param.z * cos(2 * 3.1415926 * pos.x / v_uPd - 2 * 3.1415926 * param.x);
	
	vMCposition = tmpPos.xyz;//(tmpPos * modelMatrix).xyz;
	vECposition = (tmpPos * viewMatrix).xyz;
	
	v_param = param;
	v_texcoord = vec2(texCoord.x, 1 - texCoord.y);
	uPd = v_uPd;
    gl_Position = tmpPos * MVP;
}
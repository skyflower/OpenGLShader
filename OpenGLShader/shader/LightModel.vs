
attribute vec3 pos;
attribute vec3 normal;
attribute vec2 texCoord;
attribute vec3 color;

uniform mat4 MVP;
uniform mat4 NM;

varying vec3 V_Normal;
varying vec4 V_WorldPos;
varying vec2 V_TexCoord;
varying vec4 V_LightPos;
varying vec3 V_LightDirection;
varying float V_SpotAngle;

void main()
{
	vec4 LightPos = vec4(-0.5, 1.0, 1.0, 1.0);
	vec3 tmpLightDirection = vec3(1.0, -0.5, -0.5) - LightPos.xyz;
	V_LightDirection = tmpLightDirection;
	V_SpotAngle = 10 * 3.1415926 / 180;
    V_Normal = normal * mat3(NM);
    gl_Position = vec4(pos, 1.0) * MVP;
    
	V_TexCoord = texCoord;
	V_WorldPos = vec4(pos, 1.0) * MVP;
   
	V_LightPos = LightPos;
}
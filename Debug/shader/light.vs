attribute vec3 pos;
attribute vec2 texcoord;
attribute vec3 normal;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 NM;
//varying vec4 V_Color;
varying vec4 V_Normal;
void main()
{
    V_Normal = vec4(mat3(NM) * normal, 1.0);
    //vec4 tmp = 
    gl_Position = P*V*M*vec4(pos.x, pos.y, pos.z - 10, 1.0);
    //V_Color = vec4(color, 0.5, 1.0);
}
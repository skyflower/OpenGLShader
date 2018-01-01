
attribute vec3 pos;
attribute vec3 normal;
attribute vec2 texCoord;
attribute vec3 color;

uniform mat4 MVP;
uniform mat4 NM;

//varying vec4 V_Color;
varying vec3 V_Normal;
varying vec4 V_WorldPos;
varying vec2 V_TexCoord;

void main()
{

    V_Normal = mat3(NM) * normal;
    gl_Position = vec4(pos, 1.0) * MVP;
    //gl_Position = tmp;//normalize(tmp);//vec4(tmp.x / tmp.w, tmp.y / tmp.w, tmp.z / tmp.w, 1.0);
	V_TexCoord = texCoord;
	V_WorldPos = MVP*vec4(pos, 1.0);
    //V_Color = AmbientColor + diffuseColor;//vec4(color, 0.5, 1.0);

}
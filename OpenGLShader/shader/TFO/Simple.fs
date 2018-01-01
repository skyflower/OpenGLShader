#version 430

varying vec2 V_TexCoord;
uniform sampler2D U_MainTexture;
varying vec4 secondColorTwo;
void main()
{
    gl_FragColor = vec4(vec3(0.2, 0.3, 0.8), texture2D(U_MainTexture, V_TexCoord.xy).a* sin(secondColorTwo.x));
}
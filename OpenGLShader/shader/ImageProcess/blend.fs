uniform sampler2D texSampler;
uniform sampler2D texSamplerAux;

varying vec2 V_TexCoord;
void main()
{
	vec4 color = texture2D(texSampler, V_TexCoord);
	vec4 colorTwo = texture2D(texSamplerAux, V_TexCoord);
    gl_FragColor = color * colorTwo.a + colorTwo * (1 - colorTwo.a);
}
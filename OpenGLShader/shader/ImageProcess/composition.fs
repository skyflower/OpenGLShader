uniform sampler2D texSampler;
uniform sampler2D texSamplerAux;

varying vec2 V_TexCoord;
void main()
{
	vec4 lumCoeff = vec4(0.2125, 0.7154, 0.0721, 1.0);
	vec4 color = texture2D(texSampler, V_TexCoord);
	vec4 colorTwo = texture2D(texSamplerAux, V_TexCoord);
	float luminance = dot(color.xyz, lumCoeff.xyz);
	if(luminance < 0.45)
	{
		gl_FragColor = 2 * color * colorTwo;
	}
	else if(luminance > 0.55)
	{
		gl_FragColor = vec4(1.0) - 2 * (vec4(1.0) - color) * (vec4(1.0) - colorTwo);
	}
	else
	{
		vec4 color1 = 2 * color * colorTwo;
		vec4 color2 = vec4(1.0) - 2 * (vec4(1.0) - color) * (vec4(1.0) - colorTwo);
		gl_FragColor = mix(color1, color2, (luminance - 0.45) * 10);
	}
}
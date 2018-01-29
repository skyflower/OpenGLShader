uniform sampler2D texSampler;



varying vec2 V_TexCoord;
void main()
{
	vec4 color = texture2D(texSampler, V_TexCoord) * 1.5;
	
	if((color.r > 1) && (color.g > 1) && (color.b > 1) && (color.a > 1))
	{
		gl_FragData[1] = color;
	}
	else
	{
		gl_FragData[0] = color;
	}
}
uniform sampler2D texSampler;

varying vec2 V_TexCoord;
void main()
{
	vec4 color = texture2D(texSampler,V_TexCoord);
	if((color.r > 1) || (color.g > 1) || (color.b > 1) || (color.a > 1))
	{
		discard;
	}
    gl_FragColor = color;
}
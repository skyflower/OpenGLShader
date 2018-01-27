uniform sampler2D texSampler;

varying vec2 V_TexCoord;
void main()
{
	vec4 maxValue = vec4(0.0);
	int coreSize = 10;
	int halfSize = -coreSize / 2;
	int x = 0;
	int y = 0;
	float texOffset = 1 / 300.0;
	for(y = 0; y < coreSize; ++y)
	{
		for(x = 0; x < coreSize; ++x)
		{
			vec4 currentAlpha = texture2D(texSampler, V_TexCoord + vec2(halfSize + x, halfSize + y) * texOffset);
			maxValue = max(maxValue, currentAlpha);
		}
	}
    gl_FragColor = maxValue;//vec4(texture2D(texSampler,V_TexCoord).xyz, minValue);
}
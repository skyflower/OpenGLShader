uniform sampler2D texSampler;

varying vec2 V_TexCoord;
void main()
{
	float Factor[9];
	Factor[6] = 0;Factor[7] = 1;Factor[8] = 0;
	Factor[3] = 1;Factor[4] = -4;Factor[5] = 1;
	Factor[0] = 0;Factor[1] = 1;Factor[2] = 0;
	
	vec4 Value = vec4(0.0);
	int coreSize = 3;
	int halfSize = -coreSize / 2;
	int x = 0;
	int y = 0;
	float texOffset = 1 / 200.0;
	for(y = 0; y < coreSize; ++y)
	{
		for(x = 0; x < coreSize; ++x)
		{
			vec4 currentAlpha = texture2D(texSampler, V_TexCoord + vec2(halfSize + x, halfSize + y) * texOffset);
			Value = Value + currentAlpha * Factor[(x * 3 + y)];
		}
	}
	
    gl_FragColor = Value;//vec4(texture2D(texSampler,V_TexCoord).xyz, minValue);
	
}
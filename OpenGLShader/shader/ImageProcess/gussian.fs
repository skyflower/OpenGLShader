uniform sampler2D texSampler;

varying vec2 V_TexCoord;
void main()
{
	float Factor[9];
	Factor[0] = 1;
	Factor[1] = 2;
	Factor[2] = 1;
	Factor[3] = 2;
	Factor[4] = 4;
	Factor[5] = 2;
	Factor[6] = 1;
	Factor[7] = 2;
	Factor[8] = 1;
	
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
	
    gl_FragColor = Value / 16;//vec4(texture2D(texSampler,V_TexCoord).xyz, minValue);
	
}
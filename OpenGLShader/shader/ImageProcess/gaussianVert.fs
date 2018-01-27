uniform sampler2D texSampler;

varying vec2 V_TexCoord;
void main()
{
	float Factor[5];
	Factor[0] = 1;
	Factor[1] = 1;
	Factor[2] = 2;
	Factor[3] = 1;
	Factor[4] = 1;
	
	vec4 Value = vec4(0.0);
	int coreSize = 5;
	int halfSize = -coreSize / 2;
	int y = 0;
	
	vec2 texSize = textureSize(texSampler, 0);
	float texOffset = 1 / texSize.y;
	for(y = 0; y < coreSize; ++y)
	{
		vec4 currentAlpha = texture2D(texSampler, V_TexCoord + vec2(0, halfSize + y) * texOffset);
		Value = Value + currentAlpha * Factor[y];
	}
	
    gl_FragColor = Value / 6;//vec4(texture2D(texSampler,V_TexCoord).xyz, minValue);
	
}
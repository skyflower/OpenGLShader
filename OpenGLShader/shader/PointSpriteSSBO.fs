uniform sampler2D texSampler;
uniform sampler2D skyTexSampler;

varying vec2 V_TexCoord;
void main()
{
	//float distance = length(2.0 * (V_TexCoord - 0.5)) * 10.0;
	//float alpha = exp(- 5 * distance * distance);
	//if(alpha < 0.01) discard;
    //gl_FragColor = vec4(0.4, 0.3, 1.0, alpha);
	gl_FragColor = vec4(texture2D(texSampler,V_TexCoord).rgb, 1.0) * vec4(texture2D(skyTexSampler,V_TexCoord).rgb, 1.0);
	//gl_FragColor = texture2D(texSampler,gl_PointCoord.xy);
}
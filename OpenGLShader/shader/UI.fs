uniform sampler2D texSampler;

varying vec2 V_TexCoord;
void main()
{
    gl_FragColor = texture2D(texSampler,V_TexCoord);
	//gl_FragColor = texture2D(texSampler,gl_PointCoord.xy);
}
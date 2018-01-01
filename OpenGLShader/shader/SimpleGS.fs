#version 430

varying vec2 V_TexCoord;
void main()
{
	float distance = length(2.0 * (V_TexCoord - 0.5)) * 10.0;
	float alpha = exp(- 5 * distance * distance);
	if(alpha < 0.1) discard;
    gl_FragColor = vec4(0.4, 0.3, 1.0, alpha);
	
}
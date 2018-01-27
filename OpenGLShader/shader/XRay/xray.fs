

varying vec4 v_worldPos;
varying vec3 v_normal;


void main()
{
	vec3 eyePos = vec3(0, 0, 0);
	vec3 ToEyeVec = eyePos - v_worldPos.xyz;
	ToEyeVec = normalize(ToEyeVec);
	vec3 normalVec = normalize(v_normal);
	float cosAngle = dot(normalVec, ToEyeVec);
	float alpha = 0.0;
	if(cosAngle > 0)
	{
		alpha = 1 - cosAngle;
		//alpha = pow(alpha, 2);
	}
	gl_FragColor = vec4(0.3, 0.3, 0.3, alpha);
}
#version 430
varying vec3 v_normal;
varying vec2 vST;
varying vec3 v_pos;

float GetIntensity(vec3 pos)
{
	vec3 LightPos = vec3(10, 10, 10);
	vec3 LightDirector = LightPos - pos;
	vec3 unitNormal = normalize(v_normal);
	LightDirector = normalize(LightDirector);
	float dotResult = max(0, dot(unitNormal, LightDirector));
	return dotResult;
}


bool colorTest(vec2 st)
{
	float x = st.x;
	float y = st.y;
	
	int k = 2;
	bool ret = false;
	float uLimit = 1.4;
	int i = 0;
	for(i = 0; i < k; ++i)
	{
		float newX = x * x - y * y;
		float newY = abs(2 * x * y) - 1;
		x = newX;
		y = newY;
		if(x * x + y * y > uLimit)
		{
			ret = true;
			break;
		}
	}
	if(i == k)
	{
		return false;
	}
	return ret;
}


void main()
{

	float s_Mag_Factor = 4.0;
	float t_Mag_Factor = 4.0;
	vec4 theColor;
	vec2 st = vST;
	st.s = fract( st.s * s_Mag_Factor );
	st.t = fract( st.t * t_Mag_Factor );
	vec4 srcColor = vec4( 0.2, 0.3, 0.7, 1.0);
	vec4 desColor = vec4( 0.9, 0.6, 0.4, 1.0);
	if ( colorTest( st ) )
	{
		theColor = srcColor; // color of brick	
	}
	else
	{
		float dotResult = GetIntensity(v_pos);
		dotResult = 0.4;
		theColor = mix(srcColor, desColor, dotResult); // color of mortar
	}
	gl_FragColor = vec4( theColor.xyz, 1.0);
}
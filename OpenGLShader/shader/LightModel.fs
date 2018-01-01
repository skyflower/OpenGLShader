uniform sampler2D texSampler;

varying vec3 V_Normal;
varying vec4 V_WorldPos;
varying vec2 V_TexCoord;
varying vec4 V_LightPos;

varying vec3 V_LightDirection;
varying float V_SpotAngle;

uniform int nLightType;

vec4 Ambient()
{
    vec4 AmbientLightColor = vec4(0.5, 0.5, 0.5, 1.0);
    vec4 AmbientMaterial = vec4(0.5, 0.5, 0.5, 1.0);
    vec4 AmbientColor = AmbientLightColor * AmbientMaterial;
	return AmbientColor;
}

vec4 diffuse(vec3 LightPos)
{
    vec3 L = normalize(LightPos);
    vec3 n = normalize(V_Normal);
	vec4 diffuseLightColor = vec4(0.9, 0.9, 0.9, 1.0);
    vec4 diffuseMaterial = vec4(0.8, 0.8, 0.8, 1.0);
    vec4 diffuseColor = diffuseLightColor * diffuseMaterial * max(0, dot(L, n));
	return diffuseColor;
}

vec4 specular(vec3 LightPos)
{
    vec3 L = normalize(LightPos);
   
    vec3 n = normalize(V_Normal);
	vec4 SpecularLightColor = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 SpecularMaterial = vec4(0.8, 0.8, 0.8, 1.0);
    vec3 reflectDir = normalize(reflect(L, n));
    vec3 viewDir = normalize(vec3(0, 0, 0) - V_WorldPos.xyz);
	
	float cosAlpha = dot(reflectDir, viewDir);
	if(cosAlpha < 0)
	{
		cosAlpha = 0;
	}
    vec4 SpecularColor = SpecularLightColor * SpecularMaterial * cosAlpha;//pow(cosAlpha, 20.0f);
	return SpecularColor;
}


void main()
{
	gl_FragColor = vec4(0, 0, 0, 0);
	
	vec3 LightDirection;
	if(V_LightPos.w == 0.0)
	{
		LightDirection = -V_LightPos.xyz;
	}
	else
	{
		LightDirection = V_WorldPos.xyz - V_LightPos.xyz;
	}
	if((nLightType == 1) || (nLightType == 2))
	{
		gl_FragColor = gl_FragColor + Ambient()+ diffuse(LightDirection)+ specular(LightDirection);
	}
	else if(nLightType == 3)
	{
		//gl_FragColor = gl_FragColor + Ambient() + diffuse(LightDirection);
		float k1 = 1;
		float k2 = 1;
		float k3 = 2;
		
		float angle = max(dot(V_LightDirection, LightDirection), 0);
		if(angle <= cos(V_SpotAngle))
		{
			k1 = 0;
		}
		else
		{
			k1 = angle - cos(V_SpotAngle);
		}
		
		float distance = length(LightDirection);
		distance = pow(distance, 4);
		//k1 = pow(k1, );
		vec4 SpecularLightColor = vec4(1.0, 1.0, 1.0, 1.0);
		vec4 SpecularMaterial = vec4(0.8, 0.8, 0.8, 1.0);
		
		vec4 SpecularColor = k3 * k1 * SpecularLightColor * SpecularMaterial / (1 + distance + k2 * distance * distance);
		gl_FragColor = gl_FragColor + SpecularColor;
	}
	
	//  1 : parallel light   2 : direction light  3 : spot light
	
	
	
	//gl_FragColor = gl_FragColor;//*texture2D(texSampler,V_TexCoord);
}


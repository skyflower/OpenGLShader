#version 430

varying vec3 V_Normal;
varying vec4 V_WorldPos;
varying float nLightType;

vec4 Ambient()
{
    vec4 AmbientLightColor = vec4(0.4, 0.4, 0.4, 1.0);
    vec4 AmbientMaterial = vec4(0.4, 0.4, 0.4, 1.0);
    vec4 AmbientColor = AmbientLightColor * AmbientMaterial;
	return AmbientColor;
}

vec4 diffuse(vec3 LightPos, vec3 param)
{
    vec3 L = normalize(LightPos);
    vec3 n = normalize(V_Normal);
	vec4 diffuseLightColor = vec4(0.7, 0.7, 0.7, 1.0);
    vec4 diffuseMaterial = vec4(0.7, 0.7, 0.7, 1.0);
	float cosAngle = max(0, dot(L, n));
	if(cosAngle <= 0)
	{
		return vec4(0, 0, 0, 0);
	}
	float attention = 1.0;
	if((nLightType == 2) || (nLightType == 3))
	{
		float dist = length(LightPos);
		attention = 1 / (param.x + param.y * dist + param.z * pow(dist, 2));
	}
	
    vec4 diffuseColor = attention * diffuseLightColor * diffuseMaterial;
	return diffuseColor;
}

vec4 specular(vec3 LightPos, vec3 param)
{
    vec3 L = normalize(LightPos);
   
    vec3 n = normalize(V_Normal);
	vec4 SpecularLightColor = vec4(0.9, 0.9, 0.9, 1.0);
    vec4 SpecularMaterial = vec4(0.9, 0.9, 0.9, 1.0);
    vec3 reflectDir = normalize(reflect(L, n));
    vec3 viewDir = normalize(vec3(0, 0, 0) - V_WorldPos.xyz);
	
	float cosAlpha = dot(reflectDir, viewDir);
	if(cosAlpha < 0)
	{
		return vec4(0, 0, 0, 0);
	}
	float attention = 1.0;
	if((nLightType == 2) || (nLightType == 3))
	{
		float dist = length(LightPos);
		attention = 1 / (param.x + param.y * dist + param.z * pow(dist, 2));
	}
    vec4 SpecularColor = attention * SpecularLightColor * SpecularMaterial * pow(cosAlpha, 8.0f);
	return SpecularColor;
}


void main()
{
	vec4 V_LightPos = vec4(3, 3, 3, 1);
	vec3 V_LightDirection = vec3(-1, -1, -1);

	vec3 LightDirection = V_LightPos.xyz - V_WorldPos.xyz;
	vec3 param = vec3(0.05, 0.05, 0.05);
	if(nLightType == 3)
	{
		gl_FragData[0] = vec4(1.0, 1.0, 1.0, 1.0);
		gl_FragData[1] = vec4(1.0, 1.0, 1.0, 1.0);
	}
	else
	{
		gl_FragData[0] = Ambient()+ diffuse(LightDirection, param) + specular(LightDirection, param);
	}
}


#version 430
varying vec3 v_normal;
varying vec4 v_pos;

varying vec4 v_lightPos;
varying vec4 v_eyePos;

varying float v_fogBegin;
varying float v_fogEnd;
varying vec3 v_fogColor;


float CalcLinearFog(float dist)
{
	float factor = (dist - v_fogBegin)/(v_fogEnd - v_fogBegin);
	return 1.0 - clamp(factor, 0.0, 1.0);
}


vec4 ambient(vec4 lightPos, vec4 pos)
{
	vec4 ambientLight = vec4(0.4, 0.4, 0.4, 1);
	vec4 ambientMaterial = vec4(0.4, 0.4, 0.4, 1);
	return ambientLight * ambientMaterial;
}

vec4 diffuse(vec3 pointToLight, vec3 normal)
{
	vec4 diffuseLight = vec4(0.7, 0.7, 0.7, 1);
	vec4 diffuseMaterial = vec4(0.7, 0.7, 0.7, 1);
	pointToLight = normalize(pointToLight);
	normal = normalize(normal);

	return max(0, dot(pointToLight, normal)) * diffuseLight * diffuseMaterial;
}
vec4 specular(vec3 pointToLight, vec3 normal, vec3 eye)
{
	vec4 specularLight = vec4(0.9, 0.9, 0.9, 1.0);
	vec4 specularMaterial = vec4(0.9, 0.9, 0.9, 1.0);
	pointToLight = normalize(pointToLight);
	eye = normalize(eye);
	vec3 ref = normalize(pointToLight + eye);
	normal = normalize(normal);
	float factor = max(0, dot(normal, ref));
	factor = pow(factor, 32);
	return factor * specularLight * specularMaterial;
}


void main()
{
	vec4 lightPos = v_lightPos - v_pos;
	
	vec4 tmpFragColor = ambient(lightPos, v_pos) + diffuse(lightPos.xyz, v_normal);
	
	vec3 eye =vec3(0, 0, 0) - v_pos.xyz;
	tmpFragColor = tmpFragColor + specular(lightPos.xyz, v_normal, eye);
	
	float fogAlpha = CalcLinearFog(abs(v_eyePos.z / v_eyePos.w));
	fogAlpha = 1 - fogAlpha;
	gl_FragColor = vec4(fogAlpha * v_fogColor + (1 - fogAlpha) * tmpFragColor.xyz, 1.0);
	//gl_FragColor = mix(v_fogColor, vec3(tmpFragColor.xyz), fogAlpha);
}
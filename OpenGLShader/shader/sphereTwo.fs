
varying vec3 v_normal;
varying vec4 v_pos;

varying vec4 v_lightPos;


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
	
    gl_FragColor = ambient(lightPos, v_pos);
	gl_FragColor = gl_FragColor + diffuse(lightPos.xyz, v_normal);
	
	vec3 eye =vec3(0, 0, 0) - v_pos.xyz;
	gl_FragColor = gl_FragColor + specular(lightPos.xyz, v_normal, eye);
}
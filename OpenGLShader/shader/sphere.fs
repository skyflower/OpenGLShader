
varying vec3 v_normal;
varying vec3 v_pos;

vec4 ambient(vec4 lightPos, vec4 pos)
{
	vec4 ambientLight = vec4(0.5, 0.5, 0.5, 1);
	vec4 ambientMaterial = vec4(0.5, 0.5, 0.5, 1);
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
	vec3 ref = reflect(pointToLight, normal);
	eye = normalize(eye);
	ref = normalize(ref);
	return pow(max(0, dot(eye, ref)), 128) * specularLight * specularMaterial;
}


void main()
{
	vec4 lightPos = vec4(1, 1, -1, 0);
	
    gl_FragColor = ambient(lightPos, vec4(v_pos, 1.0)) + diffuse(lightPos.xyz, v_normal);
	
	vec3 eye = vec3(0, 0, 0) - v_pos.xyz;
	gl_FragColor = gl_FragColor + specular(lightPos.xyz, v_normal, eye);
}
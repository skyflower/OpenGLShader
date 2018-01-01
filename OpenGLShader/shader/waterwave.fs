
uniform sampler2D texSampler;
varying vec4 v_pos;
varying vec4 v_param;
varying vec2 v_texcoord;

float dfdx(vec3 pos)
{
	float dist = sqrt(pos.x * pos.x + pos.y * pos.y);
	float dx = v_param.z * cos(2 * dist / v_param.y - v_param.x / 500) * 2 / v_param.y * 2 * pos.x / dist;
	return dx;
}

float dfdy(vec3 pos)
{
	float dist = sqrt(pos.x * pos.x + pos.y * pos.y);
	float dy = v_param.z * cos(2 * dist / v_param.y - v_param.x / 500) * 2 / v_param.y * 2 * pos.y / dist;
	return dy;
}


vec3 calcNormal(vec3 pos)
{
	float dx = dfdx(pos);
	float dy = dfdy(pos);
	
	vec3 xtangent = vec3( 1.0, 0.0, dx );
	vec3 ytangent = vec3( 0.0, 1.0, dy );
	vec3 tmpNormal = normalize( cross( xtangent, ytangent ) );
	return tmpNormal;
}

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
    vec3 n = calcNormal(v_pos);
	vec4 diffuseLightColor = vec4(0.9, 0.9, 0.9, 1.0);
    vec4 diffuseMaterial = vec4(0.8, 0.8, 0.8, 1.0);
    vec4 diffuseColor = diffuseLightColor * diffuseMaterial * max(0, dot(L, n));
	return diffuseColor;
}

vec4 specular(vec3 LightPos)
{
    vec3 L = normalize(LightPos);
   
    vec3 n = calcNormal(v_pos);
	vec4 SpecularLightColor = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 SpecularMaterial = vec4(0.8, 0.8, 0.8, 1.0);
    vec3 reflectDir = normalize(reflect(L, n));
    vec3 viewDir = normalize(vec3(0, 0, 0) - v_pos.xyz);
	
	float cosAlpha = max(dot(reflectDir, viewDir), 0);
    vec4 SpecularColor = SpecularLightColor * SpecularMaterial * pow(cosAlpha, 128.0f);
	return SpecularColor;
}


void main()
{
	
	float maxValue = abs(v_param.w) + v_param.z;
	float minValue = abs(v_param.w) - v_param.z;
	//float blueColor = (abs(v_pos.z) - minValue)/(2 * (maxValue - minValue)) + 0.3;
	vec3 LightDirection = vec3(-1, -1, -1);
	
	vec4 lightColor = Ambient()+ diffuse(LightDirection) + specular(LightDirection);
	//gl_FragColor = vec4(lightColor.xyz, 1.0);
    //gl_FragColor = vec4(0, 0.0, blueColor, 1.0);
	gl_FragColor = texture2D(texSampler, v_texcoord) * lightColor;
	
}
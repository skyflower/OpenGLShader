uniform sampler2D texSampler;
subroutine vec4 SurfaceColor();

subroutine uniform SurfaceColor U_SurfaceColor;

varying vec3 V_Normal;
varying vec4 V_WorldPos;
varying vec2 V_TexCoord;


void main()
{
	gl_FragColor = U_SurfaceColor();
}


subroutine (SurfaceColor) vec4 Ambient()
{
	vec3 LightPos = vec3(10, 10, 0);
    vec3 L = normalize(LightPos);
    //L = normalize(L);
    vec3 n = normalize(V_Normal);
    vec4 AmbientLightColor = vec4(0.5, 0.5, 0.5, 1.0);
    vec4 AmbientMaterial = vec4(0.5, 0.5, 0.5, 1.0);
    vec4 AmbientColor = AmbientLightColor * AmbientMaterial;
	return AmbientColor * texture2D(texSampler,V_TexCoord);
}

subroutine (SurfaceColor) vec4 diffuse()
{
	vec3 LightPos = vec3(10, 10, 0);
    vec3 L = normalize(LightPos);
    //L = normalize(L);
    vec3 n = normalize(V_Normal);
	vec4 diffuseLightColor = vec4(0.9, 0.9, 0.9, 1.0);
    vec4 diffuseMaterial = vec4(0.4, 0.4, 0.4, 1.0);
    vec4 diffuseColor = diffuseLightColor * diffuseMaterial * max(0, dot(L, n));
	return diffuseColor + texture2D(texSampler,V_TexCoord);
}

subroutine (SurfaceColor) vec4 specular()
{
	vec3 LightPos = vec3(10, 10, 0);
    vec3 L = normalize(LightPos);
    //L = normalize(L);
    vec3 n = normalize(V_Normal);
	vec4 SpecularLightColor = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 SpecularMaterial = vec4(0.8, 0.8, 0.8, 1.0);
    vec3 reflectDir = normalize(reflect(-L, n));
    vec3 viewDir = normalize(vec3(0, 0, 0) - V_WorldPos.xyz);
    vec4 SpecularColor = SpecularLightColor * SpecularMaterial * pow(max(0.0f, dot(reflectDir, viewDir)), 8.0f);
	return SpecularColor + texture2D(texSampler,V_TexCoord);
}

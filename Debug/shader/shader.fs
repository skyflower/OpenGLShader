uniform sampler2D texSampler;

varying vec3 V_Normal;
varying vec4 V_WorldPos;
varying vec2 V_TexCoord;
void main()
{

    vec3 LightPos = vec3(10, 10, 0);
    vec3 L = normalize(LightPos);
    //L = normalize(L);
    vec3 n = normalize(V_Normal);
    vec4 AmbientLightColor = vec4(0.2, 0.2, 0.2, 1.0);
    vec4 AmbientMaterial = vec4(0.2, 0.2, 0.2, 1.0);
    vec4 AmbientColor = AmbientLightColor * AmbientMaterial;

    vec4 diffuseLightColor = vec4(0.7, 0.7, 0.7, 1.0);
    vec4 diffuseMaterial = vec4(0.7, 0.7, 0.7, 1.0);
    vec4 diffuseColor = diffuseLightColor * diffuseMaterial * max(0, dot(L, n));

    vec4 SpecularLightColor = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 SpecularMaterial = vec4(0.9, 0.9, 0.9, 1.0);
    vec3 reflectDir = normalize(reflect(-L, n));
    vec3 viewDir = normalize(vec3(0, 0, 0) - V_WorldPos.xyz);
    vec4 SpecularColor = SpecularLightColor * SpecularMaterial * pow(max(0.0f, dot(reflectDir, viewDir)), 8.0f);


    gl_FragColor = AmbientColor + texture2D(texSampler,V_TexCoord)*(diffuseColor + SpecularColor);
	//gl_FragData[0] = AmbientColor + texture2D(texSampler,V_TexCoord)*(diffuseColor + SpecularColor);
	//gl_FragData[1] = AmbientColor;
}
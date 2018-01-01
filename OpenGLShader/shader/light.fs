

//varying vec4 V_Color;
varying vec4 V_Normal;

void main()
{
    vec3 LightPos = vec3(0, 10, 0, 0);
    vec3 L = LightPos;
    L = normalize(L);
    vec3 n = normalize(V_Normal).xyz;
    vec4 AmbientLightColor = vec4(0.2, 0.2, 0.2, 1.0);
    vec4 AmbientMaterial = vec4(0.2, 0.2, 0.2, 1.0);
    vec4 AmbientColor = AmbientLightColor * AmbientMaterial;

    vec4 diffuseLightColor = vec4(0.6, 0.6, 0.6, 1.0);
    vec4 diffuseMaterial = vec4(0.2, 0.2, 0.2, 1.0);
    vec4 diffuseColor = diffuseLightColor * diffuseMaterial * max(0, dot(L, n));

    gl_FragColor = AmbientColor + diffuseColor;
}
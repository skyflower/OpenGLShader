#version 430

layout(std140, binding=0)buffer Pos
{
	vec4 pos[];
};

layout(std140, binding=1)buffer Vel
{
	vec4 vel[];
};


layout(local_size_x = 128, local_size_y = 1, local_size_z = 1)in;
uniform sampler3D U_MainTexture;

varying vec2 V_TexCoord;
uniform mat4 projectionMatrix;

vec3 CalV(vec3 p)
{
	vec3 vel = vec3(0.0);
	float gain = 1.0;
	float expend = 1.0;
	for(int i = 0; i < 4; ++i)
	{
		vel += texture(U_MainTexture, (p * expend) / 30.0).xyz * gain;
		expend *= 2.0;
		gain *= 0.5;
	}
	return vel;
}


void main()
{
	uint particleID = gl_GlobalInvocationID.x;
	if(particleID > 1048576) return;
	vec3 p = pos[particleID].xyz;
	vec3 v = vel[particleID].xyz;
	
	v += CalV(p * 10.0) * 0.001;
	p += v;
	
	v *= 0.95;
	
	
	pos[particleID] = vec4(p, 1.0);
	vel[particleID] = vec4(v, 1.0);
}
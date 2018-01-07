
uniform sampler2D f_sampler;

varying vec3 vMCposition;
varying vec3 vECposition;
varying vec4 v_param; 
varying vec2 v_texcoord;
varying float uPd;

const float TWOPI = 2.*3.14159265;

void main( )
{
	float uLightX = 1.0;
	float uLightY = 1.0;
	float uLightZ = 1.0;

	float uPhaseShift = 2.0;
	float uAmp0 = v_param.z;
	float uAmp1 = v_param.z;
	vec4 uColor = texture2D(f_sampler, v_texcoord);
	const vec3 C0 = vec3( -1.5, 0., 0. );
	const vec3 C1 = vec3( 1.5, 0., 0. );
	float uTime = v_param.x;
	
// first set of ripples:
	float rad0 = length( vMCposition - C0 ); // ripple center 0
	float H0 = -uAmp0 * cos( TWOPI*rad0/uPd - TWOPI*uTime );
	float u = -uAmp0 * (TWOPI/uPd) * sin(TWOPI*rad0/uPd -
	                                     TWOPI*uTime);

	float v = 0.;
	float w = 1.;
	float ang = atan(vMCposition.y - C0.y,
	                 vMCposition.x - C0.x );
	float u1 = dot( vec2(u,v), vec2(cos(ang), -sin(ang)) );
	float v1 = dot( vec2(u,v), vec2(sin(ang), cos(ang)) );
	float w1 = 1.;
// second set of ripples:
	float rad1 = length( vMCposition - C1 ); // ripple center 1
	float H1 = -uAmp1 * cos( TWOPI*rad1/uPd - TWOPI*uTime );
	u = -uAmp1*(TWOPI/uPd)*
	    sin(TWOPI*rad1/uPd-TWOPI*uTime-uPhaseShift);
	v = 0.;
	ang = atan( vMCposition.y - C1.y, vMCposition.x - C1.x );
	float u2 = dot( vec2(u,v), vec2(cos(ang), -sin(ang)) );
	float v2 = dot( vec2(u,v), vec2(sin(ang), cos(ang)) );
	float w2 = 1.;
// the sum is the normal:
	vec3 normal = normalize( vec3( u1+u2, v1+v2, w1+w2 ) );
	float LightIntensity =
	    abs(dot(normalize(vec3(uLightX,uLightY,uLightZ)-
	                      vECposition),normal));
	if( LightIntensity < 0.1 )
		LightIntensity = 0.1;
	gl_FragColor = vec4( LightIntensity*uColor.rgb, uColor.a );
}
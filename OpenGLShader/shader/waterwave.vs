
attribute vec3 pos;
attribute vec3 normal;
attribute vec2 texCoord;

uniform mat4 MVP;
uniform mat4 normalMatrix;
uniform vec4 param;

varying vec4 v_pos;
varying vec4 v_param;
varying vec2 v_texcoord;
varying vec3 v_normal;


//  z = f(x, y) = base.z + param.z * sin(2 * sqrt(x * x + y * y) / param.y - param.x / 500)
// dz/dx = param.z * cos(2 * sqrt(x * x + y * y) / param.y - param.x / 500) * 2 / param.y * 2 * x / sqrt(x * x + y * y);
float Zfunc(vec3 pos)
{
	float dist = sqrt(pos.x * pos.x + pos.y * pos.y);
	float tmpDist = dist * 2/param.y - param.x / 500;
	if(tmpDist < 0)
	{
		tmpDist = tmpDist + 2 * 3.1415926;
	}
	float circle =  sin(tmpDist);
	return pos.z + param.z * (3 - dist) / 3 * circle;
}

float dfdx(vec3 pos)
{
	float dist = sqrt(pos.x * pos.x + pos.y * pos.y);
	float dx = param.z * cos(2 * dist / param.y - param.x / 500) * 2 / param.y * 2 * pos.x / dist;
	return dx;
}

float dfdy(vec3 pos)
{
	float dist = sqrt(pos.x * pos.x + pos.y * pos.y);
	float dy = param.z * cos(2 * dist / param.y - param.x / 500) * 2 / param.y * 2 * pos.y / dist;
	return dy;
}



void main()
{
	vec4 tmpPos = vec4(pos, 1.0);
	
	tmpPos.z = Zfunc(pos);
	v_pos = tmpPos;
	v_param = param;
	v_texcoord = vec2(texCoord.x, 1 - texCoord.y);
	
	
	float dx = dfdx(pos);
	float dy = dfdy(pos);
	
	vec3 xtangent = vec3( 1.0, 0.0, dx );
	vec3 ytangent = vec3( 0.0, 1.0, dy );
	v_normal = normalize( cross( xtangent, ytangent ) );
	
    gl_Position = tmpPos * MVP;
}
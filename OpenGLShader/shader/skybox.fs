uniform sampler2D texOne;	// //	0, 1, 0
uniform sampler2D texTwo;	//0, -1, 0,
uniform sampler2D texThr;	//1, 0, 0
uniform sampler2D texFou;	// 0, 0, -1,
uniform sampler2D texFiv;	// -1, 0, 0
uniform sampler2D texSix;	//	0, 0, 1

//GLfloat tmpNormal[] = {
//	0, 1, 0,	0, -1, 0,
//	1, 0, 0,	0, 0, -1,
//	-1, 0, 0,	0, 0, 1
//	};
//	GLubyte tmpIndex[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 5, 4, 1, 2, 6, 5, 2, 3, 7, 6, 0, 3, 7, 4 };	
//

varying vec2 V_TexCoord;
varying vec3 scopePos;
varying vec3 V_Normal;

void main()
{
	vec3 normalOne = vec3(0, 1, 0);
	vec3 normalTwo = vec3(0, -1, 0);
	vec3 normalThr = vec3(1, 0, 0);
	vec3 normalFou = vec3(0, 0, -1);
	vec3 normalFiv = vec3(-1, 0, 0);
	vec3 normalSix = vec3(0, 0, 1);
	vec3 eyeToPoint = vec3(0, 0, 0) - scopePos;
	vec3 refVec = reflect(eyeToPoint, V_Normal);
	float tmpMaxComponent = abs(refVec.x);
	float minS = refVec.y;
	float minT = refVec.z;
	if(tmpMaxComponent < abs(refVec.y))
	{
		tmpMaxComponent = abs(refVec.y);
		minS = refVec.x;
		minT = refVec.z;
	}
	if(tmpMaxComponent < abs(refVec.z))
	{
		tmpMaxComponent = abs(refVec.z);
		minS = refVec.x;
		minT = refVec.y;
	}
	refVec = refVec / tmpMaxComponent;
	minS = abs(minS / tmpMaxComponent);
	minT = abs(minT / tmpMaxComponent);
	vec2 tmpTexCoord = vec2(minS, minT);
	int i = 0;
	float valueOne = dot(normalOne, refVec);
	float valueTwo = dot(normalTwo, refVec);
	float valueThr = dot(normalThr, refVec);
	float valueFou = dot(normalFou, refVec);
	float valueFiv = dot(normalFiv, refVec);
	float valueSix = dot(normalSix, refVec);
	float maxIndex = 0;
	float maxValue = valueOne;
	if(maxValue < valueTwo)
	{
		maxValue = valueTwo;
		maxIndex = 1;
	}
	if(maxValue < valueThr)
	{
		maxValue = valueThr;
		maxIndex = 2;
	}
	if(maxValue < valueFou)
	{
		maxValue = valueFou;
		maxIndex = 3;
	}
	if(maxValue < valueFiv)
	{
		maxValue = valueFiv;
		maxIndex = 4;
	}
	if(maxValue < valueSix)
	{
		maxValue = valueSix;
		maxIndex = 5;
	}
	if(maxIndex == 0)
	{
		gl_FragColor = texture2D(texOne, tmpTexCoord);
	}
	if(maxIndex == 1)
	{
		gl_FragColor = texture2D(texTwo, tmpTexCoord);
	}
	if(maxIndex == 2)
	{
		gl_FragColor = texture2D(texThr, tmpTexCoord);
	}
	if(maxIndex == 3)
	{
		gl_FragColor = texture2D(texFou, tmpTexCoord);
	}
	if(maxIndex == 4)
	{
		gl_FragColor = texture2D(texFiv, tmpTexCoord);
	}
	if(maxIndex == 5)
	{
		gl_FragColor = texture2D(texSix, tmpTexCoord);
	}
	//gl_FragColor = gl_FragColor + vec4(0.2, 0.2, 0.2, 1.0);
}
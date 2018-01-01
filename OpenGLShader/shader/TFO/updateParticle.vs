
attribute vec4 pos;
attribute vec4 life;

varying vec4 MessLife;

void main()
{
	MessLife  = vec4(0.01, 0, 0, 0) + life;
    gl_Position = pos + vec4(0, 0.005, 0, 0);
}
#version 330 core

#define PI 3.1415926535897932384626433832795

in vec2 TexCoords;
out vec4 color;
 
uniform int plasma_mode;
uniform float u_time;
uniform vec2 u_k;

vec4 PlasmaProc0();
vec4 PlasmaProc1();
vec4 PlasmaProc2();
vec4 PlasmaProc3();


void main()
{ 
	switch(plasma_mode)
	{
	case 1:
		color = PlasmaProc1();
	break;
	case 2:
		color = PlasmaProc2();
	break;
	case 3:
		color = PlasmaProc3();
	break;
	default:
		color = PlasmaProc0();
	break;
	}
}

vec4 PlasmaProc0()
{
	float v = 0.0;
    vec2 c = TexCoords * u_k - u_k/2.0;
    v += sin((c.x+u_time));
    v += sin((c.y+u_time)/2.0);
    v += sin((c.x+c.y+u_time)/2.0);
    c += u_k/2.0 * vec2(sin(u_time/3.0), cos(u_time/2.0));
    v += sin(sqrt(c.x*c.x+c.y*c.y+1.0)+u_time);
    v = v/2.0;
	vec3 col = vec3(sin(5*PI*v), sin(5*PI*v), sin(5*PI*v));
	return vec4(col*.5 + .5, 1);
}

vec4 PlasmaProc1()
{
	float v = 0.0;
    vec2 c = TexCoords * u_k - u_k/2.0;
    v += sin((c.x+u_time));
    v += sin((c.y+u_time)/2.0);
    v += sin((c.x+c.y+u_time)/2.0);
    c += u_k/2.0 * vec2(sin(u_time/3.0), cos(u_time/2.0));
    v += sin(sqrt(c.x*c.x+c.y*c.y+1.0)+u_time);
    v = v/2.0;
	vec3 col = vec3(0.5f, sin(PI*v), cos(PI*v));
	return vec4(col*.5 + .5, 1);
}

vec4 PlasmaProc2()
{
	float v = 0.0;
    vec2 c = TexCoords * u_k - u_k/2.0;
    v += sin((c.x+u_time));
    v += sin((c.y+u_time)/2.0);
    v += sin((c.x+c.y+u_time)/2.0);
    c += u_k/2.0 * vec2(sin(u_time/3.0), cos(u_time/2.0));
    v += sin(sqrt(c.x*c.x+c.y*c.y+1.0)+u_time);
    v = v/2.0;
	vec3 col = vec3(sin(PI*v), cos(PI*v), 0);
	return vec4(col*.5 + .5, 1);
}

vec4 PlasmaProc3()
{
	float v = 0.0;
    vec2 c = TexCoords * u_k - u_k/2.0;
    v += sin((c.x+u_time));
    v += sin((c.y+u_time)/2.0);
    v += sin((c.x+c.y+u_time)/2.0);
    c += u_k/2.0 * vec2(sin(u_time/3.0), cos(u_time/2.0));
    v += sin(sqrt(c.x*c.x+c.y*c.y+1.0)+u_time);
    v = v/2.0;
	vec3 col = vec3(sin(PI*v), sin(PI*v+2*PI/3), sin(PI*v+4*PI/3));
	return vec4(col*.5 + .5, 1);
}
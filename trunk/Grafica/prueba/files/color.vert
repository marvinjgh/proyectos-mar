#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 ver_uv;
layout (location = 3) in float s;

uniform mat4 m;
uniform mat4 p;
uniform mat4 n;
uniform vec4 incolor;

out vec4 color;
out vec3 vnormal;

void main()
{
    gl_Position = p*m*position;
	color = vec4(0.0,1.0,0.0,1.0);
	vec4 vRes = n*vec4(normal, 0.0);
	vnormal = vRes.xyz;
}
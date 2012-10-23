#version 330


layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 ver_uv;
layout (location = 3) in float s;

uniform mat4 m;
uniform vec4 tc;
out vec2 UV;
out float select;
out vec4 color;

void main()
{
    gl_Position = m*position;
	UV = ver_uv.xy;
	select=s;
	color = tc;
}
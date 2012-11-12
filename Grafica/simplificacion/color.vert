#version 330


layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 m;
uniform vec4 incolor;

out vec4 color;

void main()
{
    gl_Position = m*vec4(position,1.0f);
	color = incolor;
}
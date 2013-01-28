#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

uniform mat4 modelview;
uniform mat4 proyect;
uniform mat4 normalmatriz;
uniform vec4 incolor;

out vec4 color;
out vec3 vnormal;

void main()
{
    gl_Position = proyect*modelview*position;
	color = incolor;
	vec4 vRes = normalmatriz*vec4(normal, 0.0);
	vnormal = vRes.xyz;
}
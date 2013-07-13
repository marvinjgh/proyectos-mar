#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 m;
uniform mat4 p;
uniform vec4 incolor;

out vec4 color;
out smooth vec3 vnormal;
void main()
{
    gl_Position = p*m*vec4(position,1.0f);
	color = incolor;
	mat4 normalmatriz = transpose(inverse(m));
	vec4 vRes = normalmatriz*vec4(normal, 0.0);
	vnormal = vRes.xyz;
}
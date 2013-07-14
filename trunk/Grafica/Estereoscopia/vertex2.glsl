#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
uniform mat4 modelview;
uniform mat4 proyect;
uniform mat4 normalmatriz;
uniform vec4 incolor;

uniform float size;

out VertexData {
    //vec2 texCoord;
    vec3 normal;
    vec4 color;
}
VertexOut;
void main()
{
	vec4 p = vec4(position,1.0);
    vec4 vRes = normalmatriz*vec4(normal, 0.0);
    VertexOut.normal = vRes.xyz;
    gl_Position = proyect*modelview*p;
    VertexOut.color = incolor;
	gl_PointSize = size;
}

#version 330
 
layout(lines) in;
layout (line_strip) out;
in VertexData {
    vec4 p;
    vec4 normal;
	vec4 color;
}VertexIn[];

out VertexData {
    vec3 normal;
    vec4 color;
}VertexOut;

void main()
{
    gl_Position = gl_in[0].gl_Position ;
    VertexOut.normal = VertexIn[0].normal.xyz;
    VertexOut.color = VertexIn[0].color;
    EmitVertex();
    gl_Position = VertexIn[0].p;
    VertexOut.normal = VertexIn[0].normal.xyz;
    VertexOut.color = VertexIn[0].color;
    EmitVertex();
    EndPrimitive();

	gl_Position = gl_in[1].gl_Position ;
    VertexOut.normal = VertexIn[1].normal.xyz;
    VertexOut.color = VertexIn[1].color;
    EmitVertex();
    gl_Position = VertexIn[1].p;
    VertexOut.normal = VertexIn[1].normal.xyz;
    VertexOut.color = VertexIn[1].color;
    EmitVertex();
    EndPrimitive();
    //}
  
}

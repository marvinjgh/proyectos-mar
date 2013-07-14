#version 330

out vec4 outputColor;

uniform vec3 direccion;

in VertexData {
    vec3 normal;
	vec4 color;
} VertexIn;

void main()
{
		vec4 c = vec4(1,1,1,1);
		float fDiffuseIntensity = min(max(0.0, dot(normalize(VertexIn.normal), -direccion)),1.0);
		outputColor = max(VertexIn.color*fDiffuseIntensity,VertexIn.color*0.2); 
}

#version 330

in vec3 vnormal;
in vec4 color;
out vec4 outputColor;

void main()
{
		//outputColor = color;
		vec3 vDirection = vec3(-1.0,-1.0,1.0);
		float fDiffuseIntensity = max(0.0, dot(normalize(vnormal), -vDirection));
		outputColor = color*vec4(vec3(1.0,1.0,1.0)*(fDiffuseIntensity), 1.0);
}

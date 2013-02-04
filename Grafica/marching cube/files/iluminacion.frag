#version 330

smooth in vec3 vnormal;
in vec4 color;
struct SimpleDirectionalLight
{
    vec3 vColor;
    vec3 vDirection;
    float fAmbientIntensity;
};

out vec4 outputColor;

void main()
{

    float fDiffuseIntensity = max(0.0, dot(normalize(vnormal), -vec3(1.0,1.0,0.0)));
    
	outputColor = color*vec4(vec3(1.0,1.0,1.0)*(0.50+fDiffuseIntensity), 1.0);
    
}


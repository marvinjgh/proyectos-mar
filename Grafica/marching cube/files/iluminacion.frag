#version 330

in smooth vec3 vnormal;
in vec4 color;
struct LuzSimple
{
	vec3 ambiental;
	vec3 direccion;
	float intensidad;
};

uniform LuzSimple sol;

out vec4 outputColor;

void main()
{

	vec3 aux = sol.direccion;
	aux.xy = -aux.xy;
    float fDiffuseIntensity = max(0.0, dot(normalize(vnormal), aux));
    
	outputColor = color*vec4(vec3(1.0,1.0,1.0)*(0.25+fDiffuseIntensity), 1.0);
    
}


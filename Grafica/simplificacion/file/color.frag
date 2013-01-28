#version 330

in vec3 vnormal;
in vec4 color;
out vec4 outputColor;

struct LuzSimple
{
	vec3 ambiental;
	vec3 direccion;
	float intensidad;
};

uniform LuzSimple sol;

void main()
{
		vec3 vDirection = vec3(1.0,1.0,1.0);
		float fDiffuseIntensity = max(0.0, dot(normalize(vnormal), sol.direccion));
		outputColor = color * (sol.intensidad + fDiffuseIntensity); 
		//vec4(vec3(1.0,1.0,1.0)*(sol.intensidad + fDiffuseIntensity), 1.0);
}

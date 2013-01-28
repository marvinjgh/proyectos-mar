#include "..\include\mod\Modelo.h"


Modelo::Modelo(void)
{
}


Modelo::~Modelo(void)
{
}

void Modelo::cargarModelo(char* file)
{
}

void Modelo::centrar(void)
{
}

void Modelo::updateBuffer(Sombreado x)
{
}

void Modelo::min(glm::vec3& p,float* aux){
	if (p[0]>aux[0])
		p[0]=aux[0];
	if (p[1]>aux[1])
		p[1]=aux[1];
	if (p[2]>aux[2])
		p[2]=aux[2];
}

void Modelo::max(glm::vec3& p,float* aux){
	if (p[0]<aux[0])
		p[0]=aux[0];
	if (p[1]<aux[1])
		p[1]=aux[1];
	if (p[2]<aux[2])
		p[2]=aux[2];
}
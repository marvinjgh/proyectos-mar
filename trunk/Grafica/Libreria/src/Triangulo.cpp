#include "..\include\mod\Triangulo.h"


Triangulo::Triangulo(void)
{
}


Triangulo::~Triangulo(void)
{
}

Triangulo::Triangulo(size_t v0, size_t v1, size_t v2){
	vert[0]=v0;
	vert[1]=v1;
	vert[2]=v2;
	activo=true;
}

void Triangulo::setArista(size_t a, size_t v){
	aris[a]=v;
}

void Triangulo::setNormal(glm::vec3 & n){
	normal = n;
}

glm::vec3 & Triangulo::getNormal(){
	return normal;
}

size_t Triangulo::getVertice(size_t v){
	return vert[v];
}

void Triangulo::setVertice(size_t v,size_t value){
	vert[v]=value;
}

bool Triangulo::estaActivo(){
	return activo;
}

void Triangulo::cambiarEstado(){
	activo=!activo;
}

size_t Triangulo::getArista(size_t a){
	return aris[a];
}

ostream& operator<< (ostream& os, const Triangulo& p){
	return os << "Triangulo " <<p.activo << " v0 " << p.vert[0]<<" v1 "<<p.vert[1]<<" v2 "<<p.vert[2]<<" a0 "<<p.aris[0]<<" a1 "<<p.aris[1]<<" a2 "<<p.aris[2]<<"\n";
}
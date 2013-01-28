#include "..\include\mod\Arista.h"


Arista::Arista(void)
{
}


Arista::~Arista(void)
{
}

Arista::Arista(size_t v0, size_t v1)
{
	vert_a = v0;
	vert_b = v1;
}

bool Arista::operator==(Arista a){
	if ((vert_a == a.getVert_a() && vert_b == a.getVert_b()) || (vert_a == a.getVert_b() && vert_b == a.getVert_a()))
		return true;
	else
		return false;
}

void Arista::setVert_a(size_t v){
	vert_a = v;
}

void Arista::setVert_b(size_t v){
	vert_b = v;
}

void Arista::setTrian_1(size_t v){
	trian_1=v;
}

void Arista::setTrian_2(size_t v){
	trian_2=v;
}

size_t Arista::getVert_a(){
	return vert_a;
}

size_t Arista::getVert_b(){
	return vert_b;
}

size_t Arista::getTrian_1(){
	return trian_1;
}

size_t Arista::getTrian_2(){
	return trian_2;
}

bool Arista::estaActivo(){
	return activo;
}

void Arista::cambiarEstado(){
	activo=!activo;
}
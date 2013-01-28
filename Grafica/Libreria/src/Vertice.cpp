#include "..\include\mod\Vertice.h"


Vertice::Vertice(void)
{
	posicion = glm::vec4(0.0);
	normal = glm::vec3(0.0);
}

Vertice::Vertice(float* posicion){
	this->posicion = glm::vec4(posicion[0],posicion[1],posicion[2],posicion[3]);
}

Vertice::Vertice(float x, float y, float z){
	posicion = glm::vec4(x,y,z,1.0f);
}

Vertice::~Vertice(void)
{
}

glm::vec4& Vertice::getPosicion(){
	return posicion;
}

glm::vec3& Vertice::getNormal(){
	return normal;
}

size_t Vertice::getTriangulo(size_t t){
	return trian.at(t);
}

void Vertice::setPosicion(float x, float y, float z){
	posicion.x = x;
	posicion.y = y;
	posicion.z = z;
}

void Vertice::setPosicion(glm::vec4 & p){
	posicion = p;
}

void Vertice::setNormal(glm::vec3 & n){
	normal = n;
}

size_t Vertice::getSizetrian(){
	return trian.size();
}

void Vertice::agregarTriangulo(size_t t){
	trian.push_back(t);
}

void Vertice::setTrian(vector<size_t> v){
	trian.clear();
	trian=v;
}
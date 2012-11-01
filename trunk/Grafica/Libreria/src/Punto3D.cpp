#include "..\include\Geom\Punto3D.h"


Punto3D::Punto3D(void)
{
	coord[0]=0;
	coord[1]=0;
	coord[2]=0;
}

Punto3D::Punto3D(float x, float y, float z)
{
	coord[0]=x;
	coord[1]=y;
	coord[2]=z;
}

Punto3D::Punto3D(const Punto3D& p)
{
	coord[0]=p.coord[0];
	coord[1]=p.coord[1];
	coord[2]=p.coord[2];
	
}

Punto3D::~Punto3D(void)
{
}

float Punto3D::distancia(Punto3D p){
	float a = p.coord[0] - coord[0], b = p.coord[1] - coord[1], c = p.coord[2] - coord[2];
	a=a*a;b=b*b,c=c*c;
	return sqrt(a+b+c);
}

Punto3D Punto3D::operator+ (const Punto3D& p)
{
	return Punto3D(coord[0]+p.coord[0],coord[1]+p.coord[1],coord[2]+p.coord[2]);
}

Punto3D Punto3D::operator- (const Punto3D& p)
{
	return Punto3D(coord[0]-p.coord[0],coord[1]-p.coord[1],coord[2]-p.coord[2]);
}

Punto3D Punto3D::operator* (const Punto3D& p)
{
	return Punto3D(coord[0]*p.coord[0],coord[1]*p.coord[1],coord[2]*p.coord[2]);
}

Punto3D Punto3D::operator/ (float valor)
{
	Punto3D a;
	a.coord[0]=coord[0]/valor;
	a.coord[1]=coord[1]/valor;
	a.coord[2]=coord[2]/valor;
	return a;
}

Punto3D Punto3D::operator= (const Punto3D& p)
{
	this->coord[0]=p.coord[0];
	this->coord[1]=p.coord[1];
	this->coord[2]=p.coord[2];
	return *this;
}

Punto3D Punto3D::operator+= (const Punto3D& p)
{
	this->coord[0]+=p.coord[0];
	this->coord[1]+=p.coord[1];
	this->coord[2]+=p.coord[2];
	return *this;
}

Punto3D Punto3D::operator-= (const Punto3D& p)
{
	this->coord[0]-=p.coord[0];
	this->coord[1]-=p.coord[1];
	this->coord[2]-=p.coord[2];
	return *this;
}

Punto3D Punto3D::operator*= (float f)
{
	this->coord[0]*=f;
	this->coord[1]*=f;
	this->coord[2]*=f;
	return *this;
}

Punto3D Punto3D::operator/= (float f)
{
	this->coord[0]/=f;
	this->coord[1]/=f;
	this->coord[2]/=f;
	return *this;
}

bool Punto3D::operator== (const Punto3D& p){
	return (coord[0]==p.coord[0])&&(coord[1]==p.coord[1])&&(coord[2]==p.coord[2]);
}

Punto3D Punto3D::minp(const float* p){
	return Punto3D(min(coord[0],p[0]),min(coord[1],p[1]),min(coord[2],p[2]));
}

Punto3D Punto3D::maxp(const float* p){
	return Punto3D(max(coord[0],p[0]),max(coord[1],p[1]),max(coord[2],p[2]));
}


Punto3D operator* (float valor, const Punto3D& p)
{
  return Punto3D(p.coord[0] * valor, p.coord[1] * valor, p.coord[2] * valor);
}

Punto3D operator* (const Punto3D& p, float valor)
{
  return Punto3D(p.coord[0] * valor, p.coord[1] * valor, p.coord[2] * valor);
}

ostream& operator <<(ostream& os, const Punto3D& p)
{
  return os << "Punto·D(" << p.coord[0] << ", " << p.coord[1] <<", " << p.coord[2] <<  ")\n" ; 
}
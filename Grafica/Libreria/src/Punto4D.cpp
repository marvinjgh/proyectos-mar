#include "..\include\Geom\Punto4D.h"


Punto4D::Punto4D(void)
{
	coord[0]=0;
	coord[1]=0;
	coord[2]=0;
	coord[3]=0;
}

Punto4D::Punto4D(float x, float y, float z, float w)
{
	coord[0]=x;
	coord[1]=y;
	coord[2]=z;
	coord[3]=w;
}

Punto4D::Punto4D(const Punto4D& p)
{
	coord[0]=p.coord[0];
	coord[1]=p.coord[1];
	coord[2]=p.coord[2];
	coord[3]=p.coord[3];
	
}

Punto4D::~Punto4D(void)
{
}

float Punto4D::distancia(Punto4D p){
	float a = p.coord[0] - coord[0], b = p.coord[1] - coord[1], c = p.coord[2] - coord[2], d = p.coord[3] - coord[3];
	a=a*a;b=b*b,c=c*c;d=d*d;
	return sqrt(a+b+c+d);
}

Punto4D Punto4D::operator+ (const Punto4D& p)
{
	return Punto4D(coord[0]+p.coord[0],coord[1]+p.coord[1],coord[2]+p.coord[2],coord[3]+p.coord[3]);
}

Punto4D Punto4D::operator- (const Punto4D& p)
{
	return Punto4D(coord[0]-p.coord[0],coord[1]-p.coord[1],coord[2]-p.coord[2],coord[3]-p.coord[3]);
}

Punto4D Punto4D::operator* (const Punto4D& p)
{
	return Punto4D(coord[0]*p.coord[0],coord[1]*p.coord[1],coord[2]*p.coord[2],coord[3]*p.coord[3]);
}

Punto4D Punto4D::operator/ (float valor)
{
	Punto4D a;
	a.coord[0]=coord[0]/valor;
	a.coord[1]=coord[1]/valor;
	a.coord[2]=coord[2]/valor;
	a.coord[3]=coord[3]/valor;
	return a;
}

Punto4D Punto4D::operator= (const Punto4D& p)
{
	this->coord[0]=p.coord[0];
	this->coord[1]=p.coord[1];
	this->coord[2]=p.coord[2];
	this->coord[3]=p.coord[3];
	return *this;
}

void Punto4D::operator+= (const Punto4D& p)
{
	this->coord[0]+=p.coord[0];
	this->coord[1]+=p.coord[1];
	this->coord[2]+=p.coord[2];
	this->coord[3]+=p.coord[3];
}

void Punto4D::operator-= (const Punto4D& p)
{
	this->coord[0]-=p.coord[0];
	this->coord[1]-=p.coord[1];
	this->coord[2]-=p.coord[2];
	this->coord[3]-=p.coord[3];
}

void Punto4D::operator*= (float f)
{
	this->coord[0]*=f;
	this->coord[1]*=f;
	this->coord[2]*=f;
	this->coord[3]*=f;
}

void Punto4D::operator/= (float f)
{
	this->coord[0]/=f;
	this->coord[1]/=f;
	this->coord[2]/=f;
	this->coord[3]/=f;
}

bool Punto4D::operator== (const Punto4D& p){
	return (coord[0]==p.coord[0])&&(coord[1]==p.coord[1])&&(coord[2]==p.coord[2])&&(coord[3]==p.coord[3]);
}

Punto4D operator* (float valor, const Punto4D& p)
{
  return Punto4D(p.coord[0] * valor, p.coord[1] * valor, p.coord[2] * valor, p.coord[3] * valor);
}

Punto4D operator* (const Punto4D& p, float valor)
{
  return Punto4D(p.coord[0] * valor, p.coord[1] * valor, p.coord[2] * valor, p.coord[3] * valor);
}

ostream& operator <<(ostream& os, const Punto4D& p)
{
  return os << "Punto·D(" << p.coord[0] << ", " << p.coord[1] << ", " << p.coord[2] << ", " << p.coord[3]<<  ")\n" ; 
}
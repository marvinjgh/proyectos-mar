#include "Punto2D.h"

Punto2D::Punto2D(void)
{
	coord[0]=0;
	coord[1]=0;
}

Punto2D::Punto2D(double x, double y)
{
	coord[0]=x;
	coord[1]=y;
}

Punto2D::Punto2D(const Punto2D& p)
{
	coord[0]=p.coord[0];
	coord[1]=p.coord[1];
	
}

Punto2D::~Punto2D(void)
{
}

double Punto2D::distancia(Punto2D p){
	double a = p.coord[0] - coord[0], b = p.coord[1] - coord[1];
	a=a*a;b=b*b;
	return sqrt(a+b);
}

Punto2D Punto2D::operator+ (const Punto2D& p)
{
	return Punto2D(coord[0]+p.coord[0],coord[1]+p.coord[1]);
}

Punto2D Punto2D::operator- (const Punto2D& p)
{
	return Punto2D(coord[0]-p.coord[0],coord[1]-p.coord[1]);
}

Punto2D Punto2D::operator* (const Punto2D& p)
{
	return Punto2D(coord[0]*p.coord[0],coord[1]*p.coord[1]);
}

Punto2D Punto2D::operator/ (double valor)
{
	Punto2D a;
	a.coord[0]=coord[0]/valor;
	a.coord[1]=coord[1]/valor;
	return a;
}

Punto2D Punto2D::operator= (const Punto2D& p)
{
	this->coord[0]=p.coord[0];
	this->coord[1]=p.coord[1];
	return *this;
}

Punto2D Punto2D::operator+= (const Punto2D& p)
{
	this->coord[0]+=p.coord[0];
	this->coord[1]+=p.coord[1];
	return *this;
}

Punto2D Punto2D::operator-= (const Punto2D& p)
{
	this->coord[0]-=p.coord[0];
	this->coord[1]-=p.coord[1];
	return *this;
}

Punto2D Punto2D::operator*= (double f)
{
	this->coord[0]*=f;
	this->coord[1]*=f;
	return *this;
}

Punto2D Punto2D::operator/= (double f)
{
	this->coord[0]/=f;
	this->coord[1]/=f;
	return *this;
}

bool Punto2D::operator== (const Punto2D& p){
	return (coord[0]==p.coord[0])&&(coord[1]==p.coord[1]);
}

Punto2D operator* (double valor, const Punto2D& p)
{
  return Punto2D(p.coord[0] * valor, p.coord[1] * valor);
}

Punto2D operator* (const Punto2D& p, double valor)
{
  return Punto2D(p.coord[0] * valor, p.coord[1] * valor);
}

ostream& operator <<(ostream& os, const Punto2D& p)
{
  return os << "Punto2D(" << p.coord[0] << ", " << p.coord[1] <<  ")\n" ; 
}
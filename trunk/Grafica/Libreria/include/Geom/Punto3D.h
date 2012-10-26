#pragma once
#include "Punto2D.h"
class Punto3D: public Punto
{
public:
	/*coordenada */
	float coord[3];
	/*Contructor base*/
	Punto3D(void);
	/*Contructor por valor*/
	Punto3D(float x, float y, float z);
	/*Contructor por referencia*/
	Punto3D(const Punto3D& p);
	/*Destructor base*/
	~Punto3D(void);

	float distancia(Punto3D p);

	Punto3D operator+ (const Punto3D& p);
	Punto3D operator- (const Punto3D& p);
	Punto3D operator* (const Punto3D& p);

	Punto3D operator/ (float valor);
	Punto3D operator- (){
		return Punto3D(-coord[0],-coord[1],-coord[2]);
	}
	Punto3D operator=  (const Punto3D& p);
	
	Punto3D operator+= (const Punto3D& p);
	Punto3D operator-= (const Punto3D& p);
	Punto3D operator*= (float f);
	Punto3D operator/= (float f);
	bool operator== (const Punto3D& p);
	
	Punto3D minp(const float* p);
	Punto3D maxp(const float* p);

	friend Punto3D operator* (float valor, const Punto3D& p);
	friend Punto3D operator* (const Punto3D& p, float valor);
};


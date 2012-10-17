#pragma once
#include "Punto.h"
class Punto3D: public Punto
{
public:
	/*coordenada */
	double coord[3];
	/*Contructor base*/
	Punto3D(void);
	/*Contructor por valor*/
	Punto3D(double x, double y, double z);
	/*Contructor por referencia*/
	Punto3D(const Punto3D& p);
	/*Destructor base*/
	~Punto3D(void);

	double distancia(Punto3D p);

	Punto3D operator+ (const Punto3D& p);
	Punto3D operator- (const Punto3D& p);
	Punto3D operator* (const Punto3D& p);

	Punto3D operator/ (double valor);

	Punto3D operator=  (const Punto3D& p);
	
	Punto3D operator+= (const Punto3D& p);
	Punto3D operator-= (const Punto3D& p);
	Punto3D operator*= (double f);
	Punto3D operator/= (double f);
	bool operator== (const Punto3D& p);
	
	friend Punto3D operator* (double valor, const Punto3D& p);
	friend Punto3D operator* (const Punto3D& p, double valor);
};


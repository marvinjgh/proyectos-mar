#pragma once
#include "Punto3D.h"
class Punto4D: public Punto
{
public:
	/*coordenada */
	double coord[4];
	/*Contructor base*/
	Punto4D(void);
	/*Contructor por valor*/
	Punto4D(double x, double y, double z, double w);
	/*Contructor por referencia*/
	Punto4D(const Punto4D& p);
	/*Destructor base*/
	~Punto4D(void);

	double distancia(Punto4D p);

	Punto4D operator+ (const Punto4D& p);
	Punto4D operator- (const Punto4D& p);
	Punto4D operator* (const Punto4D& p);
	Punto4D operator/ (double valor);
	Punto4D operator=  (const Punto4D& p);
	void operator+= (const Punto4D& p);
	void operator-= (const Punto4D& p);
	void operator*= (double f);
	void operator/= (double f);
	bool operator== (const Punto4D& p);
	
	friend Punto4D operator* (double valor, const Punto4D& p);
	friend Punto4D operator* (const Punto4D& p, double valor);
};
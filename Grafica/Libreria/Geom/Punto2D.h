#pragma once
#include "Punto.h"

class Punto2D: public Punto
{
private:
	
public:
	/*coordenada */
	double coord[2];
	/*Contructor base*/
	Punto2D(void);
	/*Contructor por valor*/
	Punto2D(double x, double y);
	/*Contructor por referencia*/
	Punto2D(const Punto2D& p);
	/*Destructor base*/
	~Punto2D(void);

	double distancia(Punto2D p);

	Punto2D operator+ (const Punto2D& p);
	Punto2D operator- (const Punto2D& p);
	Punto2D operator* (const Punto2D& p);

	Punto2D operator/ (double valor);

	Punto2D operator=  (const Punto2D& p);
	
	Punto2D operator+= (const Punto2D& p);
	Punto2D operator-= (const Punto2D& p);
	Punto2D operator*= (double f);
	Punto2D operator/= (double f);
	bool operator== (const Punto2D& p);
	
	friend Punto2D operator* (double valor, const Punto2D& p);
	friend Punto2D operator* (const Punto2D& p, double valor);

	friend ostream& operator<< (ostream& os, const Punto2D& p);
};


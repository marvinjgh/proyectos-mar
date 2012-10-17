#pragma once
#include "Punto4D.h"

class Vec4D
{

public:

	double coord[4];
	
	Vec4D(void);
	Vec4D(double x, double y, double z, double w);
	Vec4D(const double* v);
	Vec4D(const Vec4D &v);
	Vec4D(const Punto4D &v);

	double Magnitud(void) const;

	void Normalizar(void);

	Vec4D operator+ (const Vec4D& v);
	Vec4D operator- (const Vec4D& v);
	Vec4D operator/ (double f);
	Vec4D operator+= (const Vec4D& v);
	Vec4D operator-= (const Vec4D& v);
	Vec4D operator*= (double f);
	Vec4D operator/= (double f);
  
	double  operator*  (const Vec4D& v) const;   // Producto Escalar
	Vec4D operator^  (const Vec4D& v) const;   // Producto Cruz (el ultimo valor es 1)
	Vec4D operator-  (void) const;
	Vec4D operator=  (const Vec4D& v);

	friend Vec4D operator*(double f, const Vec4D& v);
	friend Vec4D operator*(const Vec4D& v, double f);

	friend ostream& operator <<(ostream& os, const Vec4D& v);
};


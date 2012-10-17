#pragma once
#include "Vec4D.h"

enum Type_MAT {MAT_T,MAT_E,MAT_R};

class Mat4x4{

public:
	double mat[16];

	// Constructores
	Mat4x4(void);
	Mat4x4(const Mat4x4& m);


	void Identidad(void);
	Mat4x4 MatTranslation(double x, double y, double z);	//crea una matriz de traslacion
	Mat4x4 MatTranslation(const Punto3D& p);	//crea una matriz de traslacion
	Mat4x4 MatScale(double x, double y, double z);	//crea una matriz de traslacion
	Mat4x4 MatScale(const Punto3D& p);	//crea una matriz de traslacion
 
	Mat4x4 operator* (const Mat4x4& M);
	Mat4x4 operator= (const Mat4x4& M);
	void operator*=(const Mat4x4& M);
  
	friend Punto3D  operator* (const Punto3D&  p, const Mat4x4& M);

	friend Vec4D operator*(const Mat4x4& M, const Vec4D& v);

	friend ostream& operator<<(ostream& os, const Mat4x4& m);

};
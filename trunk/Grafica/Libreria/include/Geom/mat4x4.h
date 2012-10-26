#pragma once
#include "Vec4D.h"

enum Type_MAT {MAT_T,MAT_E,MAT_R};

class Mat4x4{

public:
	float mat[16];

	// Constructores
	Mat4x4(void);
	Mat4x4(const Mat4x4& m);
	void Identidad(void);
	Mat4x4 operator* (const Mat4x4& M);
	Mat4x4 operator= (const Mat4x4& M);
	void operator*=(const Mat4x4& M);

	friend Punto3D  operator* (const Punto3D&  p, const Mat4x4& M);

	friend Vec4D operator*(const Mat4x4& M, const Vec4D& v);

	friend ostream& operator<<(ostream& os, const Mat4x4& m);

};

Mat4x4 MatTranslate(float x, float y, float z);	//crea una matriz de traslacion
Mat4x4 MatTranslate(const Punto3D& p);	//crea una matriz de traslacion
Mat4x4 MatScale(float x, float y, float z);	//crea una matriz de traslacion
Mat4x4 MatScale(const Punto3D& p);	//crea una matriz de traslacion
Mat4x4 MatRotar(const float* quat);
Mat4x4 buildPerpectiva(float angulo,float aspect, float near, float far);
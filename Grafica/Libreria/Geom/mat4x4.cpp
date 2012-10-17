#include "Mat4x4.h"

Mat4x4::Mat4x4()
{
  Identidad();
}

Mat4x4::Mat4x4(const Mat4x4 &m)
{
	for(int i=0; i<16; i++)
		mat[i] = m.mat[i];
}

void Mat4x4::Identidad()
{
  for(int i=0; i<16; i++)
      mat[i] = (i%5 == 0) ? 1.0 : 0.0;
}

Mat4x4 Mat4x4::MatTranslation(double x, double y, double z)
{
	
  Mat4x4 tem;
  tem.mat[12]=x;
  tem.mat[13]=y;
  tem.mat[14]=z;

  return tem;
}

Mat4x4 Mat4x4::MatTranslation(const Punto3D& p)
{
  Mat4x4 tem;
  tem.mat[12]=p.coord[0];
  tem.mat[13]=p.coord[1];
  tem.mat[14]=p.coord[2];

  return tem;
}

Mat4x4 Mat4x4::MatScale(double x, double y, double z)
{
  Mat4x4 tem;
  tem.mat[0]=x;
  tem.mat[5]=y;
  tem.mat[10]=z;

  return tem;
}

Mat4x4 Mat4x4::MatScale(const Punto3D& p)
{
  Mat4x4 tem;
  tem.mat[0]=p.coord[0];
  tem.mat[5]=p.coord[1];
  tem.mat[10]=p.coord[2];

  return tem;
}

Mat4x4 Mat4x4::operator*(const Mat4x4& M)
{
	int i,j,k;
	Mat4x4 resul;

	for(i=0;i<4; i++){
		for(j=0;j<4; j++) {
			k=j*4;
			resul.mat[i+k] = Vec4D(mat[i],mat[i+4],mat[i+8],mat[i+12])*Vec4D(M.mat+(k));
		}
	}
	return resul;
}

Mat4x4 Mat4x4::operator=(const Mat4x4& M)
{
	for(int i=0; i<16; i++)
		mat[i] = M.mat[i];

  return (*this);
}

void Mat4x4::operator*=(const Mat4x4& M)
{
	int i,j,k;
	Mat4x4 resul;

	for(i=0;i<4; i++){
		for(j=0;j<4; j++) {
			k=j*4;
			resul.mat[i+k] = Vec4D(mat[i],mat[i+4],mat[i+8],mat[i+12])*Vec4D(M.mat+(k));
		}
	}

	*this = resul;
}

Vec4D operator*(const Mat4x4& M, const Vec4D& v){

	Vec4D out;
	int i,j;

	for(i=0;i<4; i++){
		out.coord[i] = Vec4D(M.mat[i],M.mat[i+4],M.mat[i+8],M.mat[i+12])*v;
	}

	return out;
}

ostream& operator <<(ostream& os, const Mat4x4& M)
{
  return os << "Mat4x4: \n"
	    << M.mat[0] << ", " << M.mat[4] << ", " << M.mat[8] << ", " << M.mat[12] << "\n"
	    << M.mat[1] << ", " << M.mat[5] << ", " << M.mat[9] << ", " << M.mat[13] << "\n"
	    << M.mat[2] << ", " << M.mat[6] << ", " << M.mat[10] << ", " << M.mat[14] << "\n"
	    << M.mat[3] << ", " << M.mat[7] << ", " << M.mat[11] << ", " << M.mat[15] << "\n";
}
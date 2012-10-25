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
		mat[i] = (i%5 == 0) ? 1.f : 0.f;
}

Mat4x4 Mat4x4::MatTranslate(float x, float y, float z)
{

	Mat4x4 tem;
	tem.mat[12]=x;
	tem.mat[13]=y;
	tem.mat[14]=z;

	return tem;
}

Mat4x4 Mat4x4::MatTranslate(const Punto3D& p)
{
	Mat4x4 tem;
	tem.mat[12]=p.coord[0];
	tem.mat[13]=p.coord[1];
	tem.mat[14]=p.coord[2];

	return tem;
}

Mat4x4 Mat4x4::MatScale(float x, float y, float z)
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
	int i;

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

Mat4x4 Mat4x4::MatRotar(const float* quat){
	float yy2 = 2.0f * quat[1] * quat[1];
	float xy2 = 2.0f * quat[0] * quat[1];
	float xz2 = 2.0f * quat[0] * quat[2];
	float yz2 = 2.0f * quat[1] * quat[2];
	float zz2 = 2.0f * quat[2] * quat[2];
	float wz2 = 2.0f * quat[3] * quat[2];
	float wy2 = 2.0f * quat[3] * quat[1];
	float wx2 = 2.0f * quat[3] * quat[0];
	float xx2 = 2.0f * quat[0] * quat[0];

	Mat4x4 r; 


	r.mat[0] = - yy2 - zz2 + 1.0f;
	r.mat[4] = xy2 - wz2;
	r.mat[8] = xz2 + wy2;

	r.mat[1] = xy2 + wz2;
	r.mat[5] = - xx2 - zz2 + 1.0f;
	r.mat[9] = yz2 - wx2;

	r.mat[2] = xz2 - wy2;
	r.mat[6] = yz2 + wx2;
	r.mat[10] = - xx2 - yy2 + 1.0f;

	r.mat[3] = r.mat[7] = r.mat[11] = r.mat[12] = r.mat[13] = r.mat[14] = 0;

	r.mat[15] = 1;

	return r;
}

Mat4x4 Mat4x4::buildPerpectiva(float angulo, float aspect, float znear, float zfar)
{
	const float h = 1.0f/tan(angulo*PI_OVER_360);
	float neg_depth = znear-zfar;

	Mat4x4 a;

	a.mat[0]  = h/aspect;
	a.mat[1]  = 0;
	a.mat[2]  = 0;
	a.mat[3]  = 0;
	a.mat[4]  = 0;
	a.mat[5]  = h;
	a.mat[6]  = 0;
	a.mat[7]  = 0;
	a.mat[8]  = 0;
	a.mat[9]  = 0;
	a.mat[10] = (zfar + znear)/neg_depth;;
	a.mat[11] = -1;
	a.mat[12] = 0;
	a.mat[13] = 0;
	a.mat[14] = 2.0f*(znear*zfar)/neg_depth;
	a.mat[15] = 0;

	return a;
}
#include "Geom\mat4x4.h"

int main(int argc, char* argv){

	Mat4x4 t;
	t*=t.MatScale(2.f,1.f,1.f);
	cout<<(t*t.MatTranslation(10.f,0.f,0.f))*Vec4D(10.0,10.0,10.0,1.0);

	cin.get();
	return 0;
}
#include "..\include\mod\ModeloOff.h"


ModeloOff::ModeloOff(void)
{
	BufferObject = 0;
	minp = Punto3D(FLT_MAX,FLT_MAX,FLT_MAX);
	maxp = Punto3D(FLT_MIN,FLT_MIN,FLT_MIN);
}


ModeloOff::~ModeloOff(void)
{
	glDeleteBuffersARB(1,&BufferObject);
	delete vertices;
	delete triangulos;
}

void ModeloOff::cargarOff(const char* file){
	glGenBuffersARB(1,&BufferObject);
	string str;
	size_t aux,p,n;
	size_t p0,p1;
	fstream f (file,fstream::in);

	Vector_off *v;
	Triangulo_off *triangle;

	if (!f.is_open()){ puts("Error al abrir el archivo off , saliendo del programa..."); exit(0); }
	f>>str;
	if (str=="OFF"){
		f>>nVert>>nTrian>>aux;
		vertices  = new Vector_off[nVert];
		triangulos  = new Triangulo_off[nVert];
		for (aux=0;aux<nVert;aux++){
			f>>vertices[aux].posicion.coord[0]>>vertices[aux].posicion.coord[1]>>vertices[aux].posicion.coord[2];
			minp.minp(vertices[aux].posicion.coord);
			minp.maxp(vertices[aux].posicion.coord);
		}
		for (aux = 0; aux < nTrian; aux++)
		{


			f>>n;
			f>>triangulos[aux].vertices[0];
			f>>triangulos[aux].vertices[1];
			f>>triangulos[aux].vertices[2];

			calcularNormal(aux);
		}
	}
	f.close();
}

void ModeloOff::calcularNormal(GLuint t){
	Vec3D u(vertices[triangulos[t].vertices[0]].posicion,vertices[triangulos[t].vertices[1]].posicion);
	Vec3D v(vertices[triangulos[t].vertices[0]].posicion,vertices[triangulos[t].vertices[2]].posicion);


}
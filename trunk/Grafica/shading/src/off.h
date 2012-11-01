#include "main.h"



/* estructura que define un punto */
struct Punto {
	GLfloat p[3];
};

struct Vertice{
	Punto* point;
	GLfloat normal[3];
	GLuint n;
	void acum_normal(GLfloat n0,GLfloat n1,GLfloat n2);
	void cal_normal();
};

/* estructura que define un Triangulo */
struct Triangulo {
	Vertice *p[3];
	Triangulo *next;
	GLfloat c[3];
	GLfloat normal[3];
	void cal_normal();
	void normalizar(float deno,float x,float y,float z);
};

void pintar();

void read(char * Path);

void normalizar();

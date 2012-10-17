#ifndef __CLASES_H
#define __CLASES_H
#include "main.h"
#include <vector>
using namespace std;
#define PI 3.141592653589793238462643

class Color3f {
	public:
	GLfloat r, g, b; 
	Color3f(GLfloat, GLfloat, GLfloat);
	Color3f();
};

/* estructura que define un punto */
struct Punto {
	GLint x, y; 
	Punto(GLint, GLint);
	Punto();
};

///* Clase virtual Figura */
class Figura{
	public:
		GLushort patron;
		GLushort i;
		Color3f linea;
		Color3f fondo;
		Punto *puntos;
		bool cambio;
		float cose,seno,angulo;
		float e1,e2;
		GLint xt,yt;
		L llenado;
		TipoFig type;
		GLuint size;
		virtual void draw() = 0;
		virtual void setPointEnd(GLint, GLint, PT);
		void rotar(float );
		void escalar(float ,float );
		void traladar(GLint ,GLint );
		virtual bool haypuntofinal() = 0;
		//true si es min, true es x
		GLint min_max(bool ,bool );
		virtual void addpoint(){}
		virtual GLint buscar(GLint , GLint){return -1;}
		virtual void setPoint(GLint ,GLint ,GLint ){}
		virtual void eliminar(GLint ){}
};

extern Figura* fig, *fig_last;

/* Clase rectangulo con mi implementacion de linea*/
class Rectangulo: public Figura{
public:
	Rectangulo (GLint ,GLint ,GLushort ,Color3f ,Color3f ,L );
	void draw();
	bool haypuntofinal();
};

/* Clase Linea con mi implementacion de linea */
class Linea: public Figura{
public:
	Linea (GLint ,GLint ,GLushort ,Color3f );
	void draw();
	bool haypuntofinal();
};

/* Clase Circulo implementacion vista en clase */
class Circulo: public Figura{
private:
	unsigned int r;
	void draw8point(GLint , GLint ,GLushort*, GLint);
public:
	Circulo (GLint ,GLint ,GLushort ,Color3f ,Color3f ,L);
	void setPointEnd(GLint, GLint, PT);
	void draw();
	bool haypuntofinal();
};

/* Clase Elipse implementacion vista en clase */
class Elipse: public Figura{
private:
	unsigned int a, b;
	void draw4point(GLint , GLint ,GLushort*,GLint );
public:
	Elipse (GLint ,GLint ,GLushort ,Color3f ,Color3f ,L );
	void setPointEnd(GLint, GLint, PT);
	void draw();
	bool haypuntofinal();
};

/* Clase Triangulo con mi implementacion de linea */
class Triangulo: public Figura{
public:
	Triangulo (GLint ,GLint ,GLushort ,Color3f ,Color3f ,L );
	void setPointEnd(GLint, GLint, PT);
	void draw();
	bool haypuntofinal();
};

/* Clase Triangulo con primitivas de OpenGL */
class Curva_bezier: public Figura{
private:
	Punto *puntos_curva;
	unsigned int n,count;
	double t1;
	void punto_en_curva(Punto[] ,double );
public:
	Curva_bezier (GLint ,GLint ,GLushort ,Color3f ,Color3f ,L ,GLuint);
	void setPointEnd(GLint, GLint, PT);
	void draw();
	bool haypuntofinal();
	void fijarN();
	void addpoint();
	GLint buscar(GLint , GLint);
	void setPoint(GLint ,GLint ,GLint );
	void eliminar(GLint );
};
#endif

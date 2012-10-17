#pragma comment (lib, "lib/freeglut.lib")

#include "GL/freeglut.h"
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cmath>
using namespace std;

//Variables
int WIDTH,HEIGHT;
float r,g,b;
int xii,yii,xff,yff;
int submenu, submenu_gl, submenu_tipo;
GLushort pt;
/* Clase generica de una Figura */
class Figura{
	public: 
		GLushort patron;
		virtual void draw() = 0;
		virtual void setPointEnd(int xf,int yf) = 0;
};

/* Contendra todas las figuras creadas */
vector <Figura*> figuras;
/* Figura actual */
Figura* fig;

enum TipoFig { LINE, LINE_GL, REC, REC_GL, TRI, TRI_GL, CIRC ,ELIP } fig_sel;

enum Mystate { UP, DOWN} estado;

enum Punto {INICIAL, MEDIO, FINAL} actual;

bool isDrawpixel(GLushort patron, GLushort* actual){
	if (*actual==0)
		*actual=1;
	if (patron&(*actual)){
		*actual=(*actual)<<1;
		return true;
	}else{
		*actual=(*actual)<<1;
		return false;
	}
}

/* implementacion para dibujar una linea */
void line(int xi,int yi,int xf,int yf,GLshort patron){
	
	int dx=xf-xi, dy=(600-yf)-(600-yi);
	int m=((dx>=0 && dy>=0)||(dx<0 && dy<0))? 1:-1;
	int y=0, y2=0,d=0,d1,d2;
	int x=min(xi,xf), x2 = max(xf,xi);
	GLushort actual=1;
	if (m==1){ y=max(yi,yf); y2=min(yi,yf);	}
	else{ y=min(yi,yf); y2=max(yi,yf);	}
	
	dx=x2-x; dy=(600-y2)-(600-y);

	glBegin(GL_POINTS);
	if (isDrawpixel(patron,&actual))
		glVertex2i(x,y);

	if (abs(dx)>=abs(dy)) 
	{/* linea en 315°-45° & 135°-225° */
		//d1 E - SE
		d1=(m==1)?-(dy<<1):-((dx<<1)+(dy<<1));
		//d2 NE - E
		d2=(m==1)?((dx<<1)-(dy<<1)):-(dy<<1);
		d=(dx*m)-(dy<<1);
		while (x<x2)
		{
			if (d>=0){
				d+= d1;
				y-= (m==1)? 0 : m;
			}
			else
			{
				d+= d2;
				y-= (m==1)? m : 0;
			}
			x+=1;
			if (isDrawpixel(patron,&actual))
				glVertex2i(x,y);
		}
	}
	else
	{/* linea en 45°-135° & 225°-315 */
		//d1 NE - S
		d1=(m==1)?((dx<<1)-(dy<<1)):-(dx<<1);
		//d2 N - SE
		d2=(m==1)?(dx<<1):-((dx<<1)+(dy<<1));
		d=m*(dx<<1)-(dy);
		while (y>y2 || y<y2)
		{
			if (d>=0){
				d+= d1;
				x+= (m==1)? m : 0;
			}
			else
			{
				d+= d2;
				x-= (m==1)? 0 : m;
			}
			y-=m;
			if (isDrawpixel(patron,&actual))
				glVertex2i(x,y);
		}
	
	}
	glEnd();
	//glFlush();
}

/* Clase Linea con mi implementacion de linea */
class Linea: public Figura{
	private: 
		int xi, yi, xf, yf;
	public:
		Linea (int x1,int y1,int x2,int y2)
		{
			xi=x1;
			yi=y1;
			xf=x2;
			yf=y2;
			patron=pt;
		}
		void setPointEnd(int x1,int y1)
		{
			xf=x1;
			yf=y1;
		}
		void draw(){
			line(xi,yi,xf,yf,patron);
		}
};

/* Clase rectangulo con mi implementacion de linea */
class Rectangulo: public Figura{
	private: 
		int xini, yini, xfin, yfin;
	public:
		Rectangulo (int x1,int y1,int x2,int y2)
		{
			xini=x1;
			yini=y1;
			xfin=x2;
			yfin=y2;
			patron=pt;
		}
		void setPointEnd(int xf,int yf)
		{
			xfin=xf;
			yfin=yf;
		}
		void draw(){
			line(xini,yini,xini,yfin,patron);
			line(xini,yini,xfin,yini,patron);
			line(xfin,yfin,xini,yfin,patron);
			line(xfin,yfin,xfin,yini,patron);
		}
};

/* Clase triangulo con mi implementacion de linea */
class Triangulo : public Figura{
	private: 
		int xi, yi, xm, ym, xf, yf;
		Punto interno;
	public:
		public:
		Triangulo (int x1,int y1,int x2,int y2,int x3, int y3)
		{
			xi=x1;
			yi=y1;
			xm=x2;
			ym=y2;
			xf=x3;
			yf=x3;
			interno=INICIAL;
			patron=pt;
		}
		void setPointEnd(int x1,int y1)
		{
			if (actual==MEDIO)
			{
				xm=x1;
				ym=y1;
				interno=MEDIO;
			}
			if (actual==FINAL)
			{
				xf=x1;
				yf=y1;
				interno=FINAL;
			}
		}
		void draw()
		{
			
			if (interno==MEDIO)
			{
				line(xi,yi,xm,ym,patron);
			}
			if (interno==FINAL)
			{
				line(xi,yi,xm,ym,patron);
				line(xm,ym,xf,yf,patron);
				line(xf,yf,xi,yi,patron);
			}
		}
};

/* Clase Circulo implementacion vista en clase*/
class Circulo: public Figura{
	private: 
		int xc, yc, xf, yf, r;
		void draw8point(int x, int y,GLushort* actual){
			//xc,yc son el centro
			//x,y seran las coordenadas de la espera unitaria
			if (isDrawpixel(patron,actual)){
				glVertex2i(xc+y,yc-x);//O1
				glVertex2i(xc+x,yc-y);//O2
				glVertex2i(xc-x,yc-y);//O3
				glVertex2i(xc-y,yc-x);//O4
				glVertex2i(xc-y,yc+x);//O5
				glVertex2i(xc+x,yc+y);//O6
				glVertex2i(xc-x,yc+y);//O7
				glVertex2i(xc+y,yc+x);//O8
			}
		}
	public:
		Circulo (int x1,int y1,int x2,int y2)
		{
			xc=x1;
			yc=y1;
			xf=x2;
			yf=y2;
			patron=pt;
		}
		void setPointEnd(int x1, int y1)
		{
			xf=x1;
			yf=y1;
			r=(int)sqrt(pow((double)(xf-xc),2.0)+pow((double)(yf-yc),2.0));
		}	
		void draw()
		{
			int x=0, d=(1-r)<<1, y=r;
			GLushort actual=1;
			glBegin(GL_POINTS);
			
			draw8point(x,y,&actual);
			while(y>x){
				if (d>=0) 
				{
					d+=((x-y)<<1)+5;
					--y;
				}
				else 
				{
					d+=((x<<1)+3); 
				}
				x++;
				draw8point(x,y,&actual);
			}
			glEnd();
		}
};

/* Clase Elipse implementacion vista en clase*/
class Elipse: public Figura{
	private: 
		int xi, yi, xf, yf, a, b;
		
		void draw4point(int x, int y, GLushort* actual){
			//xc,yc son el centro
			//x,y seran las coordenadas de la espera unitaria
			int xc=min(xi,xf)+a,yc=min(yi,yf)+b;
			if (isDrawpixel(patron,actual)){
				glVertex2i(xc+x,yc-y);//C1
				glVertex2i(xc-x,yc-y);//C2
				glVertex2i(xc-x,yc+y);//C3
				glVertex2i(xc+x,yc+y);//C4
			}
		}
	public:
		Elipse (int x1,int y1,int x2,int y2)
		{
			xi=x1;
			yi=y1;
			xf=x2;
			yf=y2;
			patron=pt;
		}
		void setPointEnd(int x1, int y1)
		{
			xf=x1;
			yf=y1;
			a=abs(xi-xf)>>1;
			b=abs(yi-yf)>>1;
		}
		
		void draw()
		{
			int a2=a*a,b2=b*b;
			int d=((b2-a2*b)<<2)+a2;
			int x=0,y=b;
			GLushort actual=1;

			glBegin(GL_POINTS);
			draw4point(x,y,&actual);
			while(((b2<<1)*(x+1))<(a2*((y+1)<<1))){
				if (d>=0)
				{
					d+=((b2*((x<<1)+3))<<2)-((a2*((y-1)<<1))<<2);
					y--;
				}
				else
				{
					d+=(b2*((x<<1)+3))<<2;
				}
				x++;
				draw4point(x,y,&actual);
			}

			d=b2*((x<<1)+1)*((x<<1)+1)+(a2<<2)*(((y-1)*(y-1))-b2);
			while(y>0){
				if (d<0){
					d+=(b2*((x+1)<<1)+a2*(-(y<<1)+3))<<2;
					x++;
				}
				else
				{
					d+=(a2*(-(y<<1)+3))<<2;
				}
				y--;
				draw4point(x,y,&actual);
			}
			glEnd();
		}
};

/* Clase Linea con mi primitiva de OpenGL */
class Linea_gl: public Figura{
	private: 
		int xi, yi, xf, yf;
	public:
		Linea_gl (int x1,int y1,int x2,int y2)
		{
			xi=x1;
			yi=y1;
			xf=x2;
			yf=y2;
			patron=pt;
		}
		void setPointEnd(int x1,int y1)
		{
			xf=x1;
			yf=y1;
		}
		void draw(){
			glLineStipple(1,patron);
			glBegin(GL_LINES);
				glVertex2i(xi,yi);
				glVertex2i(xf,yf);
			glEnd();
			//glDisable(GL_LINE_STIPPLE);
		}
};

/* Clase rectangulo con primitivas de OpenGL */
class Rectangulo_gl: public Figura{
	private: 
		int xini, yini, xfin, yfin;
	public:
		Rectangulo_gl (int x1,int y1,int x2,int y2)
		{
			xini=x1;
			yini=y1;
			xfin=x2;
			yfin=y2;
			patron=pt;
		}
		void setPointEnd(int xf,int yf)
		{
			xfin=xf;
			yfin=yf;
		}
		void draw()
		{
			glLineStipple(1,patron);
			glBegin(GL_LINES);
				glVertex2i(xini,yini);
				glVertex2i(xini,yfin);
				glVertex2i(xini,yini);
				glVertex2i(xfin,yini);
				glVertex2i(xfin,yfin);
				glVertex2i(xfin,yini);
				glVertex2i(xfin,yfin);
				glVertex2i(xini,yfin);
			glEnd();
		}
};

/* Clase triangulo con primitivas de OpenGL */
class Triangulo_gl : public Figura{
	private: 
		int xi, yi, xm, ym, xf, yf;
		Punto interno;
	public:
		public:
		Triangulo_gl (int x1,int y1,int x2,int y2,int x3, int y3)
		{
			xi=x1;
			yi=y1;
			xm=x2;
			ym=y2;
			xf=x3;
			yf=x3;
			interno=INICIAL;
			patron=pt;
		}
		void setPointEnd(int x1,int y1)
		{
			if (actual==MEDIO)
			{
				xm=x1;
				ym=y1;
				interno=MEDIO;
				//printf("mofica medio\n");
			}
			if (actual==FINAL)
			{
				xf=x1;
				yf=y1;
				interno=FINAL;
				//printf("mofica final\n");
			}
		}
		void draw()
		{
			glLineStipple(1,patron);
			glBegin(GL_LINES);
			if (interno==MEDIO)
			{
				glVertex2i(xi,yi);
				glVertex2i(xm,ym);
			}
			if (interno==FINAL)
			{
				glVertex2i(xi,yi);
				glVertex2i(xm,ym);
				glVertex2i(xm,ym);
				glVertex2i(xf,yf);
				glVertex2i(xf,yf);
				glVertex2i(xi,yi);
			}
			glEnd();
		}
};


void prueba(){	
	//patron++;
	//glFlush();
}

void create_figura()
{
	switch(fig_sel)
	{
		case LINE: fig = new Linea(xii,yii,xii,yii); figuras.push_back(fig); actual=FINAL; break;
		case LINE_GL: fig = new Linea_gl(xii,yii,xii,yii); figuras.push_back(fig); actual=FINAL; break;
		case REC: fig = new Rectangulo(xii,yii,xii,yii); figuras.push_back(fig); actual=FINAL; break;
		case REC_GL: fig = new Rectangulo_gl(xii,yii,xii,yii); figuras.push_back(fig); actual=FINAL; break;
		case TRI: fig = new Triangulo(xii,yii,xii,yii,xii,yii); figuras.push_back(fig); actual=MEDIO; break;
		case TRI_GL: fig = new Triangulo_gl(xii,yii,xii,yii,xii,yii); figuras.push_back(fig); actual=MEDIO; break;
		case CIRC: fig = new Circulo(xii,yii,xii,yii); figuras.push_back(fig); actual=FINAL; break;
		case ELIP: fig = new Elipse(xii,yii,xii,yii); figuras.push_back(fig); actual=FINAL; break;
	}
}

void display(void)
{
	glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
	glClear (GL_COLOR_BUFFER_BIT);
	WIDTH=glutGet(GLUT_WINDOW_WIDTH);
	HEIGHT=glutGet(GLUT_WINDOW_HEIGHT);
	glOrtho (0, WIDTH, HEIGHT, 0,-1,0);
	for(size_t i = 0;i<figuras.size(); i++)
	{
		figuras.at(i) -> draw();
	}
	glFlush();
	//prueba();
}
void limpiar(){
	while (!figuras.empty())
		figuras.pop_back();
	display();
}
void salir(){
	limpiar();
	exit(0);
}
void menu(int value){
	switch(value){
		case 1: fig_sel = LINE;	break;
		case 2:	fig_sel = TRI;	break;
		case 3:	fig_sel = REC;	break;
		case 4: fig_sel = ELIP;	break;
		case 5: fig_sel = LINE_GL;	break;
		case 6:	fig_sel = TRI_GL;	break;
		case 7:	fig_sel = REC_GL;	break;
		case 8: fig_sel = CIRC;	break;
		case 9: pt=0xFFFF;	break;
		case 10: pt=0x0F0F;	break;
		case 11: pt=0x7D7D;	break;
		case 12: limpiar();	break;
		default : salir(); break;
	}
}
void init (void) 
{
	r=0.0;g=0.0;b=0.0;
	glClearColor (1.0, 1.0, 1.0, 0.0);
    glColor3f (r,g,b);
	xii=yii=xff=yff=0;
	fig=NULL;
	fig_sel=LINE;
	actual=INICIAL;
	estado=UP;
	pt=0xFFFF;

	submenu = glutCreateMenu(menu);
	glutAddMenuEntry("Linea", 1);
	glutAddMenuEntry("Triangulo", 2);
	glutAddMenuEntry("Rectangulo", 3);
	glutAddMenuEntry("Circulo", 8);
	glutAddMenuEntry("Elipse", 4);

	submenu_gl = glutCreateMenu(menu);
	glutAddMenuEntry("Linea", 5);
	glutAddMenuEntry("Triangulo", 6);
	glutAddMenuEntry("Rectangulo", 7);

	submenu_tipo = glutCreateMenu(menu);
	glutAddMenuEntry("Continua", 9);
	glutAddMenuEntry("Discontinua", 10);
	glutAddMenuEntry("Punto-raya", 11);

	glutCreateMenu(menu);
	glutAddSubMenu("Mi implementacion", submenu);
	glutAddSubMenu("Primitivas open_gl", submenu_gl);
	glutAddSubMenu("Tipo de linea", submenu_tipo);
	glutAddMenuEntry("Limpiar         (C)", 12);
	glutAddMenuEntry("salir           (Q)", 0);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glEnable(GL_LINE_STIPPLE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
void reshape(int width, int height)
{
	display();
}
void pasivo(int x, int y)
{
	if (estado)
	 {
		xff=x;
		yff=y;
		fig -> setPointEnd(xff,yff);
		display();
	 }
}
void motion_mouse(int x, int y)
 {
	 if (estado)
	 {
		xff=x;
		yff=y;
		fig -> setPointEnd(xff,yff);
		display();
	 }
 }
void mouse(int button, int state, int x, int y) 
{	
	if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
		//printf("%d %d 1\n",x,y);
		xii=x;
		yii=y;
		glPointSize(1);
		glLineWidth(1);
		if (actual==INICIAL)
			create_figura();
		estado=DOWN;
		//prueba();
	}
	if (button==GLUT_LEFT_BUTTON && state==GLUT_UP){
		//printf("%d %d 2\n",x,y);
		xff=x;
		yff=y;
		
		if (actual==FINAL){
			estado=UP;
			actual=INICIAL;
			fig=NULL;
		}
		if (actual==MEDIO){
			actual=FINAL;
			motion_mouse(x,y);
		}
	}
}
void teclado(unsigned char key, int, int y){

	if (key == 'c' || key == 'C')
		limpiar();
	if (key == 'q' || key == 'Q')
		salir();

}

/* 
 */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (800, 600);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-800)/2, (glutGet(GLUT_SCREEN_HEIGHT)-600)/2); 
    glutCreateWindow ("Tarea 1");
    init ();
    glutDisplayFunc(display); 
	glutMouseFunc(mouse);
	glutMotionFunc(motion_mouse);
	glutKeyboardFunc(teclado);
	glutPassiveMotionFunc(pasivo);
    glutMainLoop();
    return 0;
}//27 ESC , c es la letra como tal
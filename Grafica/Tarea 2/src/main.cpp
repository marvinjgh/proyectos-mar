#pragma comment (lib, "lib/freeglut.lib")
//#pragma comment (lib, "lib/AntTweakBar.lib")
//#include "GL/freeglut.h"
//#include <vector>
//#include "GL\AntTweakBar.h"
#include "main.h"
#include "Clases.h"
#include <cstdio>
#include <cstdlib>
using namespace std;

//Variables
int WIDTH,HEIGHT;
int xii,yii,xff,yff,n,asd;
int submenu, submenu_fi, submenu_tipo;
int submenu_c_line,submenu_c_fill,submenu_c_canva;
GLushort pt;
Color3f canva;
Color3f c_line;
Color3f c_fill;
TipoFig fig_sel;
Mystate estado;
PT actual;
L fi;
bool bandera,bandera_e;

/* Contendra todas las figuras creadas */
vector <Figura*> figuras;
/* Figura actual y fugura anterior*/
Figura* fig, *fig_last;

void create_file(){
	FILE * f;
	Punto *as;
	f=fopen("fig.log","w");
	fprintf(f,"%d\n",figuras.size());
	fprintf(f,"%f %f %f ",canva.r,canva.g,canva.b);
	for (int i=0;i<figuras.size();i++){
		fig = figuras.at(i);
		c_line=fig->linea;
		c_fill=fig->fondo;
		fprintf(f,"%d %x %f %f %f %f %f %f",fig->type,fig->patron,c_line.r,c_line.g,c_line.b,c_fill.r,c_fill.g,c_fill.b);
		
		fprintf(f," %f %f %d %d %f",fig->e1,fig->e2,fig->xt,fig->yt,fig->angulo);
		
		if (fig->type != LINE){
			fprintf(f," %d",fig->llenado);
				if (fig->type == CUR_B){
					fprintf(f," %d",fig->i+1);
				}
		}
		as=fig->puntos;
		for (int j=0;j<=fig->i;j++){
			fprintf(f," %d %d",as[j].x,as[j].y);
		}
		fprintf(f,"\n");
	}
	fclose(f);
}


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
void line(GLint xi,GLint yi,GLint xf,GLint yf,GLint xc,GLint yc,float cose,float seno,GLushort patron){
	
	int dx=xf-xi, dy=(HEIGHT-yf)-(HEIGHT-yi);
	int m=((dx>=0 && dy>=0)||(dx<0 && dy<0))? 1:-1;
	int y=0, y2=0,d=0,d1,d2;
	int x=min(xi,xf), x2 = max(xf,xi);
	GLushort actual=1;
	if (m==1){ y=max(yi,yf); y2=min(yi,yf);	}
	else{ y=min(yi,yf); y2=max(yi,yf);	}
	dx=x2-x; dy=(HEIGHT-y2)-(HEIGHT-y);
	float xs,ys;
	glBegin(GL_POINTS);
	if (isDrawpixel(patron,&actual)){
		xs=xc+(cose*(x-xc)-seno*(y-yc));
		ys=yc+(seno*(x-xc)+cose*(y-yc));		
		glVertex2f(xs,ys);
	}
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
			if (isDrawpixel(patron,&actual)){
				xs=xc+(cose*(x-xc)-seno*(y-yc));
				ys=yc+(seno*(x-xc)+cose*(y-yc));
				glVertex2f(xs,ys);
			}
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
			if (isDrawpixel(patron,&actual)){
				xs=xc+(cose*(x-xc)-seno*(y-yc));
				ys=yc+(seno*(x-xc)+cose*(y-yc));
				glVertex2f(xs,ys);
			}
		}
	
	}
	glEnd();
}

void prueba(double angulo){	
	//glRectf(-0.1f,-0.1f,0.1f,0.1f);
	double angle = PI*(angulo/180.0);
	//GLdouble ma[]={cos(angle),sin(angle),0,0,-sin(angle),cos(angle),0,0,0,0,1,0,0,0,0,1};
	//GLdouble ma1[]={1,0,0,0,0,1,0,0,0,0,1,0,200,200,0,1};
	//GLdouble ma2[]={1,0,0,0,0,1,0,0,0,0,1,0,-200,-200,0,1};
	glPushMatrix();
	//glMultMatrixd(ma1);
	//glMultMatrixd(ma);
	//glMultMatrixd(ma2);
	//glMultMatrixd(ma);

	glTranslated(450,450,0);
	glTranslated(-200,0,0);
	glScaled(2,2,0);
	glRotated(45.0,0,0,1);
	glTranslated(-450,-450,0);
	//glRectd(0.35,0.45,0.65,0.55);
	glBegin(GL_LINES);
	glVertex2i(400,400);
	glVertex2i(500,500);
	//glVertex2i(200,200);
	//glVertex2i(200,200);
	glEnd();
	glPopMatrix();

	glFlush();
}

void create_figura()
{
	switch(fig_sel)
	{
		case LINE: fig = new Linea(xii,yii,pt,c_line); figuras.push_back(fig); actual=NEW; break;
		case REC: fig = new Rectangulo(xii,yii,pt,c_line,c_fill,fi); figuras.push_back(fig); actual=NEW; break;
		case TRI: fig = new Triangulo(xii,yii,pt,c_line,c_fill,fi); figuras.push_back(fig); actual=NEW; break;
		case CIRC: fig = new Circulo(xii,yii,pt,c_line,c_fill,fi); figuras.push_back(fig); actual=NEW; break;
		case ELIP: fig = new Elipse(xii,yii,pt,c_line,c_fill,fi); figuras.push_back(fig); actual=NEW; break;
		case CUR_B: fig = new Curva_bezier(xii,yii,pt,c_line,c_fill,fi,n); figuras.push_back(fig); actual=NEW; 	break;	
	}
}

void display(void)
{
	glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
	glClear (GL_COLOR_BUFFER_BIT);
	glClearColor (canva.r,canva.g,canva.b, 0.0);
	WIDTH=glutGet(GLUT_WINDOW_WIDTH);
	HEIGHT=glutGet(GLUT_WINDOW_HEIGHT);
	glOrtho (0, WIDTH, HEIGHT, 0,-1,0);
	for(size_t i = 0;i<figuras.size(); i++)
	{
		figuras.at(i) -> draw();
	}
	glFlush();
	//for (double t=0;t<=360;t+=1.0){
		//prueba(45.0);
	//}
}
void limpiar(){
	while (!figuras.empty())
		figuras.pop_back();
	display();
}
void salir(){
	create_file();
	limpiar();
	exit(0);
}
void menu(int value){
	switch(value){
		case 1: fig_sel = LINE;	break;
		case 2:	fig_sel = TRI;	break;
		case 3:	fig_sel = REC;	break;
		case 4: fig_sel = ELIP;	break;
		case 5: fi = FILL;	break;
		case 6:	fi = NO_FILL;	break;
		case 7:	printf("Ingrese un valor inicial para la curva de Bezier\n"); scanf("%d",&n); 
			fig_sel = CUR_B; break;
		case 8: fig_sel = CIRC;	break;
		case 9: pt=0xFFFF;	break;
		case 10: pt=0x0F0F;	break;
		case 11: pt=0x7D7D;	break;
		case 12: limpiar();	break;
		case 13: 	break;
		case 0: salir(); break;
	}
}
void menu_line(int value){
	switch(value){
		case 1: c_line=Color3f(0.0,0.0,0.0); break;
		case 2:	c_line=Color3f(1.0,1.0,1.0); break;
		case 3:	c_line=Color3f(0.0,0.0,1.0); break;
		case 4: c_line=Color3f(0.0,1.0,0.0); break;
		case 5: c_line=Color3f(1.0,0.0,0.0); break;
		case 6:	c_line=Color3f(1.0,1.0,0.0); break;
		case 7:	c_line=Color3f(0.5,0.5,0.5); break;
		case 8: c_line=Color3f(0.75,0.75,0.75); break;
		case 9: c_line=Color3f(0.78125,0.390625,0.0); break;
		case 0: c_line=Color3f(0.5,0.0,0.5); break;
	}
}
void menu_fill(int value){
	switch(value){
		case 1: c_fill=Color3f(0.0,0.0,0.0); break;
		case 2:	c_fill=Color3f(1.0,1.0,1.0); break;
		case 3:	c_fill=Color3f(0.0,0.0,1.0); break;
		case 4: c_fill=Color3f(0.0,1.0,0.0); break;
		case 5: c_fill=Color3f(1.0,0.0,0.0); break;
		case 6:	c_fill=Color3f(1.0,1.0,0.0); break;
		case 7:	c_fill=Color3f(0.5,0.5,0.5); break;
		case 8: c_fill=Color3f(0.75,0.75,0.75); break;
		case 9: c_fill=Color3f(0.78125,0.390625,0.0); break;
		case 0: c_fill=Color3f(0.5,0.0,0.5); break;
	}
}
void menu_canva(int value){
	switch(value){
		case 1: canva=Color3f(0.0,0.0,0.0); break;
		case 2:	canva=Color3f(1.0,1.0,1.0); break;
		case 3:	canva=Color3f(0.0,0.0,1.0); break;
		case 4: canva=Color3f(0.0,1.0,0.0); break;
		case 5: canva=Color3f(1.0,0.0,0.0); break;
		case 6:	canva=Color3f(1.0,1.0,0.0); break;
		case 7:	canva=Color3f(0.5,0.5,0.5); break;
		case 8: canva=Color3f(0.75,0.75,0.75); break;
		case 9: canva=Color3f(0.78125,0.390625,0.0); break;
		case 0: canva=Color3f(0.5,0.0,0.5); break;
	}
	display();
}
void create_menu(){

	submenu = glutCreateMenu(menu);
	glutAddMenuEntry("Linea", 1);
	glutAddMenuEntry("Triangulo", 2);
	glutAddMenuEntry("Rectangulo", 3);
	glutAddMenuEntry("Circulo", 8);
	glutAddMenuEntry("Elipse", 4);
	glutAddMenuEntry("Curva de bezier", 7);

	submenu_fi = glutCreateMenu(menu);
	glutAddMenuEntry("Con relleno", 5);
	glutAddMenuEntry("Sin relleno", 6);

	submenu_tipo = glutCreateMenu(menu);
	glutAddMenuEntry("Continua", 9);
	glutAddMenuEntry("Discontinua", 10);
	glutAddMenuEntry("Punto-raya", 11);

	submenu_c_line = glutCreateMenu(menu_line);
	glutAddMenuEntry("Negro", 1);
	glutAddMenuEntry("Blanco", 2);
	glutAddMenuEntry("azul", 3);
	glutAddMenuEntry("verde", 4);
	glutAddMenuEntry("rojo", 5);
	glutAddMenuEntry("amarillo", 6);
	glutAddMenuEntry("gris", 7);
	glutAddMenuEntry("plata", 8);
	glutAddMenuEntry("marron", 9);
	glutAddMenuEntry("morado", 0);

	submenu_c_fill = glutCreateMenu(menu_fill);
	glutAddMenuEntry("Negro", 1);
	glutAddMenuEntry("Blanco", 2);
	glutAddMenuEntry("azul", 3);
	glutAddMenuEntry("verde", 4);
	glutAddMenuEntry("rojo", 5);
	glutAddMenuEntry("amarillo", 6);
	glutAddMenuEntry("gris", 7);
	glutAddMenuEntry("plata", 8);
	glutAddMenuEntry("marron", 9);
	glutAddMenuEntry("morado", 0);

	submenu_c_canva = glutCreateMenu(menu_canva);
	glutAddMenuEntry("Negro", 1);
	glutAddMenuEntry("Blanco", 2);
	glutAddMenuEntry("azul", 3);
	glutAddMenuEntry("verde", 4);
	glutAddMenuEntry("rojo", 5);
	glutAddMenuEntry("amarillo", 6);
	glutAddMenuEntry("gris", 7);
	glutAddMenuEntry("plata", 8);
	glutAddMenuEntry("marron", 9);
	glutAddMenuEntry("morado", 0);

	glutCreateMenu(menu);
	glutAddSubMenu("Mi implementacion", submenu);
	glutAddSubMenu("Tipo de linea", submenu_tipo);
	glutAddSubMenu("Relleno", submenu_fi);
	glutAddSubMenu("Color de la linea", submenu_c_line);
	glutAddSubMenu("Color de relleno", submenu_c_fill);
	glutAddSubMenu("Color del lienzo", submenu_c_canva);
	glutAddMenuEntry("Limpiar         (C)", 12);
	glutAddMenuEntry("salir           (Q)", 0);

}
void init (void) 
{
	canva=Color3f(1.0,1.0,1.0);
	c_line=Color3f(0.0,0.0,0.0);
	c_fill=Color3f(0.0,0.0,0.0);
	glClearColor (canva.r,canva.g,canva.b, 0.0);
    glColor3f (c_line.r,c_line.g,c_line.b);
	xii=yii=xff=yff=0;
	fig=NULL;
	fig_sel=LINE;
	actual=NEW;
	estado=UP;
	pt=0xFFFF;
	fi=NO_FILL;
	bandera=false;

	create_menu();

	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glEnable(GL_LINE_STIPPLE);

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
		fig -> setPointEnd(xff,yff,actual);
		actual=OLD;
		display();
	 }
}
void motion_mouse(int x, int y)
 {
	 if (estado)
	 {
		xff=x;
		yff=y;
		if (bandera){fig_last -> setPoint(asd,xff,yff);}
		else{fig -> setPointEnd(xff,yff,actual);}
		actual=OLD;
		display();
	 }
 }
void mouse(int button, int state, int x, int y) 
{	
	if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
		//printf("%d %d 1\n",x,y);
		xii=x;
		yii=y;

		if (actual==NEW)
			create_figura();
		if (bandera){
			asd=fig_last->buscar(x,y);
			if (asd==-1)
			{
				actual=NEW;
				estado=UP;
				bandera=false;
			}
		}
		if (bandera_e){
			asd=fig_last->buscar(x,y);
			if (asd==-1)
			{
				bandera_e=false;
			}else{
				fig_last->eliminar(asd);
				bandera_e=false;
			}
		}
		estado=DOWN;
		//prueba();
	}
	if (button==GLUT_LEFT_BUTTON && state==GLUT_UP){
		//printf("%d %d 2\n",x,y);
		xff=x;
		yff=y;
	
		if (actual==OLD || actual==NEW){
			motion_mouse(x,y);
			if (fig != NULL && fig->haypuntofinal()){
				estado=UP;
				fig_last=fig;
				fig=NULL;

			}
			if (bandera){
				actual=NEW;
				estado=UP;
				bandera=false;
			}
		}
		actual=NEW;
	}
}
void teclado(unsigned char key, int, int y){

	if (key == 'c' || key == 'C')
		limpiar();
	if (key == 'q' || key == 'Q')
		salir();
	if (key == 'r' || key == 'R'  && fig==NULL){
		float d;
		printf("Ingrese un angulo para la rotacion\n");
		scanf("%f",&d);
		d=fmod(d,360);
		fig_last -> rotar(d);
		display();
	}
	if (key == 'e' || key == 'E'  && fig==NULL){
		float d,s;
		printf("Ingrese un factor de escalamiento para 'x' y 'y'\n");
		scanf("%f %f",&d,&s);
		fig_last -> escalar(d,s);
		display();
	}
	if (key == 't' || key == 'T'  && fig==NULL){
		GLint d,s;
		printf("Ingrese un valor de traslacion para 'x' y 'y'\n");
		scanf("%d %d",&d,&s);
		fig_last -> traladar(d,s);
		display();
	}
	if (key == 'a' || key == 'A'  && fig_last!=NULL && fig_last->type==CUR_B){
		fig=fig_last;
		actual=NEW;
		fig->addpoint();
		estado=DOWN;
		display();
	}
	if (key == 'm' || key == 'M'  && fig_last!=NULL && fig_last->type==CUR_B){
		actual=OLD;
		estado=DOWN;
		bandera=true;
		display();
	}
	if (key == 'd' || key == 'D'  && fig_last!=NULL && fig_last->type==CUR_B){
		bandera_e=true;
		display();
	}
}

void read_file(){
	FILE * f;
	int n,x,y,n_p=0;
	float es1,es2,an;

	f=fopen("fig.log","r");
	if (f!=NULL && !feof(f)){
		fscanf(f,"%d",&n);
		fscanf(f,"%f %f %f",&canva.r,&canva.g,&canva.b);
		for (int i=0;i<n;i++){
			fscanf(f,"%d %x %f %f %f %f %f %f",&fig_sel,&pt,&c_line.r,&c_line.g,&c_line.b,&c_fill.r,&c_fill.g,&c_fill.b);

			fscanf(f," %f %f %d %d %f",&es1,&es2,&x,&y,&an);

			if (fig_sel != LINE){
				fscanf(f," %d",&fi);
					if (fig_sel == CUR_B){
						fscanf(f," %d",&n_p);
					}
			}
			if (fig_sel == TRI)
				n_p=3;
			if (fig_sel != TRI || fig_sel != CUR_B)
				n_p=2;
			fscanf(f,"%d %d",&xii,&yii);
			create_figura();
			for (int j=1;j<n_p;j++){
				fscanf(f,"%d %d",&xff,&yff);
				fig -> setPointEnd(xff,yff,PT::NEW);

			}
			fig->e1=es1;
			fig->e2=es2;
			fig->xt=x;
			fig->yt=y;
		}
		fclose(f);
	}
	
}

/* 
 */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (800, 600);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-800)/2, (glutGet(GLUT_SCREEN_HEIGHT)-600)/2); 
    glutCreateWindow ("Tarea 2");
	read_file();
    init ();
    glutDisplayFunc(display); 
	glutMouseFunc(mouse);
	glutMotionFunc(motion_mouse);
	glutKeyboardFunc(teclado);
	glutPassiveMotionFunc(pasivo);
    glutMainLoop();
    return 0;
}//27 ESC , c es la letra como tal
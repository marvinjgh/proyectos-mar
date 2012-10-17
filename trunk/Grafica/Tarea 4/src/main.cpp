/* Tarea 4 */
#pragma comment (lib, "lib/freeglut.lib")
#pragma comment (lib, "lib/AntTweakBar.lib")
#include "GL\AntTweakBar.h"
#include "GL\freeglut.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <Commdlg.h>
#include <windows.h>
#include <vector>
#include <cmath>

#if !defined(_WIN32) && !defined(_WIN64)
#   define _snprintf snprintf
#endif

using namespace std;

/* colores */
GLfloat c_line[]={ 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat c_point[]={ 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat c_fill[]={ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat c_norm[]={ 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat c_box[]={ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat c_bgtop[]={0.3f, 0.0f, 0.0f, 1.0f };
GLfloat c_bgbottom[]={0.9f,0.4f,0.0f, 1.0f };
GLfloat mat_specular[]={0.9f,0.9f,0.9f, 1.0f };

/* booleanos de activacion 
punto, linea, relleno, norm, box, back , z-b
*/
bool bools[]={false,false,true,false,false,true,true};

/* # de vertices, # de polínomios */
size_t vert, face;

/* x min, y min, z min, x max, y max, z max*/
float xmin, ymin, zmin, xmax, ymax, zmax;

float coef;

/* Quaternio, Quaternio inicial */
float q_rotate[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float q_rotateStart[] = { 0.0f, 0.0f, 0.0f, 1.0f };


/* */
float tralacion[]={0.0f,0.0f,0.0f};
float zoom=1.0f;
float zoom_nor=0.4f;

/* estructura que define un punto */
struct Punto {
	GLfloat p[3];
};

struct Vertice{
	Punto* point;
	GLfloat normal[3];
	GLuint n;
	void acum_normal(GLfloat n0,GLfloat n1,GLfloat n2){
		normal[0]+=n0;
		normal[1]+=n1;
		normal[2]+=n2;
		++n;
	}
	void cal_normal(){
		normal[0]/=n;
		normal[1]/=n;
		normal[2]/=n;
	}
};

/* estructura que define un Triangulo */
struct Triangulo {
	Vertice *p[3];
	Triangulo *next;
	GLfloat c[3];
	GLfloat normal[3];
	void cal_normal(){
		c[0]=(p[0]->point->p[0]+p[1]->point->p[0]+p[2]->point->p[0])/3;
		c[1]=(p[0]->point->p[1]+p[1]->point->p[1]+p[2]->point->p[1])/3;
		c[2]=(p[0]->point->p[2]+p[1]->point->p[2]+p[2]->point->p[2])/3;

		GLfloat u[]={p[1]->point->p[0]-p[0]->point->p[0],p[1]->point->p[1]-p[0]->point->p[1],p[1]->point->p[2]-p[0]->point->p[2]};
		GLfloat v[]={p[2]->point->p[0]-p[0]->point->p[0],p[2]->point->p[1]-p[0]->point->p[1],p[2]->point->p[2]-p[0]->point->p[2]};

		normal[0]=u[1]*v[2]-u[2]*v[1];
		normal[1]=u[2]*v[0]-u[0]*v[2];
		normal[2]=u[0]*v[1]-u[1]*v[0];

	}
	void normalizar(float deno,float x,float y,float z){
		c[0]=(2*(c[0]-x))/deno+(xmin+xmax);
		c[1]=(2*(c[1]-y))/deno+(ymin+ymax);
		c[2]=(2*(c[2]-z))/deno+(zmin+zmax);
		float d=sqrtf(pow(normal[0],2)+pow(normal[1],2)+pow(normal[2],2));
		normal[0]/=d;
		normal[1]/=d;
		normal[2]/=d;

		p[0]->acum_normal(normal[0],normal[1],normal[2]);
		p[1]->acum_normal(normal[0],normal[1],normal[2]);
		p[2]->acum_normal(normal[0],normal[1],normal[2]);
	}
};

struct Light
{
    bool    Active;     
    float   Pos[4];     
    float   Ambient[4];
	float   Diffuse[4];
	float   Specular[4];
    float   Cons_ate, Line_ate, Qua_ate; // light initial cylindrical coordinates and speed
    char    Name[4];    // light short name (will be named "1", "2", "3",...)
};

enum Sombreado { FLAT , GOURAUD}; 

Light lights[8];
vector<Vertice*> vertices;
vector<Triangulo*> faces;
Sombreado shadow;

void init(){

	for (int i = 0;i<8;i++){
		lights[i].Ambient[0]=lights[i].Ambient[1]=lights[i].Ambient[2]=lights[i].Ambient[3]=0.0f;
		lights[i].Diffuse[0]=lights[i].Diffuse[1]=lights[i].Diffuse[2]=lights[i].Diffuse[3]=0.0f;
		lights[i].Specular[0]=lights[i].Specular[1]=lights[i].Specular[2]=lights[i].Specular[3]=0.0f;
		lights[i].Pos[0]=lights[i].Pos[1]=lights[i].Pos[2]=lights[i].Pos[3]=-1.0;
		lights[i].Cons_ate=1.0;lights[i].Line_ate=0.0;lights[i].Qua_ate=0.0;
		_snprintf(lights[i].Name,sizeof(lights[i].Name),"l%d",i);
	}
	TwBar *light= TwNewBar("Luz");
	TwDefine(" Luz label='Iluminacion' refresh=0.5 position='580 16' alpha=0 color='0 0 0'");	
	//struct light ambient
	TwStructMember lightMembers_amb[]=
	{
		 { "Activar",    TW_TYPE_BOOLCPP, offsetof(Light, Active),    " help='Activar/desactivar Luz ambienta.' " },
		 { "Color",     TW_TYPE_COLOR3F, offsetof(Light, Ambient),     " help='Color de la luz.' " }
	};
	TwType light_ambType = TwDefineStruct("Light ambient", lightMembers_amb, 2, sizeof(Light), NULL, NULL);
	
	//struct light diffuse
	TwStructMember lightMembers_diff[]=
	{
		 { "Activar",	TW_TYPE_BOOLCPP,	offsetof(Light, Active),	" help='Activar/desactivar Luz difusa.' " },
		 { "Color ",	TW_TYPE_COLOR3F,	offsetof(Light, Diffuse),	" help='Color de la luz.' " },
		 { "Posicion",	TW_TYPE_DIR3F,		offsetof(Light, Pos),		" help='Posicion de la luz.' " },
		 { "Const. Attenuation", TW_TYPE_FLOAT, offsetof(Light, Cons_ate), 
															" help='Constant Attenuation.' min=0.00 max=1.00 step=0.01"},
		 { "Linear Attenuation", TW_TYPE_FLOAT, offsetof(Light, Line_ate), 
															" help='Linear Attenuation.' min=0.00 max=1.00 step=0.01"},
		 { "Quadratic Attenuation", TW_TYPE_FLOAT, offsetof(Light, Qua_ate), 
															" help='Quadratic Attenuation.' min=0.00 max=1.00 step=0.01"}
	};

	TwType light_diffType = TwDefineStruct("Light diffuse", lightMembers_diff, 6, sizeof(Light), NULL, NULL);

	//struct light specular
	TwStructMember lightMembers_spe[]=
	{
		 { "Activar",	TW_TYPE_BOOLCPP,	offsetof(Light, Active),	" help='Activar/desactivar Luz especular.' " },
		 { "Color ",	TW_TYPE_COLOR3F,	offsetof(Light, Specular),	" help='Color de la luz.' " },
		 { "Posicion",	TW_TYPE_DIR3F,		offsetof(Light, Pos),		" help='Posicion de la luz.' " },
		 { "Const. Attenuation", TW_TYPE_FLOAT, offsetof(Light, Cons_ate), 
															" help='Constant Attenuation.' min=0.00 max=1.00 step=0.01"},
		 { "Linear Attenuation", TW_TYPE_FLOAT, offsetof(Light, Line_ate), 
															" help='Linear Attenuation.' min=0.00 max=1.00 step=0.01"},
		 { "Quadratic Attenuation", TW_TYPE_FLOAT, offsetof(Light, Qua_ate), 												" help='Quadratic Attenuation.' min=0.00 max=1.00 step=0.01"}
	};
	TwType light_speType = TwDefineStruct("Light specular", lightMembers_spe, 6, sizeof(Light), NULL, NULL);

	//struct light all
	TwStructMember lightMembers_all[]=
	{
		 { "Activar",	TW_TYPE_BOOLCPP,	offsetof(Light, Active),	" help='Activar/desactivar Luz especular.' " },
		 { "Ambient",     TW_TYPE_COLOR3F, offsetof(Light, Ambient),     " help='Color de la luz.' " },
		 { "Diffuse ",	TW_TYPE_COLOR3F,	offsetof(Light, Diffuse),	" help='Color de la luz.' " },
		 { "Specular ",	TW_TYPE_COLOR3F,	offsetof(Light, Specular),	" help='Color de la luz.' " },
		 { "Const. Attenuation", TW_TYPE_FLOAT, offsetof(Light, Cons_ate), 
															" help='Constant Attenuation.' min=0.00 max=1.00 step=0.01"},
		 { "Linear Attenuation", TW_TYPE_FLOAT, offsetof(Light, Line_ate), 
															" help='Linear Attenuation.' min=0.00 max=1.00 step=0.01"},
		 { "Quadratic Attenuation", TW_TYPE_FLOAT, offsetof(Light, Qua_ate), 
															" help='Quadratic Attenuation.' min=0.00 max=1.00 step=0.01"},
		 { "Posicion",	TW_TYPE_DIR3F,		offsetof(Light, Pos),		" help='Posicion de la luz.' " },
	};

	TwType light_allType = TwDefineStruct("Light", lightMembers_all, 8, sizeof(Light), NULL, NULL);

	lights[0].Ambient[0]=lights[0].Ambient[1]=lights[0].Ambient[2]=lights[0].Ambient[3]=1.0f;
	lights[0].Pos[0]=lights[0].Pos[1]=lights[0].Pos[2]=lights[0].Pos[3]=0.0f;

	lights[1].Ambient[0]=0.2f;lights[1].Ambient[1]=0.0f;lights[1].Ambient[2]=lights[1].Ambient[3]=1.0f;
	lights[1].Pos[0]=lights[1].Pos[1]=lights[1].Pos[2]=lights[1].Pos[3]=0.0f;

	TwAddVarRW(light, lights[0].Name, light_ambType,&lights[0],
               " label='Light0' help='Luz ambiental.' group='Luz Ambiental'");
	TwAddVarRW(light, lights[1].Name, light_ambType,&lights[1],
               " label='Light1' help='Luz ambiental.' group='Luz Ambiental'");

	TwAddSeparator(light,"s0","");

	lights[2].Diffuse[0]=1.0f;lights[2].Diffuse[1]=lights[2].Diffuse[2]=0.0;lights[2].Diffuse[3]=1.0f;
	lights[3].Diffuse[1]=1.0f;lights[3].Diffuse[0]=lights[3].Diffuse[2]=0.0;lights[3].Diffuse[3]=1.0f;
	TwAddVarRW(light, lights[2].Name, light_diffType,&lights[2],
               " label='Light2' help='Luz Difusa.' group='Luz Difusa'");
	TwAddVarRW(light, lights[3].Name, light_diffType,&lights[3],
               " label='Light3' help='Luz Difusa.' group='Luz Difusa'");
	
	TwAddSeparator(light,"s1","");
	lights[4].Specular[0]=1.0f;lights[4].Specular[1]=lights[4].Specular[2]=0.0;lights[4].Specular[3]=1.0f;
	lights[5].Specular[1]=1.0f;lights[5].Specular[0]=lights[5].Specular[2]=0.0;lights[5].Specular[3]=1.0f;
	TwAddVarRW(light, lights[4].Name, light_speType,&lights[4],
               " label='Light4' help='Luz Especular.' group='Luz Especular'");
	TwAddVarRW(light, lights[5].Name, light_speType,&lights[5],
               " label='Light5' help='Luz Especular.' group='Luz Especular'");

	TwAddSeparator(light,"s2","");

	TwAddVarRW(light, lights[6].Name, light_allType,&lights[6],
               " label='Light6' help='Luz.' group='Otros'");
	TwAddVarRW(light, lights[7].Name, light_allType,&lights[6],
               " label='Light7' help='Luz.' group='Otros'");

	TwAddSeparator(light,"s3","");
	TwEnumVal sombra[2] = { { FLAT , "Flat"}, { GOURAUD, "Gouraud"} };
	TwType shadowType = TwDefineEnum("shadowType", sombra, 2);
	TwAddVarRW (light, "sombra", shadowType, &shadow, " label=' Tipo de sombra' keyIncr='<' keyDecr='>' help='Cambia el tipo de sombreado.' ");
}

/* Funcion pintar
 * Dibuja el objeto, con un parametro mode, que define si sera debujara
 * unicamente puntos, lineas o figuras con relleno
 */
void pintar(GLenum mode){
	glPolygonMode(GL_FRONT_AND_BACK,mode);
	Triangulo *triangle;
	for (size_t i=0;i<face;i++){
		glBegin(GL_TRIANGLES);
		triangle=faces.at(i);
		for (size_t j=0;j<3;j++){
			if (shadow==FLAT){
				glNormal3fv(triangle->normal);
			}else{
				glNormal3fv(triangle->p[j]->normal);
			}
			glVertex3fv(triangle->p[j]->point->p);
		}
		glEnd();
	}
}

/* Funcion box
 * Pinta el Bounding Box
 */
void box(){
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin( GL_LINE_LOOP ); // lateral superior
		glVertex3f(xmax,ymax,zmax);
		glVertex3f(xmax,ymax,zmin);
		glVertex3f(xmin,ymax,zmin);
		glVertex3f(xmin,ymax,zmax);
	glEnd();
	glBegin( GL_LINE_LOOP ); // lateral inferior
		glVertex3f(xmax,ymin,zmax);
		glVertex3f(xmax,ymin,zmin);
		glVertex3f(xmin,ymin,zmin);
		glVertex3f(xmin,ymin,zmax);
	glEnd();
	glBegin( GL_LINE_LOOP ); // lateral derecho
		glVertex3f(xmax,ymin,zmax);
		glVertex3f(xmax,ymin,zmin);
		glVertex3f(xmax,ymax,zmin);
		glVertex3f(xmax,ymax,zmax);
	glEnd();
	glBegin( GL_LINE_LOOP ); // lateral izquierdo
		glVertex3f(xmin,ymin,zmax);
		glVertex3f(xmin,ymin,zmin);
		glVertex3f(xmin,ymax,zmin);
		glVertex3f(xmin,ymax,zmax);
	glEnd();

}

void normals(){
	Triangulo *t;
	for (size_t i=0;i<face;i++){
		t=faces.at(i);
		glBegin(GL_LINES);
		glVertex3f(t->normal[0]*zoom_nor+t->c[0],t->normal[1]*zoom_nor+t->c[1],t->normal[2]*zoom_nor+t->c[2]);
		glVertex3fv(t->c);
		glEnd();
	}
}

void normals_p(){
	Vertice *v;
	for (size_t i=0;i<vert;i++){
		v=vertices.at(i);
		glBegin(GL_LINES);
		glVertex3f(v->normal[0]*zoom_nor+v->point->p[0],v->normal[1]*zoom_nor+v->point->p[1],v->normal[2]*zoom_nor+v->point->p[2]);
		glVertex3fv(v->point->p);
		glEnd();
	}
	glFlush();
}

void rotacion(const float *quat, float *mat)
{
    float yy2 = 2.0f * quat[1] * quat[1];
    float xy2 = 2.0f * quat[0] * quat[1];
    float xz2 = 2.0f * quat[0] * quat[2];
    float yz2 = 2.0f * quat[1] * quat[2];
    float zz2 = 2.0f * quat[2] * quat[2];
    float wz2 = 2.0f * quat[3] * quat[2];
    float wy2 = 2.0f * quat[3] * quat[1];
    float wx2 = 2.0f * quat[3] * quat[0];
    float xx2 = 2.0f * quat[0] * quat[0];
    mat[0] = - yy2 - zz2 + 1.0f;
	mat[4] = xy2 - wz2;
	mat[8] = xz2 + wy2;

    mat[1] = xy2 + wz2;
	mat[5] = - xx2 - zz2 + 1.0f;
    mat[9] = yz2 - wx2;

    mat[2] = xz2 - wy2;
    mat[6] = yz2 + wx2;
	mat[10] = - xx2 - yy2 + 1.0f;

	mat[3] = mat[7] = mat[11] = mat[12] = mat[13] = mat[14] = 0;

    mat[15] = 1;
}

void normalizar(){
	size_t i;
	float x=(xmax+xmin)/2,y=(ymax+ymin)/2,z=(zmax+zmin)/2;
	float denox=xmax-xmin;
	float denoy=ymax-ymin;
	float denoz=zmax-zmin;
	float deno=max(max(denox,denoy),denoz);
	float aux=xmin+xmax-2*x;
	xmin=2*(xmin-x)/deno+aux;
	xmax=2*(xmax-x)/deno+aux;
	aux=ymin+ymax-2*y;
	ymin=2*(ymin-y)/deno+aux;
	ymax=2*(ymax-y)/deno+aux;
	aux=zmin+zmax-2*z;
	zmin=2*(zmin-z)/deno+aux;
	zmax=2*(zmax-z)/deno+aux;
	
	Punto *point;
	for (i=0;i<vert;i++){
		point=vertices.at(i)->point;
		point->p[0]=(2*(point->p[0]-x))/deno+(xmin+xmax);
		point->p[1]=(2*(point->p[1]-y))/deno+(ymin+ymax);
		point->p[2]=(2*(point->p[2]-z))/deno+(zmin+zmax);
	}
	for (i=0;i<face;i++){
		faces.at(i)->normalizar(deno,x,y,z);
	}
	for (i=0;i<vert;i++){
		vertices.at(i)->cal_normal();
	}
}

void luz(){
	float v[4];
	for (int i=0;i<8;i++){
		glLightfv(GL_LIGHT0+i, GL_AMBIENT, lights[i].Ambient);
		glLightfv(GL_LIGHT0+i, GL_DIFFUSE, lights[i].Diffuse);
		glLightfv(GL_LIGHT0+i, GL_SPECULAR, lights[i].Specular);
		glLightf(GL_LIGHT0+i, GL_CONSTANT_ATTENUATION, lights[i].Cons_ate);
		glLightf(GL_LIGHT0+i, GL_LINEAR_ATTENUATION, lights[i].Line_ate);
		glLightf(GL_LIGHT0+i, GL_QUADRATIC_ATTENUATION, lights[i].Qua_ate);
		v[0]=-lights[i].Pos[0];v[1]=-lights[i].Pos[1];v[2]=-lights[i].Pos[2];v[3]=-lights[i].Pos[3];
		glLightfv(GL_LIGHT0+i, GL_POSITION, v);
		if (lights[i].Active)
			glEnable(GL_LIGHT0+i);
		else
			glDisable(GL_LIGHT0+i);
	}
	glDisable(GL_SMOOTH);
	glDisable(GL_NORMALIZE);
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//inicio el fondo
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glBegin(GL_QUADS);
		//bottom
		glColor3fv(c_bgbottom);
        glVertex3f(-1, -1, 0.9f); 
        glVertex3f(1, -1, 0.9f); 
		//top
		glColor3fv(c_bgtop);
        glVertex3f(1, 1, 0.9f);
        glVertex3f(-1, 1, 0.9f);
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
	//fin de el fondo
	
	glLoadIdentity();
	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, coef);

	glEnable(GL_COLOR_MATERIAL);
	
	gluLookAt(0,0,3, 0,0,0, 0,1,0);
	luz();
	float matriz[16];
	
	glTranslatef(tralacion[0],tralacion[1],tralacion[2]);
	glScalef(zoom,zoom,zoom);
	rotacion(q_rotate,matriz);
	glMultMatrixf(matriz);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	

	if (bools[6]){
		glEnable(GL_DEPTH_TEST);
	}else{
		glDisable(GL_DEPTH_TEST);
	}
	 
	if (bools[4]){
		glColor4fv(c_box);
		box();
	}
	
	if (bools[5]){
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}else{
		glDisable(GL_CULL_FACE);
	}
	if (bools[2]){
		glColor4fv(c_fill);
		pintar(GL_FILL);
	}
	if (bools[1]){
		glColor4fv(c_line);
		pintar(GL_LINE);
	}
	if (bools[0]){
		glColor4fv(c_point);
		pintar(GL_POINT);
	}
	if(bools[3]){
		glColor3fv(c_norm);
		normals();
		normals_p();
	}
	//glutWireTeapot(3.0);
	
	TwDraw();

	glutSwapBuffers();
    // Recall Display at next frame
    glutPostRedisplay();
}

void reshape(int width, int height){
    glViewport(0, 0, width, height);
    
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	//Defino el clipping space
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0);
	//Activo la matriz de ModelView
    glMatrixMode(GL_MODELVIEW);
    TwWindowSize(width, height);
}

void read(char * Path){
	string str;
	size_t aux,p,n;
	size_t p0,p1;
	fstream f (Path,fstream::in);
	Punto *point;
	Vertice *v;
	Triangulo *triangle;

	if (!f.is_open()){ puts("Error al abrir el archivo off , saliendo del programa..."); exit(0); }
	f>>str;
	if (str=="OFF"){
		f>>vert>>face>>aux;
		vertices.clear();
		faces.clear();
		for (aux=0;aux<vert;aux++){
			point=(Punto*)calloc(1,sizeof(Punto));
			v=(Vertice*)calloc(1,sizeof(Vertice));
			f>>point->p[0]>>point->p[1]>>point->p[2];
			if (aux==0){
				xmin=xmax=point->p[0];
				ymin=ymax=point->p[1];
				zmin=zmax=point->p[2];
			}else{
				xmin=min(xmin,point->p[0]);
				ymin=min(ymin,point->p[1]);
				zmin=min(zmin,point->p[2]);
				xmax=max(xmax,point->p[0]);
				ymax=max(ymax,point->p[1]);
				zmax=max(zmax,point->p[2]);
			}
			v->point=point;
			vertices.push_back(v);
		}
		
		aux=0;
		while (!f.eof()){
			triangle=(Triangulo*)calloc(1,sizeof(Triangulo));
			f>>n;
			f>>p0;
			triangle->p[0]=vertices.at(p0);
			f>>p;
			triangle->p[1]=vertices.at(p);
			f>>p1;
			triangle->p[2]=vertices.at(p1);
			if (n>3){
				faces.push_back(triangle);
				triangle->cal_normal();
				face+=(n-3);
				for (size_t j=0;j<n-3;j++){
					triangle=(Triangulo*)calloc(1,sizeof(Triangulo));
					triangle->p[0]=vertices.at(p0);
					triangle->p[1]=vertices.at(p1);
					f>>p;
					triangle->p[2]=vertices.at(p1=p);
					faces.push_back(triangle);
					triangle->cal_normal();
				}
			}else{
				faces.push_back(triangle);
				triangle->cal_normal();
			}
			aux++;
		}
	}
	f.close();
}

void TW_CALL cargar(void *clientData){
	OPENFILENAME ofn;
	char Path[512]="";
	memset(&ofn, 0, sizeof(ofn));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
	ofn.lpstrFile = Path;
	ofn.nMaxFile = 255;
	ofn.lpstrTitle = "Load Model...";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	ofn.lpstrFilter = "OFF Files\0*.off\0OBJ Files\0*.obj\0\0";//hago que solo muestre archivos .off
    ofn.lpstrDefExt = "off";
	
	if (GetOpenFileName(&ofn)){

		//printf("%s\n",ofn.lpstrFile+ofn.nFileOffset); saber el nombre con extencion (nFileExtension solo ext)

		//printf("%d",ofn.nFilterIndex); saber el filtro usado

		read(Path);//funcion que carga el modelo apartir del path
		normalizar();//normalizo todo
	}
		
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-800)>>1, (glutGet(GLUT_SCREEN_HEIGHT)-600)>>1);
	glutCreateWindow("Tarea 4"); 
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	
	TwInit(TW_OPENGL, NULL);
	
	glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
    glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
    glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
    glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
    glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);
	TwGLUTModifiersFunc(glutGetModifiers);
	
	init();

	TwBar *bar;
	bar = TwNewBar("Barra");
	TwDefine(" GLOBAL help='Aqui se encuentra la ayuda.' ");
    TwDefine(" Barra label='Modelo' refresh=0.5 position='16 16' size='230 520' alpha=0 color='0 0 0'");
	

	//abrir archivo
	TwAddButton(bar, "c", cargar, NULL, 
                " label='Cargar Modelo' key=o help='Carga el archivo off que contiene al model.' ");
	TwAddSeparator(bar,"se","");
	//bloque de la rotacion
    TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &q_rotate, 
               " label='Rotacion' opened=true help='Cambia la orientacion del modelo.' ");
	//traslacion
	TwAddVarRW(bar, "x", TW_TYPE_FLOAT, &tralacion[0], 
		"label='Mover en x' step=0.08 keyIncr='d' keyDecr='a' help='Permite mover el objeto en el eje x(hacia la derecha-izquierda).'");
	TwAddVarRW(bar, "y", TW_TYPE_FLOAT, &tralacion[1], 
		"label='Mover en y' step=0.08 keyIncr='q' keyDecr='e' help='Permite mover el objeto en el eje y(hacia arriba-abajo).");
	TwAddVarRW(bar, "z", TW_TYPE_FLOAT, &tralacion[2], 
		"label='Mover en z' step=0.08 keyIncr='w' keyDecr='s' help='Permite mover el objeto en el eje x(hacia la adelante-atras).");
	//zoom
	TwAddVarRW(bar, "zoom", TW_TYPE_FLOAT, &zoom, 
		"label='Zoom' min=0.01 max=2.5 step=0.01 keyIncr='+' keyDecr='-' help='Escalamiento de la figura.");
	//backface
	TwAddVarRW(bar, "ba", TW_TYPE_BOOLCPP, &bools[5], 
		" label='Activar backface culling' help='Activa el backface culling.' key=v ");
	//z-buffer
	TwAddVarRW(bar, "zb", TW_TYPE_BOOLCPP, &bools[6], 
		" label='Activar Z-Buffer ' help='Activa el Z-Buffer.' key=z ");

	//bloque punto
	TwAddVarRW(bar, "a0", TW_TYPE_BOOLCPP, &bools[0], 
		"label='Activar' help='Activa dibujar las lineas del objeto.' key=p group='Puntos'  ");
	TwAddVarRW(bar, "c0", TW_TYPE_COLOR3F, &c_point, 
		"label='Color' help='Cambia el color de los puntos.' group='Puntos' ");
	//bloque linea
	TwAddVarRW(bar, "a1", TW_TYPE_BOOLCPP, &bools[1],
		" label='Activar' help='Activa dibujar las lineas del objeto.' key=l group='Lineas' ");
	TwAddVarRW(bar, "c1", TW_TYPE_COLOR3F, &c_line, 
		"label='Color' help='Cambia el color de las lineas.' group='Lineas' ");
	//bloque relleno
	TwAddVarRW(bar, "a2", TW_TYPE_BOOLCPP, &bools[2],
		" label='Activar' help='Activa dibujar el relleno del objeto.' key=f group='Relleno' ");
	TwAddVarRW(bar, "c2", TW_TYPE_COLOR3F, &c_fill, 
		"label='Color' help='Cambia el color del relleno.' group='Relleno' ");
	//bloque norma
	TwAddVarRW(bar, "a3", TW_TYPE_BOOLCPP, &bools[3],
		" label='Activar' help='Activa dibujar las normales de los polínomios.' key=n group='Normales' ");
	TwAddVarRW(bar, "b", TW_TYPE_FLOAT, &zoom_nor, 
		"label='Zoom_n' min=0.1 max=1 step=0.05 help='Escalamiento de las normales.' group='Normales' ");
	TwAddVarRW(bar, "c3", TW_TYPE_COLOR3F, &c_norm, 
		"label='Color' help='Cambia el color de las lineas.' group='Normales' ");

	//bloque box
	TwAddVarRW(bar, "a4", TW_TYPE_BOOLCPP, &bools[4], 
		" label='Activar' help='Activa dibujar las lineas del objeto.' key=b group='Bounding box' ");
	TwAddVarRW(bar, "c4", TW_TYPE_COLOR3F, &c_box, 
		"label='Color' help='Cambia el color de las lineas.' group='Bounding box' ");

	//bloque especular
	TwAddVarRW(bar, "c5", TW_TYPE_COLOR3F, &mat_specular, 
		"label='Material especular' help='Cambia el color del material especular.' group='Componente especular' ");
	TwAddVarRW(bar, "c6", TW_TYPE_FLOAT, &coef, 
		"label='Exponente de refrexión especular' help='Cambia el valor del exponente especular.' group='Componente especular' min=0.00 max=128.0 step=0.01");

	//bloque del background
	TwAddVarRW(bar, "BgTop", TW_TYPE_COLOR3F, c_bgtop, 
               " group='Background' help='Cambia el color de superior del background.' ");  // 'BgTop' and 'BgBottom' are put in the group 'Background' (which is then created)
    TwAddVarRW(bar, "BgBottom", TW_TYPE_COLOR3F, c_bgbottom, 
               " group='Background' help='Cambia el color inferior del background.' ");

	//cosas de iluminacion

	glutMainLoop();
	TwTerminate();

	return 0;
}
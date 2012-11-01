/**
  * off.cpp
  * 
  * Creador Marvin Gonzalez
  */

#include "off.h"

/* # de vertices, # de polínomios */
size_t vert, face;

/* x min, y min, z min, x max, y max, z max*/
float xmin, ymin, zmin, xmax, ymax, zmax;

vector<Vertice*> vertices;
vector<Triangulo*> faces;

void Vertice::acum_normal(GLfloat n0,GLfloat n1,GLfloat n2){
	normal[0]+=n0;
	normal[1]+=n1;
	normal[2]+=n2;
	++n;
}
void Vertice::cal_normal(){
	normal[0]/=n;
	normal[1]/=n;
	normal[2]/=n;
}

/* estructura que define un Triangulo */

void Triangulo::cal_normal(){
	c[0]=(p[0]->point->p[0]+p[1]->point->p[0]+p[2]->point->p[0])/3;
	c[1]=(p[0]->point->p[1]+p[1]->point->p[1]+p[2]->point->p[1])/3;
	c[2]=(p[0]->point->p[2]+p[1]->point->p[2]+p[2]->point->p[2])/3;

	GLfloat u[]={p[1]->point->p[0]-p[0]->point->p[0],p[1]->point->p[1]-p[0]->point->p[1],p[1]->point->p[2]-p[0]->point->p[2]};
	GLfloat v[]={p[2]->point->p[0]-p[0]->point->p[0],p[2]->point->p[1]-p[0]->point->p[1],p[2]->point->p[2]-p[0]->point->p[2]};

	normal[0]=u[1]*v[2]-u[2]*v[1];
	normal[1]=u[2]*v[0]-u[0]*v[2];
	normal[2]=u[0]*v[1]-u[1]*v[0];

}

void Triangulo::normalizar(float deno,float x,float y,float z){
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



void pintar(){
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	Triangulo *triangle;
	for (size_t i=0;i<face;i++){
		glBegin(GL_TRIANGLES);
		triangle=faces.at(i);
		for (size_t j=0;j<3;j++){
			glNormal3fv(triangle->p[j]->normal);
			glVertex3fv(triangle->p[j]->point->p);
		}
		glEnd();
	}
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

/*
GLfloat pos[]={0.0f,0.0f,0.0f,1.0f};
gluLookAt(0.0f,1.0f,10.0f,
				0.0,0.0,-1.0,			       
				0.0f,1.0f,0.0f);
glRotatef(angle, 0, 1, 0);
glTranslatef(0,0,-10.f);

glUseProgramObjectARB(p);
angle+=0.1f;
glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, a);
glLightfv(GL_LIGHT0, GL_POSITION, pos);
gluSphere(GlightSphere,0.5,20,20);
	glLoadIdentity();
gluLookAt(0.0f,1.0f,10.0f,
			0.0,0.0,-1.0,			       
			0.0f,1.0f,0.0f);

				*/
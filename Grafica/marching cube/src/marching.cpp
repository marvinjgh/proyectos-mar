#include <AntTweakBar.h>
#include <glsl\Shader.h>
#include <Geom\Punto3D.h>
#include "casos.h"
#include <cstring>
#include <Commdlg.h>
#include <windows.h>
#include <vector>
#include <Geom\Vec3D.h>
#include <Geom\mat4x4.h>
#include "lectura.h"

#define ABS(x) (x < 0 ? -(x) : (x))

Shader sl;

GLfloat q_rotate[] = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat traslacion[] = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat c_fill[]={ 0.0f, 0.0f, 1.0f,1.0f };
GLfloat luz[]={0.f,0.f,-1.f};
GLfloat zoom,actual;
size_t limita;
bool bools[]={false,false};
Mat4x4 m,p;

float umbral=1.f;

class Modelo{
public:
	struct Vertice{
		Punto3D p;
		Vec3D n; 
		int count;
	};

	struct Celda{
		Punto3D p[8];
		Punto3D g[8];
		float val[8];
	};

	Modelo(){
		dimx=dimy=dimz=0;
		b=NULL;
		vol=NULL;
	}

	struct Triangulo{
		Vertice* p[3];	/* Vertices */
		Vec3D normal;
	};

	void inline calcular(Celda* grids,int i,int j,int k){
		Punto3D p(0.f,0.f,0.f);
		grids->p[0].coord[0] = (float)i;
		grids->p[0].coord[1] = (float)j;
		grids->p[0].coord[2] = (float)dimz-k+1;
		grids->val[0] = vol[i][j][k];
		grids->p[1].coord[0] = (float)i+1;
		grids->p[1].coord[1] = (float)j;
		grids->p[1].coord[2] = (float)dimz-k+1; 
		grids->val[1] = vol[i+1][j][k];
		grids->p[2].coord[0] = (float)i+1;
		grids->p[2].coord[1] = (float)j+1;
		grids->p[2].coord[2] = (float)dimz-k+1;
		grids->val[2] = vol[i+1][j+1][k];
		grids->p[3].coord[0] = (float)i;
		grids->p[3].coord[1] = (float)j+1;
		grids->p[3].coord[2] = (float)dimz-k+1;
		grids->val[3] = vol[i][j+1][k];
		grids->p[4].coord[0] = (float)i;
		grids->p[4].coord[1] = (float)j;
		grids->p[4].coord[2] = (float)dimz-k;
		grids->val[4] = vol[i][j][k+1];
		grids->p[5].coord[0] = (float)i+1;
		grids->p[5].coord[1] = (float)j;
		grids->p[5].coord[2] = (float)dimz-k;
		grids->val[5] = vol[i+1][j][k+1];
		grids->p[6].coord[0] = (float)i+1;
		grids->p[6].coord[1] = (float)j+1;
		grids->p[6].coord[2] = (float)dimz-k;
		grids->val[6] = vol[i+1][j+1][k+1];
		grids->p[7].coord[0] = (float)i;
		grids->p[7].coord[1] = (float)j+1;
		grids->p[7].coord[2] = (float)dimz-k;
		grids->val[7] = vol[i][j+1][k+1];

		grids->g[0]=(i!=0 && j!=0 && k!=0)?CALC_GRAD_VERT_0():p;
		grids->g[1]=(i!=dimx-2 && j!=0 && k!=0)?CALC_GRAD_VERT_1():p;
		grids->g[2]=(i!=dimx-2 && j!=dimy-2 && k!=0)?CALC_GRAD_VERT_2():p;
		grids->g[3]=(i!=0 && j!=dimy-2 && k!=0)?CALC_GRAD_VERT_3():p;
		grids->g[4]=(i!=0 && j!=0 && k!=dimz-2)?CALC_GRAD_VERT_4():p;
		grids->g[5]=(i!=dimx-2 && j!=0 && k!=dimz-2)?CALC_GRAD_VERT_5():p;
		grids->g[6]=(i!=dimx-2 && j!=dimy-2 && k!=dimz-2)?CALC_GRAD_VERT_6():p;
		grids->g[7]=(i!=0 && j!=dimy-2 && k!=dimz-2)?CALC_GRAD_VERT_7():p;

	}

	Vertice* buscar_vertice(Vertice* p){
		size_t i= - 1;
		Vertice* aux = NULL;
		/*for (; i >= 0 && i!=-1; --i)
		{
			if (puntos.at(i)->p==p->p)
				puntos.at(i);
		}*/

		if (i==-1){
			aux = new Vertice;
			aux->p=p->p;
			aux->n=p->n;
			aux->count=0;
			puntos.push_back(aux);
		}
		return aux;
	}

	Vertice* VertexInterp(Punto3D* p1,Punto3D* p2,Punto3D* g1,Punto3D* g2,double valp1,double valp2)
	{
		float mu;
		Vertice *p,aux;
		aux.p=*p1;
		aux.n=*g1;
		if (ABS(umbral-valp1) < 0.00001)
			return buscar_vertice(&aux);
		aux.p=*p2;
		aux.n=*g2;
		if (ABS(umbral-valp2) < 0.00001)
			return buscar_vertice(&aux);
		aux.p=*p1;
		aux.n=*g1;
		if (ABS(valp1-valp2) < 0.00001)
			return buscar_vertice(&aux);

		p=new Vertice();
		p->p=*p1;
		p->n=*g1;

		mu = (float)((umbral - valp1) / (valp2 - valp1));

		p->p += mu * (*p2 - *p1);
		p->n += mu * (*g2 - *g1);

		return buscar_vertice(p);
	}

	int PolygoniseCube(Celda g,Triangulo *tri,int index)
	{
		int i,ntri = 0;
		Vertice* vertlist[12];

		/* Find the vertices where the surface intersects the cube */
		if (edgeTable[index] & 1) {
			vertlist[0] = VertexInterp(&g.p[0],&g.p[1],&g.g[0],&g.g[1],g.val[0],g.val[1]);
		}
		if (edgeTable[index] & 2) {
			vertlist[1] = VertexInterp(&g.p[1],&g.p[2],&g.g[1],&g.g[2],g.val[1],g.val[2]);
		}
		if (edgeTable[index] & 4) {
			vertlist[2] = VertexInterp(&g.p[2],&g.p[3],&g.g[2],&g.g[3],g.val[2],g.val[3]);
		}
		if (edgeTable[index] & 8) {
			vertlist[3] = VertexInterp(&g.p[3],&g.p[0],&g.g[3],&g.g[0],g.val[3],g.val[0]);
		}
		if (edgeTable[index] & 16) {
			vertlist[4] = VertexInterp(&g.p[4],&g.p[5],&g.g[4],&g.g[5],g.val[4],g.val[5]);
		}
		if (edgeTable[index] & 32) {
			vertlist[5] = VertexInterp(&g.p[5],&g.p[6],&g.g[5],&g.g[6],g.val[5],g.val[6]);
		}
		if (edgeTable[index] & 64) {
			vertlist[6] = VertexInterp(&g.p[6],&g.p[7],&g.g[6],&g.g[7],g.val[6],g.val[7]);
		}
		if (edgeTable[index] & 128) {
			vertlist[7] = VertexInterp(&g.p[7],&g.p[4],&g.g[7],&g.g[4],g.val[7],g.val[4]);
		}
		if (edgeTable[index] & 256) {
			vertlist[8] = VertexInterp(&g.p[0],&g.p[4],&g.g[0],&g.g[4],g.val[0],g.val[4]);
		}
		if (edgeTable[index] & 512) {
			vertlist[9] = VertexInterp(&g.p[1],&g.p[5],&g.g[1],&g.g[5],g.val[1],g.val[5]);
		}
		if (edgeTable[index] & 1024) {
			vertlist[10] = VertexInterp(&g.p[2],&g.p[6],&g.g[2],&g.g[6],g.val[2],g.val[6]);
		}
		if (edgeTable[index] & 2048) {
			vertlist[11] = VertexInterp(&g.p[3],&g.p[7],&g.g[3],&g.g[7],g.val[3],g.val[7]);
		}

		/* Create the triangles */
		for (i=0;triTable[index][i]!=-1;i+=3) {
			tri[ntri].p[0] = vertlist[triTable[index][i  ]];
			tri[ntri].p[1] = vertlist[triTable[index][i+1]];
			tri[ntri].p[2] = vertlist[triTable[index][i+2]];
			ntri++;
		}

		return(ntri);
	}

	void mc(){
		Celda grids;
		int n=0,ntri=0,index;
		Triangulo triangles[10];

		finales.clear();
		puntos.clear();

		for (int i=0;i<dimx-1;i++) {
			//if (i % (dimx/10) == 0)
			if(i%25 == 0){
				printf("slite %d\n",i);
			}
			for (int j=0;j<dimy-1;j++) {
				for (int k=0;k<dimz-1;k++) {
					index=0;

					if (vol[i][j][k] < umbral) index |= 1;
					if (vol[i+1][j][k] < umbral) index |= 2;
					if (vol[i+1][j+1][k] < umbral) index |= 4;
					if (vol[i][j+1][k] < umbral) index |= 8;
					if (vol[i][j][k+1] < umbral) index |= 16;
					if (vol[i+1][j][k+1] < umbral) index |= 32;
					if (vol[i+1][j+1][k+1] < umbral) index |= 64;
					if (vol[i][j+1][k+1] < umbral) index |= 128;

					if (index == 0 || index == 255) continue;

					calcular(&grids,i,j,k);


					n = PolygoniseCube(grids,triangles,index);
					for (int l=0;l<n;l++)
						finales.push_back(triangles[l]);
					ntri += n;

					if (ntri >= limita){
						if (dimx>0){
							actual = umbral;
							printf("////////////////////////////////////////////\n");
							printf("Triangulos generados %d\n",ntri);
							printf("////////////////////////////////////////////\n");
						}
						return;
					}

				}
			}

		}

		if (dimx>0){
			printf("////////////////////////////////////////////\n");
			printf("Triangulos generados %d\n",ntri);
			printf("////////////////////////////////////////////\n");
			actual = umbral;
		}
	}

	void readRaw(char* file){

		int dx,dy,dz,c;
		GLubyte *data = ReadRawVolume(file,&dx,&dy,&dz,&c);

		if (data == NULL)
			return;
		if (vol != NULL){

			FOR(i,dimx){
				FOR(j,dimy)
					delete vol[i][j];
				delete vol[i];
			}
			delete vol;
		}
		dimx=dx;dimy=dy;dimz=dz;comp=c;
		dx<<=1; dy<<=1;dz<<=1;
		dimmx = max(max(dx,dy),dz);

		vol = new float**[dimx];
		FOR(i,dimx){
			vol[i] = new float*[dimy];
			FOR(j,dimy)
				vol[i][j] = new float[dimz];
		}


		FOR(z,dimz){
			FOR(y,dimy){
				FOR(x,dimx){
					if (c==1)
					{
						vol[x][y][z]=data[dimx*((z*dimy+y)+1)-(x+1)];
					}else{
						c=dx*((z*dy+y)+1)-((x-1)<<1);
						vol[x][z][y]=(data[c]<<8)+data[c+1];
					}

				}
			}
		}

		delete data;
	}

	void readPvm(char* file){

		size_t dx,dy,dz,c;
		float sx,sy,sz;
		GLubyte *data = readPVMvolume(file,&dx,&dy,&dz,&c,&sx,&sy,&sz);

		if (data == NULL)
			return;
		if (vol != NULL){
			FOR(i,dimx){
				FOR(j,dimy)
					delete vol[i][j];
				delete vol[i];
			}
			delete vol;
		}
		dimx=dx;dimy=dy;dimz=dz;comp=c;
		dx<<=1; dy<<=1;dz<<=1;
		dimmx = max(max(dx,dy),dz);

		vol = new float**[dimx];
		FOR(i,dimx){
			vol[i] = new float*[dimy];
			FOR(j,dimy)
				vol[i][j] = new float[dimz];
		}

		FOR(z,dimz){
			FOR(y,dimy){
				FOR(x,dimx){
					if (c==1)
					{
						vol[x][y][z]=data[dimx*((z*dimy+y)+1)-(x+1)];
					}else{
						c=dx*((z*dy+y)+1)-((x-1)<<1);
						vol[x][z][y]=(data[c]<<8)+data[c+1];
					}

				}
			}
		}

		delete data;
	}

	void centrar(){
		Punto3D x = (Punto3D(0,0,0)+Punto3D(dimx-1,dimy-1,dimz-1))/2;
		Punto3D y = (Punto3D(dimx-1,dimy-1,dimz-1)-Punto3D(0,0,0));
		float d = max(max(y.coord[0],y.coord[1]),y.coord[2]);
		centro = MatScale(2/d,2/d,2/d)*MatTranslate(-x);
	}

	void buffer(){
		int asd=0;
		Triangulo t;
		Vec3D v3;
		Punto3D p3;
		GLintptr datasize=(finales.size()*(18))*sizeof(float);
		if (b==NULL){
			b = new float[finales.size()*18];
			glGenBuffers(1, &BufferObject);
			glBindBuffer(GL_ARRAY_BUFFER, BufferObject);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, datasize, 0, GL_DYNAMIC_COPY_ARB);
		}else{
			delete b;
			b = new float[finales.size()*18];
			glBindBuffer(GL_ARRAY_BUFFER, BufferObject);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, datasize, 0, GL_DYNAMIC_COPY_ARB);
		}

		for (size_t i = 0; i < finales.size(); i++)
		{
			t = finales.at(i);
			for (int k = 0; k < 3; k++)
			{
				//v3=t.p[k]->n;
				memcpy(b+asd,t.p[k]->p.coord,sizeof(t.p[k]->p.coord));
				memcpy(b+asd+3,t.p[k]->n.coord,sizeof(t.p[k]->n.coord));
				asd+=6;
			} 
		}

		glBufferSubDataARB(GL_ARRAY_BUFFER,0, datasize,b);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	GLuint BufferObject;
	vector<Triangulo> finales;
	vector<Vertice*> puntos;
	int dimx,dimy,dimz,size;
	int comp,dimmx;
	Mat4x4 centro;
	float *b;
	float*** vol;
};

Modelo mod;

void init(void)
{   
	glEnable(GL_DEPTH_TEST);

	zoom=1;
	limita = 10000;

	sl.loadShader(VERTEX_SHADER,"files/iluminacion.vert");
	sl.loadShader(FRAGMENT_SHADER,"files/iluminacion.frag");
	sl.create_Link();
	sl.enable();
	sl.AddUniform("m");
	sl.AddUniform("p");
	sl.AddUniform("incolor");

	//luz
	sl.AddUniform("sol.ambiental");
	sl.AddUniform("sol.direccion");
	sl.AddUniform("sol.intensidad");

	sl.disable();
	actual=umbral;

}

void display (void){

	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	if (umbral!=(int)actual){
		mod.mc();
		mod.buffer();
	}

	m=MatTranslate(traslacion[0],traslacion[1],traslacion[2])* (MatScale(zoom,zoom,zoom) * (MatRotar(q_rotate)*mod.centro ));
	m =((MatTranslate(0,0,-2)* m));

	sl.enable();
	glUniformMatrix4fv(sl["m"],1,0,m.mat);
	glUniformMatrix4fv(sl["p"],1,0,p.mat);
	glUniform4fv(sl["incolor"],1,c_fill);

	glUniform3fv(sl["sol.direccion"],1,luz);

	glBindBuffer(GL_ARRAY_BUFFER, mod.BufferObject);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)12);

	if (bools[0]){
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	}else
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	glDrawArrays(GL_TRIANGLES,0,mod.finales.size()*3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	sl.disable();

	TwDraw();
	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape (int width, int height)
{
	float ratio = (float)width / (float)height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	p=buildPerpectiva(45.0,ratio,1.0,100.0);
	TwWindowSize(width, height);
	glutSwapBuffers();
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
	ofn.lpstrFilter = "Raw Files\0*.raw\0Pvm Files\0*.pvm\0\0";//hago que solo muestre archivos del filtro
	ofn.lpstrDefExt = "raw";

	if (GetOpenFileName(&ofn)){

		//printf("%s\n",ofn.lpstrFile+ofn.nFileOffset);// saber el nombre con extencion (nFileExtension solo ext)
		//printf("%d",ofn.nFilterIndex);// saber el filtro usado

		switch (ofn.nFilterIndex)
		{
		case 1:
			mod.readRaw(Path);//funcion que carga el modelo apartir del path
			mod.centrar();
			actual--;
			break;
		case 2:
			mod.readPvm(Path);//funcion que carga el modelo apartir del path
			mod.centrar();
			actual--;
			break;
		default:
			break;
		}

	}	
}

void genMenu(TwBar *bar)
{
	//bloque de la rotacion
	TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &q_rotate, 
		" label='Object rotation' opened=true help='Change the object orientation.' ");
	TwAddVarRW(bar,"luz", TW_TYPE_DIR3F, luz, "label='luz' help='Direccion de la luz.' opened=true");

	//traslacion
	TwAddVarRW(bar, "x", TW_TYPE_FLOAT, &traslacion[0], 
		"label='Mover en x' step=0.08 keyIncr='d' keyDecr='a' help='Permite mover el objeto en el eje x.'");
	TwAddVarRW(bar, "y", TW_TYPE_FLOAT, &traslacion[1], 
		"label='Mover en y' step=0.08 keyIncr='q' keyDecr='e' help='Permite mover el objeto en el eje y.'");
	TwAddVarRW(bar, "z", TW_TYPE_FLOAT, &traslacion[2], 
		"label='Mover en z' step=0.08 keyIncr='w' keyDecr='s' help='Permite mover el objeto en el eje z.'");
	//zoom
	TwAddVarRW(bar, "zoom", TW_TYPE_FLOAT, &zoom, 
		"label='Zoom' min=0.01 max=2.5 step=0.01 help='Escalamiento de la figura.'");
	TwAddVarRW(bar, "umbral", TW_TYPE_FLOAT, &umbral, 
		"label='umbral' min=1 step=1  keyIncr='+' keyDecr='-'");
	TwAddVarRW(bar, "limite", TW_TYPE_UINT32, &limita, 
		"label='Limite de poligonos' min=1 step=1 ");
	TwAddButton(bar,"Boton_1", cargar,NULL,"label='Cargar raw' ");
	TwAddVarRW(bar, "ba", TW_TYPE_BOOLCPP, &bools[0], 
		" label='Activar' help='Activa'");
	/*TwAddVarRW(bar, "c1", TW_TYPE_COLOR4F, &c_line, 
	"label='Color' help='Cambia el color de las lineas.' group='Lineas' ");*/

}

int main(int argc,char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-800)>>1, (glutGet(GLUT_SCREEN_HEIGHT)-600)>>1);
	glutCreateWindow("Visor obj");
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS); 
	glewInit();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
	glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
	glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);
	TwGLUTModifiersFunc(glutGetModifiers);

	init();
	TwInit(TW_OPENGL, NULL);
	TwBar *bar;
	bar = TwNewBar("Barra");
	TwDefine(" GLOBAL help='Aqui se encuentra .' ");
	TwDefine(" Barra label='Barra' refresh=0.5 position='16 16' size='230 450' alpha=0 color='0 0 0'");
	genMenu(bar);

	glutMainLoop();
	TwTerminate();
	return 0;
}

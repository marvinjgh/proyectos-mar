#include <cstdlib>
#include <cstdio>
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

#define ABS(x) (x < 0 ? -(x) : (x))

Shader sl;

GLfloat q_rotate[] = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat traslacion[] = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat c_fill[]={ 0.0f, 0.0f, 1.0f,1.0f };
GLfloat zoom,actual;
bool bools[]={true,false};

Mat4x4 m,p;

int umbral=128;
float*** final;

enum Sombreado { FLAT , GOURAUD };

class Modelo{
public:
	struct Vertice{
		Punto3D p;
		Vec3D n; 
		int count;
	};

	struct Celda{
		Punto3D p[8];
		double val[8];
	};

	struct Triangulo{
		Vertice* p[3];	/* Vertices */
		Vec3D normal;
	};

	void calcular_0_5(GLubyte *data){

		for (int z = 0; z < dimz ; z++){
			for ( int y = 0; y < dimy; y++){
				for ( int x = 0; x < dimx; x++){

					final[x][y][z]=data[((z*dimy)+y)*dimz+x];

				}
			}
		}

	}

	Vertice* buscar_vertice(Punto3D* p){
		int i=0;
		Vertice* aux = NULL;
		for (; i < puntos.size(); i++)
		{
			if (puntos.at(i)->p==*p)
				return puntos[i];
		}

		if (i==puntos.size()){
			aux = new Vertice;
			aux->p=*p;
			aux->count=0;
			puntos.push_back(aux);
		}
		return aux;
	}

	Vertice* VertexInterp(Punto3D* p1,Punto3D* p2,double valp1,double valp2)
	{
		float mu;
		Punto3D *p;

		if (ABS(umbral-valp1) < 0.00001)
			return buscar_vertice(p1);
		if (ABS(umbral-valp2) < 0.00001)
			return buscar_vertice(p2);
		if (ABS(valp1-valp2) < 0.00001)
			return buscar_vertice(p1);

		p=new Punto3D(*p1);
		mu = (float)(umbral - valp1) / (valp2 - valp1);

		*p+= mu * (*p2 - *p1);

		return buscar_vertice(p);
	}

	int PolygoniseCube(Celda g,Triangulo *tri,int index)
	{
		int i,ntri = 0;
		Vertice* vertlist[12];

		/* Find the vertices where the surface intersects the cube */
		if (edgeTable[index] & 1) {
			vertlist[0] = VertexInterp(&g.p[0],&g.p[1],g.val[0],g.val[1]);
		}
		if (edgeTable[index] & 2) {
			vertlist[1] = VertexInterp(&g.p[1],&g.p[2],g.val[1],g.val[2]);
		}
		if (edgeTable[index] & 4) {
			vertlist[2] = VertexInterp(&g.p[2],&g.p[3],g.val[2],g.val[3]);
		}
		if (edgeTable[index] & 8) {
			vertlist[3] = VertexInterp(&g.p[3],&g.p[0],g.val[3],g.val[0]);
		}
		if (edgeTable[index] & 16) {
			vertlist[4] = VertexInterp(&g.p[4],&g.p[5],g.val[4],g.val[5]);
		}
		if (edgeTable[index] & 32) {
			vertlist[5] = VertexInterp(&g.p[5],&g.p[6],g.val[5],g.val[6]);
		}
		if (edgeTable[index] & 64) {
			vertlist[6] = VertexInterp(&g.p[6],&g.p[7],g.val[6],g.val[7]);
		}
		if (edgeTable[index] & 128) {
			vertlist[7] = VertexInterp(&g.p[7],&g.p[4],g.val[7],g.val[4]);
		}
		if (edgeTable[index] & 256) {
			vertlist[8] = VertexInterp(&g.p[0],&g.p[4],g.val[0],g.val[4]);
		}
		if (edgeTable[index] & 512) {
			vertlist[9] = VertexInterp(&g.p[1],&g.p[5],g.val[1],g.val[5]);
		}
		if (edgeTable[index] & 1024) {
			vertlist[10] = VertexInterp(&g.p[2],&g.p[6],g.val[2],g.val[6]);
		}
		if (edgeTable[index] & 2048) {
			vertlist[11] = VertexInterp(&g.p[3],&g.p[7],g.val[3],g.val[7]);
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

	void probar(){
		Celda grid;
		int n=0,ntri=0,index;
		Triangulo triangles[10];

		glDeleteBuffers(1,&BufferObject);
		delete b;
		finales.clear();
		puntos.clear();

		for (int i=0;i<dimx-1;i++) {
			//if (i % (dimx/10) == 0)
			for (int j=0;j<dimy-1;j++) {
				for (int k=0;k<dimz-1;k++) {
					index=0;

					if (final[i][j][k] < umbral) index |= 1;
					if (final[i+1][j][k] < umbral) index |= 2;
					if (final[i+1][j+1][k] < umbral) index |= 4;
					if (final[i][j+1][k] < umbral) index |= 8;
					if (final[i][j][k+1] < umbral) index |= 16;
					if (final[i+1][j][k+1] < umbral) index |= 32;
					if (final[i+1][j+1][k+1] < umbral) index |= 64;
					if (final[i][j+1][k+1] < umbral) index |= 128;

					if (index == 0 || index == 255) continue;

					grid.p[0].coord[0] = (float)i;
					grid.p[0].coord[1] = (float)j;
					grid.p[0].coord[2] = (float)dimz-k;
					grid.val[0] = final[i][j][k];
					grid.p[1].coord[0] = (float)i+1;
					grid.p[1].coord[1] = (float)j;
					grid.p[1].coord[2] = (float)dimz-k; 
					grid.val[1] = final[i+1][j][k];
					grid.p[2].coord[0] = (float)i+1;
					grid.p[2].coord[1] = (float)j+1;
					grid.p[2].coord[2] = (float)dimz-k;
					grid.val[2] = final[i+1][j+1][k];
					grid.p[3].coord[0] = (float)i;
					grid.p[3].coord[1] = (float)j+1;
					grid.p[3].coord[2] = (float)dimz-k;
					grid.val[3] = final[i][j+1][k];
					grid.p[4].coord[0] = (float)i;
					grid.p[4].coord[1] = (float)j;
					grid.p[4].coord[2] = (float)dimz-k+1;
					grid.val[4] = final[i][j][k+1];
					grid.p[5].coord[0] = (float)i+1;
					grid.p[5].coord[1] = (float)j;
					grid.p[5].coord[2] = (float)dimz-k+1;
					grid.val[5] = final[i+1][j][k+1];
					grid.p[6].coord[0] = (float)i+1;
					grid.p[6].coord[1] = (float)j+1;
					grid.p[6].coord[2] = (float)dimz-k+1;
					grid.val[6] = final[i+1][j+1][k+1];
					grid.p[7].coord[0] = (float)i;
					grid.p[7].coord[1] = (float)j+1;
					grid.p[7].coord[2] = (float)dimz-k+1;
					grid.val[7] = final[i][j+1][k+1];
					n = PolygoniseCube(grid,triangles,index);
					for (int l=0;l<n;l++)
						finales.push_back(triangles[l]);
					ntri += n;

				}
			}
		}

		if (dimx>0){
			actual = umbral;
		}
	}

	bool setDims(char* str){
		char keys[] = "1234567890",*ptr;
		size_t i;
		string a = str,name;

		int m=0;

		a = strrchr(str,'\\')+1;

		if (a.empty())
			a = str;

		i = a.find_first_of(keys);
		name = str;
		a = a.substr(i, a.find_first_of(".raw")-i);
		i = a.find_first_of("x");

		ptr=strtok ((char*)a.c_str(),"x");
		if (ptr != NULL){
			dimx = atoi(ptr);
		}else{
			dimx = 0;
			return false;
		}
		if ((ptr = strtok (NULL, "x") )!= NULL){
			dimy = atoi(ptr);
		}else{
			dimy = 0;
			return false;
		}
		if ((ptr = strtok (NULL, "x") )!= NULL){
			dimz = atoi(ptr);
		}else{
			dimz = 0;
			return false;
		}

		m = (dimx>dimy)? dimx : dimy;
		m = (m<dimz)? dimz : m;
		dimmx=m;
		if (m <= 256)
			maximo = 256;
		else
			maximo = 512;

		return true;
	}

	void readRaw(char* file){

		FILE *fp;

		size_t size = dimx * dimy * dimz;   
		GLubyte *data = new GLubyte[size];            // 8bit   

		final = new float**[dimx];
		for (int i = 0; i < dimx; ++i) {
			final[i] = new float*[dimy];
			for (int j = 0; j < dimy; ++j)
				final[i][j] = new float[dimz];
		}

		if (!(fp = fopen(file, "rb")))   
		{   
			printf("Error: opening .raw file failed\n");   
			//exit(1);   
		}   
		else   
			printf("OK: open .raw file successed\n");   

		if (maximo == 256){
			if ( fread(data, sizeof(char), size, fp)!= size)   
			{  
				printf("Error: read .raw file failed\n");   
				//exit(1);   
			}   
			else   
				printf("OK: read .raw file successed\n");   

		}else{
			if ( fread(data, sizeof(short), size, fp)!= size)   
			{  
				printf("Error: read .raw file failed\n");   
				//exit(1);   
			}   
			else   
				printf("OK: read .raw file successed\n");   
		}


		calcular_0_5(data);

		fclose(fp);

		delete[] data;
	}

	void calcular_normales(){
		int i;
		Triangulo t;
		Vertice* v;
		for (i=0;i < finales.size();i++){
			t=finales.at(i);
			Vec3D u(t.p[0]->p,t.p[1]->p);
			Vec3D v(t.p[0]->p,t.p[2]->p);
			t.normal=u^v;
			t.normal.Normalizar();

			t.p[0]->n+=t.normal;
			t.p[0]->count++;
			t.p[1]->n+=t.normal;
			t.p[1]->count++;
			t.p[2]->n+=t.normal;
			t.p[2]->count++;
		}

		for (i=0;i < puntos.size();i++){
			v=puntos.at(i);
			v->n/=v->count;
		}
	}

	void centrar(){
		Punto3D x = (Punto3D(0,0,0)+Punto3D(dimx-1,dimy-1,dimz-1))/2;
		Punto3D y = (Punto3D(dimx-1,dimy-1,dimz-1)-Punto3D(0,0,0));
		float d = max(max(y.coord[0],y.coord[1]),y.coord[2]);
		centro = MatScale(2/d,2/d,2/d)*MatTranslate(-x);
	}

	void buffer(Sombreado x){

		int asd=0;
		Triangulo t;
		Vec3D v3;
		Punto3D p3;
		GLintptr datasize=(finales.size()*(18))*sizeof(float);
		b = new float[finales.size()*18];

		for (int i = 0; i < finales.size(); i++)
		{

			t = finales.at(i);

			for (int k = 0; k < 3; k++)
			{
				v3=(x)? t.p[k]->n:t.normal;

				memcpy(b+asd,t.p[k]->p.coord,sizeof(t.p[k]->p.coord));
				memcpy(b+asd+3,v3.coord,sizeof(v3.coord));

				asd+=6;
			} 
		}

		glGenBuffers(1, &BufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, BufferObject);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, datasize, 0, GL_STREAM_DRAW_ARB);
		glBufferSubDataARB(GL_ARRAY_BUFFER,0, datasize,b);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	GLuint BufferObject;
	vector<Triangulo> finales;
	vector<Vertice*> puntos;
	int dimx,dimy,dimz,size;
	int maximo,dimmx;
	Mat4x4 centro;
	float *b;
};

Modelo mod;
void init(void)
{   
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	zoom=1;

	sl.loadShader(VERTEX_SHADER,"files/iluminacion.vert");
	sl.loadShader(FRAGMENT_SHADER,"files/iluminacion.frag");
	sl.create_Link();
	sl.enable();
	sl.AddUniform("m");
	sl.AddUniform("incolor");
	sl.disable();
}



void display (void){

	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	if (umbral!=(int)actual){
		mod.probar();
		mod.calcular_normales();
		mod.buffer(GOURAUD);
	}

	//glutSolidCube(64);

	m=MatTranslate(traslacion[0],traslacion[1],traslacion[2])* (MatScale(zoom,zoom,zoom) * (MatRotar(q_rotate)*mod.centro ));
	m =( p * (MatTranslate(0,0,-2)* m));

	sl.enable();
	glUniformMatrix4fv(sl["m"],1,0,m.mat);
	glUniform4fv(sl["incolor"],1,c_fill);

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
	//gluPerspective(45.0, (float)width / (float)height,0.5, 200.0);
	// Set the correct perspective.
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
	ofn.lpstrFilter = "Raw Files\0*.raw\0\0";//hago que solo muestre archivos .off
	ofn.lpstrDefExt = "raw";

	if (GetOpenFileName(&ofn)){

		//printf("%s\n",ofn.lpstrFile+ofn.nFileOffset); saber el nombre con extencion (nFileExtension solo ext)

		//printf("%d",ofn.nFilterIndex); saber el filtro usado
		if (mod.setDims(Path)){
			mod.readRaw(Path);//funcion que carga el modelo apartir del path
			mod.centrar();
		}
	}	
}

void genMenu(TwBar *bar)
{
	//bloque de la rotacion
	TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &q_rotate, 
		" label='Object rotation' opened=true help='Change the object orientation.' ");

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
	TwAddVarRW(bar, "umbral", TW_TYPE_INT32, &umbral, 
		"label='umbral' min=1 max=255 step=1  keyIncr='+' keyDecr='-'");
	TwAddButton(bar,"Boton_1", cargar,NULL,"label='Cargar raw' ");

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

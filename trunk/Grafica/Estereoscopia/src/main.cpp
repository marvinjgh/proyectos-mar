#include "mc.h"
#include "AntTweakBar.h"
#include <Commdlg.h>
#include <windows.h>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "estereoscopia.h"



#pragma region variables
Modelo mod;
enum estereoscopia { anaglifo, doble_flujo} este;
float umbral,actual,zoom,off,zero;
glm::vec3 luz(0.f,0.f,-1.f);
glm::fquat q;
glm::vec3 trasla;
float size_point,size_normal,size_line;
GLfloat c_line[]={ 0.0f, 1.0f, 0.0f ,1.0};
GLfloat c_point[]={ 0.0f, 0.0f, 1.0f,1.0 };
GLfloat c_fill[]={ 1.0f, 1.0f, 1.0f ,1.0};
GLfloat c_normal[]={ 1.0f, 0.0f, 0.0f,1.0 };
int asdf,h,w;
Shader s,n;
size_t limite;
GLboolean verde,maya,norma,punto,invertir;
#pragma endregion

//esto lo elimino despues
//GLuint BuffersObject[2];
glm::mat4 Projection,modelview;
//esto lo elimino despues
/*#pragma region cubo
float vertices[8][3]={
{1.f,-1.f,-1.f},
{1.f,-1.f , 1.f},
{-1.f,-1.f,1.f},
{-1.f, -1.f,-1.f},
{1.f, 1.f,-1.f},
{1.f, 1.f,1.f},
{-1.f, 1.f,1.f},
{-1.f, 1.f,-1.f},
};
float normales[8][3]={
{+0.577349,-0.577349,-0.577349	},
{+0.577349,-0.577349,+0.577349	},
{-0.577349,-0.577349,+0.577349	},
{-0.577349,-0.577349,-0.577349	},
{+0.577349,+0.577349,-0.577349	},
{+0.577349,+0.577349,+0.577349	},
{-0.577349,+0.577349,+0.577349	},
{-0.577349,+0.577349,-0.577349	},
};
int triangulos[12][3]={
{ 0, 1, 2},
{ 4, 7, 5},
{ 0, 4, 1},
{ 1, 5, 2},
{ 2, 6, 7},
{ 4, 0, 3},
{ 3, 0, 2},
{ 7, 6, 5},
{ 4, 5, 1},
{ 5, 6, 2},
{ 3, 2, 7},
{ 7, 4, 3}
};
#pragma endregion
*/
void init(){

#pragma region caragar cubo
	/*
	GLintptr datasize=48*sizeof(float);
	glGenBuffers(2, BuffersObject);
	glBindBuffer(GL_ARRAY_BUFFER_ARB, BuffersObject[0]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, datasize, 0, GL_DYNAMIC_COPY_ARB);
	float b[48];int a=0;
	FOR(i,8){
	memcpy(b+a,vertices[i],sizeof(vertices[i]));
	memcpy(b+a+3,normales[i],sizeof(normales[i]));
	a+=6;
	}
	glBufferSubDataARB(GL_ARRAY_BUFFER,0, datasize,b);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, BuffersObject[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(int), &triangulos , GL_DYNAMIC_COPY_ARB);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	*/
#pragma endregion

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);

	trasla=glm::vec3(0.0f);
	size_point=1.f;
	size_normal=1.f;
	size_line=1.5f;
	zoom=1.f;
	limite=1000;
	umbral=1;

	zero = 1.5;
	off=0.2;

	s.loadShader(VERTEX_SHADER,"vertex2.glsl");
	s.loadShader(FRAGMENT_SHADER,"fragment.glsl");
	s.create_Link();
	s.enable();
	s.AddUniform("modelview");
	s.AddUniform("proyect");
	s.AddUniform("normalmatriz");
	s.AddUniform("incolor");
	s.AddUniform("size");
	//luz
	s.AddUniform("direccion");
	s.disable();

	n.loadShader(VERTEX_SHADER,"vertex.glsl");
	n.loadShader(GEOMETRY_SHADER,"geometry.glsl");
	n.loadShader(FRAGMENT_SHADER,"fragment.glsl");
	n.create_Link();
	n.enable();
	n.AddUniform("modelview");
	n.AddUniform("proyect");
	n.AddUniform("normalmatriz");
	n.AddUniform("color");
	//luz
	n.AddUniform("direccion");
	n.disable();

	modelview = glm::translate(glm::mat4(),glm::vec3(0,0,-2));

}

inline void normal(glm::mat4 m, glm::mat4 nm){
	n.enable();
	glUniformMatrix4fv(n["modelview"],1,GL_FALSE,glm::value_ptr(m));
	glUniformMatrix4fv(n["proyect"],1,GL_FALSE,glm::value_ptr(Projection));
	glUniformMatrix4fv(n["normalmatriz"],1,GL_FALSE,glm::value_ptr(nm));
	glUniform4fv(n["incolor"],1,c_normal);
	glUniform3fv(n["direccion"],1,glm::value_ptr(luz));

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mod.BuffersObject[0]);
	glEnableVertexAttribArrayARB(0);
	glEnableVertexAttribArrayARB(1);
	glVertexAttribPointerARB(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, 0);
	glVertexAttribPointerARB(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)12);
	glDrawArrays(GL_LINES,0,mod.puntos.size()+1);//tamaño de los vercies
	glDisableVertexAttribArrayARB(0);
	glDisableVertexAttribArrayARB(1);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
	n.disable();
}

inline void general(glm::mat4 m, glm::mat4 nm,float* color){
	s.enable();
	glUniformMatrix4fv(s["modelview"],1,GL_FALSE,glm::value_ptr(m));
	glUniformMatrix4fv(s["proyect"],1,GL_FALSE,glm::value_ptr(Projection));
	glUniformMatrix4fv(s["normalmatriz"],1,GL_FALSE,glm::value_ptr(nm));
	glUniform1fARB(s["size"],size_point);
	glUniform4fv(s["incolor"],1,color);
	glUniform3fv(s["direccion"],1,glm::value_ptr(luz));


	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mod.BuffersObject[0]);

	glEnableVertexAttribArrayARB(0);
	glEnableVertexAttribArrayARB(1);
	glVertexAttribPointerARB(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, 0);
	glVertexAttribPointerARB(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)12);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod.BuffersObject[1]);
	glDrawElements(
		GL_TRIANGLES,      // mode
		mod.finales.size()*3,    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
		);
	//glDrawArrays(GL_TRIANGLES,0,mod.finales.size()*3);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glDisableVertexAttribArrayARB(0);
	glDisableVertexAttribArrayARB(1);
	s.disable();
}

inline void pinto(glm::mat4 m, glm::mat4 nm){
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glHint (GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
	general(m,nm,c_fill);
	glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
	if (punto)
		general(m,nm,c_point);


	s.enable();

	s.disable();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);

	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	if (maya)
		general(m,nm,c_line);
	if (norma)
		normal(m,nm);

	glDisable(GL_BLEND);
}

void display(void){
	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glLineWidth(size_line);
	float color1[4]={0.0,0.0,0.0,1.0},radio;
	//Model
	glm::mat4 m,m2;
	glm::mat4 nm;
	m = glm::translate(m,trasla);
	m =	glm::scale(m,glm::vec3(zoom));
	m = m*glm::mat4_cast(q);
	m2= m;

	if (!este){
		radio=(float)w/(float)h;
		StereoCamera cam(zero,off,radio,45.0f,0.1,100, -2);
		glViewport(0, 0, (GLsizei) w, (GLsizei) h);
#pragma region pinto
		cam.ApplyLeftFrustum(&Projection,&modelview);
		m = modelview*m*mod.centro;
		nm=glm::transpose(glm::inverse(m));
		glColorMask(true, false, false, false);
		pinto(m,nm);

		glClear(GL_DEPTH_BUFFER_BIT);

		cam.ApplyRightFrustum(&Projection,&modelview);
		m2 = modelview*m2*mod.centro;
		nm=glm::transpose(glm::inverse(m2));
		glColorMask(false, verde, true, false);
		pinto(m2,nm);

#pragma endregion 
		glColorMask(true, true, true, true);
	}else{
		radio=(float)(w>>1)/(float)h;
		glViewport(0, 0, (GLsizei) (w>>1), (GLsizei) h);
		StereoCamera cam(zero,off,radio,45.0f,0.1,100, -2);
		cam.ApplyLeftFrustum(&Projection,&modelview);
		m = modelview*m*mod.centro;
		nm=glm::transpose(glm::inverse(m));
		pinto(m,nm);

		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport((w>>1), 0, (GLsizei) (w>>1), (GLsizei) h);
		cam.ApplyRightFrustum(&Projection,&modelview);
		m2 = modelview*m2*mod.centro;
		nm=glm::transpose(glm::inverse(m2));
		
		pinto(m2,nm);
	}


	TwDraw();
	glutSwapBuffers();

	glutPostRedisplay();
}

void reshape (int width, int height)
{
	//float ratio = (float)width / (float)height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	w=width;h=height;
	//glViewport(0, 0, (GLsizei) width, (GLsizei) height);

	// Set the correct perspective.
	//Projection = glm::perspective(45.0f, ratio, 0.1f, 100.0f);
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
			actual--;
			break;
		case 2:
			mod.readPvm(Path);//funcion que carga el modelo apartir del path
			actual--;
			break;
		default:
			break;
		}

	}	
}

void TW_CALL reload(void *clientData){
	mod.mc(umbral,limite);
	mod.buffer();
	mod.centrar();
}

void genMenu(TwBar *bar){
	//bloque de la rotacion

	TwAddVarRW(bar,"luz", TW_TYPE_DIR3F, glm::value_ptr(luz), "label='Direccion de la luz' help='Direccion de la luz.' opened=false");
	TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &q, 
		" label='Object rotation' opened=true help='Change the object orientation.' group='Transformaciones'");
	//traslacion
	TwAddVarRW(bar, "x", TW_TYPE_FLOAT, &trasla.x, 
		"label='Mover en x' step=0.08 keyIncr='d' keyDecr='a' help='Permite mover el objeto en el eje x.' group='Transformaciones'");
	TwAddVarRW(bar, "y", TW_TYPE_FLOAT, &trasla.y, 
		"label='Mover en y' step=0.08 keyIncr='q' keyDecr='e' help='Permite mover el objeto en el eje y.' group='Transformaciones'");
	TwAddVarRW(bar, "z", TW_TYPE_FLOAT, &trasla.z, 
		"label='Mover en z' step=0.08 keyIncr='w' keyDecr='s' help='Permite mover el objeto en el eje z.' group='Transformaciones'");
	//zoom
	TwAddVarRW(bar, "zoom", TW_TYPE_FLOAT, &zoom, 
		"label='Zoom' min=0.01 max=5.0 step=0.01 help='Escalamiento de la figura.' group='Transformaciones'");

	TwAddVarRW(bar, "c2", TW_TYPE_COLOR3F, c_fill, 
		"label='Relleno' help='Cambia el color del relleno.' group='Color' ");//cargar
	TwAddVarRW(bar, "c1", TW_TYPE_COLOR3F, c_line, 
		"label='Alambrado' help='Cambia el color de las lineas.' group='Color' ");
	TwAddVarRW(bar, "c3", TW_TYPE_COLOR3F, c_normal, 
		"label='Normal' help='Cambia el color de las Normales.' group='Color' ");
	TwAddVarRW(bar, "c4", TW_TYPE_COLOR3F, c_point, 
		"label='Punto' help='Cambia el color de los Puntos.' group='Color' ");
	TwAddVarRW(bar, "m", TW_TYPE_BOOLCPP, &maya,
		" label='Maya' help='Activa dibujar las lineas del objeto.' group='Mostrar' ");
	TwAddVarRW(bar, "p", TW_TYPE_BOOLCPP, &punto,
		" label='Puntos' help='Activa dibujar las lineas del objeto.' group='Mostrar' ");
	TwAddVarRW(bar, "n", TW_TYPE_BOOLCPP, &norma,
		" label='Normal' help='Activa dibujar las lineas del objeto.' group='Mostrar' ");

	TwAddVarRW(bar, "umbral", TW_TYPE_FLOAT, &umbral, 
		"label='umbral' min=1 step=1  group='MC'");
	TwAddVarRW(bar, "limite", TW_TYPE_UINT32, &limite, 
		"label='Limite de poligonos' min=1 step=1 group='MC'");

	TwAddVarRW(bar, "zero", TW_TYPE_FLOAT, &zero, 
		"label='Zero' min=0 step=0.01  group='Estereoscopia'");
	TwAddVarRW(bar, "off", TW_TYPE_FLOAT, &off, 
		"label='Offset' min=0.0001 step=0.01  group='Estereoscopia'");
	TwAddVarRW(bar, "v", TW_TYPE_BOOLCPP, &verde,
		" label='Activar canal verde' group='Estereoscopia' ");

	TwAddVarRW(bar, "asdf", TW_TYPE_FLOAT, &size_point, "label='Tamaño del Vertice' step=0.1 min=1 help='Tamaño del Vertice' ");
	TwAddVarRW(bar, "asdf2", TW_TYPE_FLOAT, &size_line, "label='Tamaño de la linea' step=0.1 min=1 ");

	TwAddButton(bar, "ca", cargar, NULL, 
		" label='Cargar Modelo' key=o help='Carga el archivo raw o pvm que contiene al model.' ");
	TwAddButton(bar, "re", reload, NULL, 
		" label='Generar superfice'");

	TwEnumVal stereo[2] = { { anaglifo , "Anaglifo"}, { doble_flujo, "Doble Flujo"} };
	TwType stereoT = TwDefineEnum("stereoType", stereo, 2);
	TwAddVarRW (bar, "stereoscopia", stereoT, &este, " label='Tecnica' help='Cambia el tipo de estereoscopia.' group='Estereoscopia'");
}

int main(int argc,char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-800)>>1, (glutGet(GLUT_SCREEN_HEIGHT)-600)>>1);
	glutCreateWindow("MC con Estereoscopia");
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
	TwDefine(" Barra label='Barra' refresh=0.5 position='16 16' size='230 550' alpha=0 color='0 0 0'");
	genMenu(bar);

	glutMainLoop();
	TwTerminate();

	return 0;
}
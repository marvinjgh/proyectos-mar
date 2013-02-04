#include "Off.h"
#include <AntTweakBar.h>
#include <Commdlg.h>
#include <windows.h>
#include <glm\gtc\quaternion.hpp>
#include <fstream>

GLuint positionBufferObject,p2,p3;

Shader s;

glm::mat4 modelview;
glm::mat4 Projection;
glm::fquat q;

Off mod;

Sombreado shadow,actual;

glm::vec3 trasla, luz;
glm::vec4 c_line, c_fill;
GLfloat zoom,angulo;
GLuint asdf;

bool bools[]={true,false};

void init(void)
{   
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	trasla=glm::vec3(0.0f);
	luz = glm::vec3(-0.57735f);
	c_line = glm::vec4(1.0f, 0.0f, 0.0f,1.0f);
	c_fill = glm::vec4(0.0f, 0.0f, 1.0f,1.0f);

	zoom=1.0f;
	angulo=5.0;

	s.loadShader(VERTEX_SHADER,"file/color.vert");
	s.loadShader(FRAGMENT_SHADER,"file/color.frag");

	s.create_Link();
	s.enable();
	s.AddUniform("modelview");
	s.AddUniform("proyect");
	s.AddUniform("normalmatriz");
	s.AddUniform("incolor");
	//luz
	s.AddUniform("sol.ambiental");
	s.AddUniform("sol.direccion");
	s.AddUniform("sol.intensidad");

	s.disable();

	modelview = glm::translate(glm::mat4(),glm::vec3(0,0,-5));

}

void display (void){

	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	float color1[4]={0.0,0.0,1.0,1.0};
	//Model
	glm::mat4 m;
	glm::mat4 n;
	m = glm::translate(m,trasla);
	m =	glm::scale(m,glm::vec3(zoom));
	m = m*glm::mat4_cast(q);


	m = modelview*m*mod.centro;
	n=glm::transpose(glm::inverse(m));


	if (mod.isLoad()){
		s.enable();
		glUniformMatrix4fv(s["modelview"],1,GL_FALSE,glm::value_ptr(m));
		glUniformMatrix4fv(s["proyect"],1,GL_FALSE,glm::value_ptr(Projection));
		glUniformMatrix4fv(s["normalmatriz"],1,GL_FALSE,glm::value_ptr(n));


		glUniform4fv(s["incolor"],1,glm::value_ptr(c_fill));

		glUniform3fv(s["sol.direccion"],1,glm::value_ptr(luz));
		glUniform1f(s["sol.intensidad"],0.0f);

#pragma region
		if (actual!=shadow){
			mod.updateBuffer(shadow);
			actual = shadow;
		}
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

		mod.pintar(asdf);
		if (bools[0]){
			glUniform4fv(s["incolor"],1,glm::value_ptr(c_line));
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			mod.pintar(asdf);
		}
#pragma endregion pinto


		s.disable();
	}
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

	// Set the correct perspective.
	Projection = glm::perspective(45.0f, ratio, 0.1f, 100.0f);
	TwWindowSize(width, height);
	glutSwapBuffers();
}

void TW_CALL next(void *clientData){
	mod.simplificar(angulo);
	mod.updateBuffer(shadow);
}

void TW_CALL undo(void *clientData){
	mod.undo();
	mod.updateBuffer(shadow);
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

		mod.cargarModelo(Path);
		mod.updateBuffer(shadow);
		actual=shadow;
		asdf = mod.nTri;
	}

}

void genMenu(TwBar *bar)
{
	//bloque de la rotacion
	TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &q, 
		" label='Object rotation' opened=true help='Change the object orientation.' ");
	TwAddVarRW(bar,"luz", TW_TYPE_DIR3F, glm::value_ptr(luz), "label='luz' help='Direccion de la luz.'");
	TwAddSeparator(bar,"s1","");
	//cargar
	TwAddButton(bar, "ca", cargar, NULL, 
		" label='Cargar Modelo' key=o help='Carga el archivo off que contiene al model.' ");
	TwAddSeparator(bar,"s5","");
	//traslacion
	TwAddVarRW(bar, "x", TW_TYPE_FLOAT, &trasla.x, 
		"label='Mover en x' step=0.08 keyIncr='d' keyDecr='a' help='Permite mover el objeto en el eje x.' group='Traslacion'");
	TwAddVarRW(bar, "y", TW_TYPE_FLOAT, &trasla.y, 
		"label='Mover en y' step=0.08 keyIncr='q' keyDecr='e' help='Permite mover el objeto en el eje y.' group='Traslacion'");
	TwAddVarRW(bar, "z", TW_TYPE_FLOAT, &trasla.z, 
		"label='Mover en z' step=0.08 keyIncr='w' keyDecr='s' help='Permite mover el objeto en el eje z.' group='Traslacion'");
	TwAddSeparator(bar,"s2","");
	//zoom
	TwAddVarRW(bar, "zoom", TW_TYPE_FLOAT, &zoom, 
		"label='Zoom' min=0.01 max=2.5 step=0.01 keyIncr='+' keyDecr='-' help='Escalamiento de la figura.'");
	//bloque linea*/
	TwAddVarRW(bar, "a1", TW_TYPE_BOOLCPP, &bools[0],
		" label='Activar' help='Activa dibujar las lineas del objeto.' key=l group='Line' ");
	TwAddVarRW(bar, "c1", TW_TYPE_COLOR4F, glm::value_ptr(c_line), 
		"label='Color' help='Cambia el color de las lineas.' group='Line' ");
	TwAddVarRW(bar, "c2", TW_TYPE_COLOR3F, glm::value_ptr(c_fill), 
		"label='Color' help='Cambia el color del relleno.' group='Fill' ");
	TwAddSeparator(bar,"s3","");
	TwAddButton(bar, "co", next, NULL, " label='Colapso' key=n help='Colapso' ");
	TwAddButton(bar, "un", undo, NULL, " label='Undo' key=b help='Restaura una de los colapsos' ");
	TwAddVarRW(bar, "ang", TW_TYPE_FLOAT, &angulo, 
		"label='Offset para el colapso' max=45.0 min=5.0 step=1.00 keyIncr='d' keyDecr='a' help='Angulo para la comprobacion'");
	TwAddSeparator(bar,"s4","");

	TwAddVarRW(bar, "asdf", TW_TYPE_UINT32, &asdf, "label='asdf' step=1 min=1 help='asdf' ");


	TwEnumVal sombra[2] = { { FLAT , "Flat"}, { GOURAUD, "Gouraud"} };
	TwType shadowType = TwDefineEnum("shadowType", sombra, 2);
	TwAddVarRW (bar, "sombra", shadowType, &shadow, " label=' Tipo de sombra' keyIncr='<' keyDecr='>' help='Cambia el tipo de sombreado.' ");
}

int main(int argc, char **argv) 
{
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

	TwInit(TW_OPENGL, NULL);
	TwBar *bar;
	bar = TwNewBar("Barra");
	TwDefine(" GLOBAL help='Aqui se encuentra .' ");
	TwDefine(" Barra label='Barra' refresh=0.5 position='16 16' size='230 450' alpha=0 color='0 0 0'");

	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
		printf("Ready for GLSL\n");
	else 
	{
		printf("No GLSL support\n");
		cin.get();
		exit(1);
	}

	init();
	genMenu(bar);
	glutMainLoop();
	TwTerminate();
	return 0;
}
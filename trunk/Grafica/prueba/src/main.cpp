#include "glsl\Shader.h"
#include <AntTweakBar.h>

#include "Modelo.h"

#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\quaternion.hpp>


glm::mat4 modelview;
glm::mat4 Projection;
glm::fquat q;
glm::vec3 trasla;

GLfloat zoom;
Modelo obj;

Shader s;

void init(){

	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	glEnable(GL_DEPTH_TEST);
	s.loadShader(VERTEX_SHADER,"files/color.vert");
	s.loadShader(FRAGMENT_SHADER,"files/color.frag");

	s.create_Link();
	s.enable();
	s.AddUniform("m");
	s.AddUniform("p");
	s.AddUniform("n");
	s.AddUniform("incolor");

	s.disable();

	obj.cargarModelo("files/daedric.obj");

	modelview = glm::translate(glm::mat4(),glm::vec3(0,0,-3));
	zoom=1.0;
	
	trasla=glm::vec3(0.0);

}

void genMenu(TwBar *bar){
	//bloque de la rotacion
	TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &q, 
		" label='Object rotation' opened=true help='Change the object orientation.' ");
	
	//traslacion
	TwAddVarRW(bar, "x", TW_TYPE_FLOAT, &trasla.x, 
		"label='Mover en x' step=0.08 keyIncr='d' keyDecr='a' help='Permite mover el objeto en el eje x.'");
	TwAddVarRW(bar, "y", TW_TYPE_FLOAT, &trasla.y, 
		"label='Mover en y' step=0.08 keyIncr='q' keyDecr='e' help='Permite mover el objeto en el eje y.'");
	TwAddVarRW(bar, "z", TW_TYPE_FLOAT, &trasla.z, 
		"label='Mover en z' step=0.08 keyIncr='w' keyDecr='s' help='Permite mover el objeto en el eje z.'");
	//zoom
	TwAddVarRW(bar, "zoom", TW_TYPE_FLOAT, &zoom, 
		"label='Zoom' min=0.01 max=2.5 step=0.01 keyIncr='+' keyDecr='-' help='Escalamiento de la figura.'");
	/*
	//bloque linea
	TwAddVarRW(bar, "a1", TW_TYPE_BOOLCPP, &bools[0],
		" label='Activar' help='Activa dibujar las lineas del objeto.' key=l group='Lineas' ");
	TwAddVarRW(bar, "c1", TW_TYPE_COLOR4F, &c_line, 
		"label='Color' help='Cambia el color de las lineas.' group='Lineas' ");
	*/
}

void display (void){
	
	
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	
	s.enable();

	glm::mat4 m;
	glm::mat4 n;
	m = glm::translate(m,trasla);
	m =	glm::scale(m,glm::vec3(zoom));
	m = m*glm::mat4_cast(q);
	

	m = modelview*m*obj.centro;
	n=glm::transpose(glm::inverse(m));
	//Model
	//m=MatTranslate(traslacion[0],traslacion[1],traslacion[2])* (MatScale(zoom,zoom,zoom) * (MatRotar(q_rotate)*mod.centro ));
	//m =( p * (MatTranslate(0,0,-3)* m));
	//MVP
	
	glUniformMatrix4fv(s["m"],1,GL_FALSE,glm::value_ptr(m));
	glUniformMatrix4fv(s["p"],1,GL_FALSE,glm::value_ptr(Projection));
	glUniformMatrix4fv(s["n"],1,GL_FALSE,glm::value_ptr(n));
	glBindBuffer(GL_ARRAY_BUFFER, obj.BufferObject);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)*10, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*10, (void*)16);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*10, (void*)28);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float)*10, (void*)36);

	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glDrawArrays(GL_TRIANGLES,0,obj.total*3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	
	
	
	s.disable();
	TwDraw();
	glutSwapBuffers();
	glutPostRedisplay();
	

}

void reshape (int width, int height)
{
	float ratio = (float)width / (float)height;
	float f = 45.0f;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	
	// Set the correct perspective.
	//p=buildPerpectiva(45.0,ratio,1.0,100.0);
	Projection = glm::perspective(45.0f, ratio, 0.1f, 100.0f);
	TwWindowSize(width, height);
	glutSwapBuffers();
}

int main (int argc, char** argv){

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
	TwDefine(" Barra label='Barra' refresh=0.5 position='16 16' size='230 250' alpha=0 color='0 0 0' ");

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
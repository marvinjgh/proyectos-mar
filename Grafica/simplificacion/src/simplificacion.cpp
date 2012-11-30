#include <mod\ModeloOff.h>
#include <AntTweakBar.h>
#include <Commdlg.h>
#include <windows.h>

GLuint positionBufferObject,p2,p3;

Shader t;
Mat4x4 m,p;
ModeloOff mod;
Sombreado shadow;
GLfloat q_rotate[] = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat traslacion[] = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat c_line[]={ 1.0f, 0.0f, 0.0f,1.0f };
GLfloat c_fill[]={ 0.0f, 0.0f, 1.0f,1.0f };
GLfloat zoom,angulo;
bool bools[]={true,false};

void init(void)
{   
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	zoom=1.0f;
	angulo=5.0;
	t.loadShader(VERTEX_SHADER,"color.vert");
	t.loadShader(FRAGMENT_SHADER,"color.frag");
	t.create_Link();
	t.enable();
	t.AddUniform("m");
	t.AddUniform("incolor");
	t.disable();

	//mod.cargarModelo("dragon.off");
	
}

void display (void){

	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	float color1[4]={0.0,0.0,1.0,1.0};
	//Model
	m=MatTranslate(traslacion[0],traslacion[1],traslacion[2])* (MatScale(zoom,zoom,zoom) * (MatRotar(q_rotate)*mod.centro ));
	m =( p * (MatTranslate(0,0,-5)* m));
	//MVP
	t.enable();
	glUniformMatrix4fv(t["m"],1,0,m.mat);
	glUniform4fv(t["incolor"],1,c_fill);
	glBindBuffer(GL_ARRAY_BUFFER, mod.BufferObject);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)12);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	#pragma region
	glDrawArrays(GL_TRIANGLES,0,mod.total*3);
	if (bools[0]){
		glUniform4fv(t["incolor"],1,c_line);
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	}
	glDrawArrays(GL_TRIANGLES,0,mod.total*3);
	#pragma endregion pinto
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	
	t.disable();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
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
	p=buildPerpectiva(45.0,ratio,1.0,100.0);
	TwWindowSize(width, height);
	glutSwapBuffers();
}

void TW_CALL next(void *clientData){
	mod.colapse(angulo);
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
	}
		
}

void genMenu(TwBar *bar)
{
	//bloque de la rotacion
	TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &q_rotate, 
		" label='Object rotation' opened=true help='Change the object orientation.' ");
	TwAddSeparator(bar,"s1","");
	//cargar
	TwAddButton(bar, "ca", cargar, NULL, 
                " label='Cargar Modelo' key=o help='Carga el archivo off que contiene al model.' ");
	TwAddSeparator(bar,"s5","");
	//traslacion
	TwAddVarRW(bar, "x", TW_TYPE_FLOAT, &traslacion[0], 
		"label='Mover en x' step=0.08 keyIncr='d' keyDecr='a' help='Permite mover el objeto en el eje x.' group='Traslacion'");
	TwAddVarRW(bar, "y", TW_TYPE_FLOAT, &traslacion[1], 
		"label='Mover en y' step=0.08 keyIncr='q' keyDecr='e' help='Permite mover el objeto en el eje y.' group='Traslacion'");
	TwAddVarRW(bar, "z", TW_TYPE_FLOAT, &traslacion[2], 
		"label='Mover en z' step=0.08 keyIncr='w' keyDecr='s' help='Permite mover el objeto en el eje z.' group='Traslacion'");
	TwAddSeparator(bar,"s2","");
	//zoom
	TwAddVarRW(bar, "zoom", TW_TYPE_FLOAT, &zoom, 
		"label='Zoom' min=0.01 max=2.5 step=0.01 keyIncr='+' keyDecr='-' help='Escalamiento de la figura.'");
	//bloque linea*/
	TwAddVarRW(bar, "a1", TW_TYPE_BOOLCPP, &bools[0],
		" label='Activar' help='Activa dibujar las lineas del objeto.' key=l group='Line' ");
	TwAddVarRW(bar, "c1", TW_TYPE_COLOR4F, &c_line, 
		"label='Color' help='Cambia el color de las lineas.' group='Line' ");
	TwAddVarRW(bar, "c2", TW_TYPE_COLOR3F, &c_fill, 
		"label='Color' help='Cambia el color del relleno.' group='Fill' ");
	TwAddSeparator(bar,"s3","");
	TwAddButton(bar, "co", next, NULL, " label='Colapso' key=n help='Colapso' ");
	TwAddVarRW(bar, "ang", TW_TYPE_FLOAT, &angulo, 
		"label='Offset para el colapso' max=45.0 min=5.0 step=1.00 keyIncr='d' keyDecr='a' help='Angulo para la comprobacion'");
	TwAddSeparator(bar,"s4","");
	
	
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
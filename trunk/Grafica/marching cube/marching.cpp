
#include <cstdlib>
#include <cstdio>
#include <AntTweakBar.h>
#include <glsl\Shader.h>


GLubyte* data_original;
int dimx,dimy,dimz,size;
Shader sl;

GLfloat q_rotate[] = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat traslacion[] = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat zoom;
bool bools[]={false,false,false,false,false,false,false,false};

GLubyte umbral;

void readRaw(char* file){
	int NX,NY,NZ;

	FILE *fp;
	size = dimx * dimy * dimz;   
	data_original = new GLubyte[size];            // 8bit   

	if (!(fp = fopen(file, "rb")))   
	{   
		printf("Error: opening .raw file failed\n");   
		//exit(1);   
	}   
	else   
		printf("OK: open .raw file successed\n");   


	if ( fread(data_original, sizeof(short), size, fp)!= size)   
	{  
		printf("Error: read .raw file failed\n");   
		//exit(1);   
	}   
	else   
		printf("OK: read .raw file successed\n");   

}

void menu(){

}

void init(void)
{   
	glEnable(GL_DEPTH_TEST);
	/*glEnable(GL_CULL_FACE);


	sl.loadShader(VERTEX_SHADER,"files/t1.vert");
	sl.loadShader(FRAGMENT_SHADER,"files/t1.frag");
	sl.create_Link();
	sl.disable();

	mod.cargarModelo("files/daedric.obj");
	mod.cargarModelo("files/plano.obj");
	*/

}

void display (void){

	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

/*
	m=MatTranslate(traslacion[0],traslacion[1],traslacion[2])* (MatScale(zoom,zoom,zoom) * (MatRotar(q_rotate)*mod.centro ));
	m =( p * (MatTranslate(0,0,-3)* m));

	t.enable();
	glUniformMatrix4fv(t["m"],1,0,m.mat);

	for (size_t i = 0; i < mod.texturas.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, mod.texturas[i]); 	
	}

	glUniform4f(t["tc"],-1,-1,-1,-1);
	glBindBuffer(GL_ARRAY_BUFFER, mod.BufferObject);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)*10, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*10, (void*)16);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*10, (void*)28);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float)*10, (void*)36);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	glDrawArrays(GL_TRIANGLES,0,mod.total*3);
	if (bools[0]){
		glUniform4fv(t["tc"],1,c_line);
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	}
	glDrawArrays(GL_TRIANGLES,0,mod.total*3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	t.disable();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
*/

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
	//p=buildPerpectiva(45.0,ratio,1.0,100.0);
	TwWindowSize(width, height);
	glutSwapBuffers();
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
		"label='Zoom' min=0.01 max=2.5 step=0.01 keyIncr='+' keyDecr='-' help='Escalamiento de la figura.'");
	//bloque linea*/
	TwAddVarRW(bar, "a1", TW_TYPE_BOOLCPP, &bools[0],
		" label='0' group='puntos' ");
	TwAddVarRW(bar, "a2", TW_TYPE_BOOLCPP, &bools[1],
		" label='1' group='puntos' ");
	TwAddVarRW(bar, "a3", TW_TYPE_BOOLCPP, &bools[2],
		" label='2' group='puntos' ");
	TwAddVarRW(bar, "a4", TW_TYPE_BOOLCPP, &bools[3],
		" label='3' group='puntos' ");
	TwAddVarRW(bar, "a5", TW_TYPE_BOOLCPP, &bools[4],
		" label='4' group='puntos' ");
	TwAddVarRW(bar, "a6", TW_TYPE_BOOLCPP, &bools[5],
		" label='5' group='puntos' ");
	TwAddVarRW(bar, "a7", TW_TYPE_BOOLCPP, &bools[6],
		" label='6' group='puntos' ");
	TwAddVarRW(bar, "a8", TW_TYPE_BOOLCPP, &bools[7],
		" label='7' group='puntos' ");
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

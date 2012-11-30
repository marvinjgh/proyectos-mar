
#include <cstdlib>
#include <cstdio>
#include <GL\freeglut.h>
#include <AntTweakBar.h>
#include <glsl\Shader.h>

GLubyte* data_original;
int dimx,dimy,dimz,size;
Shader sl;

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
	TwDefine(" Barra label='Barra' refresh=0.5 position='16 16' size='230 250' alpha=0 color='0 0 0'");

	glutMainLoop();
	TwTerminate();
	return 0;
}

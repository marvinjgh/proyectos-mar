#include "global.h"

int main(int argc,char **argv){
	
	

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	TwInit(TW_OPENGL, NULL);
	init();

	

	
	
	
	glutInitWindowSize(800,600);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-800)>>1, (glutGet(GLUT_SCREEN_HEIGHT)-600)>>1);
	glutCreateWindow("Fractales");
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glewInit();
	
	initShader();
	crear_textura();

	TwBar *bar;
	bar = TwNewBar("Barra");
	TwDefine(" GLOBAL help='Aqui se encuentra .' ");
	TwDefine(" Barra label='Barra' refresh=0.5 position='16 16' size='230 150' alpha=0 color='0 0 0'");
	genMenu(bar);




	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	
	
	
	glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
	glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutPassiveMotionFunc(mouseMove);
	glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
	glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);
	
	
	
	glutMainLoop();
	TwTerminate();

	return 0;
}

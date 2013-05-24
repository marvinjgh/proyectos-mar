#include "paleta.h"
#include <Windows.h>
enum fractal_t
{
	MANDELBROT, JULIA
} fractal;

int which_bn;
float px, py;

Shader man, jul;

int color;

float cmx=0.7f, cmy=0.0f, cjx=-0.7f, cjy=0.27015;
int iter = 70;

float scale = 3.f;

const float zoom_factor = 0.025;


void display(){
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	float t = (float) GetTickCount() / 1000.0;

	glEnable(GL_TEXTURE_1D);
	glBindTexture(GL_TEXTURE_1D,textura);	

	switch (fractal)
	{
	case MANDELBROT:
		man.enable();
		glUniform2fARB(man.getLocation("c"),cmx,cmy);
		glUniform1i(man.getLocation("iter"),iter);
		glUniform1f(man.getLocation("scale"),scale);
		break;
	case JULIA:
		jul.enable();
		glUniform2fARB(jul.getLocation("c"),cjx,cjy);
		glUniform1i(jul.getLocation("iter"),iter);
		glUniform1f(jul.getLocation("scale"),scale);
		break;
	default:
		break;
	}

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(-1, -1);
	glTexCoord2f(1, 0);
	glVertex2f(1, -1);
	glTexCoord2f(1, 1);
	glVertex2f(1, 1);
	glTexCoord2f(0, 1);
	glVertex2f(-1, 1);
	glEnd();

	glDisable(GL_TEXTURE_1D);

	jul.disable();
	man.disable();
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
	//p=buildPerpectiva(45.0,ratio,1.0,100.0);
	TwWindowSize(width, height);
	glutSwapBuffers();
}



void genMenu(TwBar *bar)
{
	//bloque de la rotacion
	TwEnumVal frac[2] = { { MANDELBROT , "Mandelbrot"}, { JULIA, "Julia"} };
	TwType shadowType = TwDefineEnum("fractalType", frac, 2);
	TwAddVarRW (bar, "Fractal", shadowType, &fractal, " label=' Tipo de fractal' keyIncr='<' keyDecr='>' help='Cambia el tipo de fractal.' ");
	//TwAddVarRW(bar, "c0", TW_TYPE_COLOR3F, control.at(0).color, "help='Cambia el color de los puntos.' opened=true");
	//TwAddVarRW(bar, "f", TW_TYPE_FLOAT, &tr, " min=0 max=100 step=0.01 label='T para julia' help='este valor permite probar diferentes t de entrada para la generacion del julia'");
	TwAddVarRW(bar, "it", TW_TYPE_INT32, &iter, " min=0 label='Iteracion' help='a'");
}

void initShader(){
	textura=-1;

	jul.loadShader(FRAGMENT_SHADER,"julia.glsl");
	jul.create_Link();
	jul.enable();
	jul.AddUniform("c");
	jul.AddUniform("iter");
	jul.AddUniform("scale");
	glUniform1i(jul.getLocation("iter"),iter);
	glUniform1f(jul.getLocation("scale"),scale);
	jul.disable();
	man.loadShader(FRAGMENT_SHADER,"mbrot.glsl");
	man.create_Link();
	man.enable();
	man.AddUniform("c");
	man.AddUniform("iter");
	man.AddUniform("scale");
	glUniform1i(man.getLocation("iter"),iter);
	glUniform1f(man.getLocation("scale"),scale);
	man.disable();
}

void mouseMove(int x, int y) {
	if (!TwEventMouseMotionGLUT(x,y)){
	}
}

void keyboard (unsigned char key, int x, int y){

	switch(key) {
	case 27:
	case 'q':
	case 'Q':
		exit(0);
		break;
	case '+':
		if(1) {
			iter += 10;
		} else {
	case '-':
		iter -= 10;
		}
		printf("iterations: %d\n", iter);
		//glUniform1i(jul.getLocation("iter"),iter);
		break;
	}

}

void bn_handler(int bn, int state, int x, int y) {
	if (!TwEventMouseMotionGLUT(x,y)){
		int xres = glutGet(GLUT_WINDOW_WIDTH);
		int yres = glutGet(GLUT_WINDOW_HEIGHT);
		px = 2.0 * ((float)x / (float)xres - 0.5);
		py = 2.0 * ((float)y / (float)yres - 0.5);
		which_bn = bn;

		if(which_bn == 3) {
			scale *= 1 - zoom_factor * 2.0;
		} else if(which_bn == 4) {
			scale *= 1 + zoom_factor * 2.0;;
		}
	}else{
		TwEventMouseButtonGLUT(bn,state,x,y);
	}
}

void mouse_handler(int x, int y) {
	if (!TwEventMouseMotionGLUT(x,y)){
		int xres = glutGet(GLUT_WINDOW_WIDTH);
		int yres = glutGet(GLUT_WINDOW_HEIGHT);
		float fx = 2.0 * ((float)x / (float)xres - 0.5);
		float fy = 2.0 * ((float)y / (float)yres - 0.5);

		if(which_bn == 1) {
			cmx += (fx - px) * scale / 2.0;
			cmy -= (fy - py) * scale / 2.0;
		}

		px = fx;
		py = fy;
	}
}
#include "paleta.h"
#include <Windows.h>
enum fractal_t
{
	MANDELBROT, JULIA
} fractal;



Shader man, jul;

int color;

float cx, cy, tr;
int iter = 60;
int interactive = 0;

void display(){
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	float t = (float) GetTickCount() / 1000.0;
		//cout<<t<<endl;
	t=tr;
	cx =(float) (sin(cos(t / 10) * 10) + cos(t * 2.0) / 4.0 + sin(t * 3.0) / 6.0) * 0.7;
	cy =(float) (cos(sin(t / 10) * 10) + sin(t * 2.0) / 4.0 + cos(t * 3.0) / 6.0) * 0.7;


	glEnable(GL_TEXTURE_1D);
	glBindTexture(GL_TEXTURE_1D,textura);	
	jul.enable();
	glUniform2fARB(jul.getLocation("c"),cx,cy);
	glUniform1i(jul.getLocation("iter"),iter);

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
	TwAddVarRW(bar, "f", TW_TYPE_FLOAT, &tr, " min=0 max=100 step=0.01 label='T para julia' help='este valor permite probar diferentes t de entrada para la generacion del julia'");
	TwAddVarRW(bar, "it", TW_TYPE_INT32, &iter, " min=0 label='Iteracion' help='a'");
}

void init(){

	textura=-1;

	tr=0.0f;
}

void initShader(){

	jul.loadShader(FRAGMENT_SHADER,"julia.glsl");
	jul.create_Link();
	jul.enable();
	jul.AddUniform("c");
	jul.AddUniform("iter");
	glUniform1i(jul.getLocation("iter"),iter);
	jul.disable();
	man.loadShader(FRAGMENT_SHADER,"mbrot.glsl");

}

 void mouseMove(int x, int y) {
	 float deltaAngle;
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

	case ' ':
		interactive = !interactive;
		break;

	case '=':
		if(1) {
			iter += 10;
		} else {
	case '-':
			iter -= 10;
		}
		printf("iterations: %d\n", iter);
		glUniform1i(jul.getLocation("iter"),iter);
		break;
	}

 }

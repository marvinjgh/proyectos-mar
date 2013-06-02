#include "paleta.h"
#include <Windows.h>
enum fractal_t
{
	MANDELBROT, JULIA, SIERPINSKI_T
} fractal;

int which_bn;
float px, py;

Shader man, jul;

float cmx=0.7f, cmy=0.0f, cjx=0.0f, cjy=0.0;
int iter = 70, iter_t = 1;
int xres, yres;
float scale = 3.f;

float zoom_factor = 0.025f;

GLuint dislis;

GLfloat triangle[3][2] = {{-1.0, -1.0}, { 0.0,  1.0}, { 1.0, -1.0}}, color[3]={0.921875f,0.7109375f,0.21875f};

void cuadrado(){
	dislis = glGenLists(1);
	glNewList(dislis, GL_COMPILE);
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
	glEndList();
}

void dividir(GLfloat *a, GLfloat *b, GLfloat *c, int iteraciones) {
	GLfloat v[3][2];
	int j;
	if (iteraciones > 0) {
		//encontrar los puntos medios de cada arista del triángulo
		for (j = 0; j < 2; j++) {
			v[0][j] = (a[j] + b[j]) / 2;
		}
		for (j = 0; j < 2; j++) {
			v[1][j] = (a[j] + c[j]) / 2;
		}
		for (j = 0; j < 2; j++) {
			v[2][j] = (b[j] + c[j]) / 2;
		}

		//por cada triángulo que entra, se crean 4 triángulos más pequeños y recursivamente estos se dividen a su vez
		dividir( a  , v[0], v[1], iteraciones-1);
		dividir(v[0],  b  , v[2], iteraciones-1);
		dividir(v[1], v[2],  c  , iteraciones-1);

	} else {
		//dibujar el triángulo de la iteración 0
		glVertex2fv(a);
		glVertex2fv(b);
		glVertex2fv(c);
	}
}

void display(){
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_1D);
	glBindTexture(GL_TEXTURE_1D,textura);	

	switch (fractal)
	{
	case MANDELBROT:
		man.enable();
		glUniform2fARB(man.getLocation("c"),cmx,cmy);
		glUniform1i(man.getLocation("iter"),iter);
		glUniform1f(man.getLocation("scale"),scale);
		glCallList(dislis);
		break;
	case JULIA:
		jul.enable();
		glUniform2fARB(jul.getLocation("c"),cjx,cjy);
		glUniform1i(jul.getLocation("iter"),iter);
		glUniform1f(jul.getLocation("scale"),scale);
		glCallList(dislis);
		break;
	default:
		glDisable(GL_TEXTURE_1D);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		glTranslated(cjx,cjy,0);
		glScalef(1/scale,1/scale,1);
		glBegin(GL_TRIANGLES);
		glColor3fv(color);
		dividir(triangle[0], triangle[1], triangle[2], iter_t);
		glEnd();
		glLoadIdentity();
		break;
	}

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
	xres=width;
	yres=height;
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
	TwEnumVal frac[3] = { { MANDELBROT , "Mandelbrot"}, { JULIA, "Julia"}, {SIERPINSKI_T, "Triangulo de Sierpinski"} };
	TwType shadowType = TwDefineEnum("fractalType", frac, 3);
	TwAddVarRW (bar, "Fractal", shadowType, &fractal, " label=' Tipo de fractal' keyIncr='<' keyDecr='>' help='Cambia el tipo de fractal.' ");
	TwAddButton(bar,"Boton_1", cargar,NULL,"label='Cargar ppm' ");
	//TwAddVarRW(bar, "f", TW_TYPE_FLOAT, &tr, " min=0 max=100 step=0.01 label='T para julia' help='este valor permite probar diferentes t de entrada para la generacion del julia'");
	TwAddVarRW(bar, "it", TW_TYPE_INT32, &iter, " min=0 label=' Iteraciones' help='iteraciones de julia y mandelbrot'  keyIncr='+' keyDecr='-' ");
	TwAddVarRW(bar, "it2", TW_TYPE_INT32, &iter_t, " min=0 max=12 label=' Iteraciones 2' help='iteraciones del triangulo'  ");
	TwAddVarRW(bar, "c0", TW_TYPE_COLOR3F, color, "label='Color del los triangulos' help='Cambia el color de los puntos.' opened=true");
	
}


void initShader(){
	textura=-1;

	cuadrado();

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

void mouse(int bn, int state, int x, int y) {
	if (!TwEventMouseMotionGLUT(x,y)){
		px = 2.0f * ((float)x / (float)xres - 0.5f);
		py = 2.0f * ((float)y / (float)yres - 0.5f);
		which_bn = bn;

		if(which_bn == 3) {
			scale *= 1.f - zoom_factor * 2.0f;
		} else if(which_bn == 4) {
			scale *= 1.f + zoom_factor * 2.0f;
		}
	}else{
		TwEventMouseButtonGLUT(bn,state,x,y);
	}
}

void mouse_motion(int x, int y) {
	if (!TwEventMouseMotionGLUT(x,y)){
		float fx = 2.0f * ((float)x / (float)xres - 0.5f);
		float fy = 2.0f * ((float)y / (float)yres - 0.5f);

		if(which_bn == 0) {
			cjx += (fx - px) * scale / 2.0f;
			cjy -= (fy - py) * scale / 2.0f;
			cmx += (fx - px) * scale / 2.0f;
			cmy -= (fy - py) * scale / 2.0f;
		}

		px = fx;
		py = fy;
	}
}
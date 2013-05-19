#include <glsl\Shader.h>
#include <AntTweakBar.h>
#include <Windows.h>
enum fractal_t
{
	MANDELBROT, JULIA
} fractal;

Shader man, jul;


float cx, cy;
int iter = 60;
int interactive = 0;

void display(){
	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	if(!interactive) {
		float t = (float) GetTickCount() / 1000.0;
		cx =(float) (sin(cos(t / 10) * 10) + cos(t * 2.0) / 4.0 + sin(t * 3.0) / 6.0) * 0.7;
		cy =(float) (cos(sin(t / 10) * 10) + sin(t * 2.0) / 4.0 + cos(t * 3.0) / 6.0) * 0.7;
	}
		
	jul.enable();
	glUniform2fARB(jul.getLocation("c"),cx,cy);
	
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

void TW_CALL cargar(void *clientData){
	/*
	OPENFILENAME ofn;
	char Path[512]="";
	memset(&ofn, 0, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = Path;
	ofn.nMaxFile = 255;
	ofn.lpstrTitle = "Load Model...";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	ofn.lpstrFilter = "Raw Files\0*.raw\0\0";//hago que solo muestre archivos .off
	ofn.lpstrDefExt = "raw";

	if (GetOpenFileName(&ofn)){

		//printf("%s\n",ofn.lpstrFile+ofn.nFileOffset); saber el nombre con extencion (nFileExtension solo ext)

		//printf("%d",ofn.nFilterIndex); saber el filtro usado
		if (mod.setDims(Path)){
			mod.readRaw(Path);//funcion que carga el modelo apartir del path
			mod.centrar();
		}
	}
	*/
}

void genMenu(TwBar *bar)
{
	//bloque de la rotacion
	TwEnumVal frac[2] = { { MANDELBROT , "Mandelbrot"}, { JULIA, "Julia"} };
	TwType shadowType = TwDefineEnum("fractalType", frac, 2);
	TwAddVarRW (bar, "Fractal", shadowType, &fractal, " label=' Tipo de fractal' keyIncr='<' keyDecr='>' help='Cambia el tipo de fractal.' ");
}

void init(){
	
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
		cout<<x<<" "<<y<<" "<<GetTickCount()/1000<<endl;
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
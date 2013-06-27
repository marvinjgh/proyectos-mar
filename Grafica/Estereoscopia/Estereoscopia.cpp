/*#include "global.h"

int main(int argc, char* argv[])
{
unsigned char *volume;

unsigned int width,height,depth,
components;

float scalex,scaley,scalez;

if (argc!=2 && argc!=3)
{
printf("usage: %s <input.pvm> [<output.raw>]\n",argv[0]);
exit(1);
}

printf("reading PVM file\n");

if ((volume=readPVMvolume(argv[1],&width,&height,&depth,&components,&scalex,&scaley,&scalez))==NULL) exit(1);
if (volume==NULL) exit(1);

printf("found volume with width=%d height=%d depth=%d components=%d\n",
width,height,depth,components);

if (scalex!=1.0f || scaley!=1.0f || scalez!=1.0f)
printf("and edge length %g/%g/%g\n",scalex,scaley,scalez);

printf("and data checksum=%08X\n",checksum(volume,width*height*depth*components));

if (argc>2)
{
writeRAWfile(argv[2],volume,width*height*depth*components,1);

printf("wrote RAW file with size=%d\n",width*height*depth*components);
}

free(volume);

return(0);
}
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	FileName:	MCExample.h and MCExample.cpp
//	Author	:	Michael Y. Polyakov
//	email	:	myp@andrew.cmu.edu	or  mikepolyakov@hotmail.com
//	Website	:	www.angelfire.com/linux/myp
//	Date	:	7/16/2002
//	
//
//	This demonstrates using the Marching Cubes Algorithm (MarchingCubes.h and MarchingCubes.cpp).
//	needed files: mpVector.cpp and mpVector.h (visit my website to get these files)
//	(Note: Uses wxWindows (www.wxwindows.org))
//
//	Controls:	Up/Down		- walks forwards and backwards
//				Left/Right	- strafes to the side
//				Del/End		- strafes up and down
//				Space		- runs Marching Cubes algorithm with minValue
//				Ctrl		- decrements minValue and runs Marching Cubes
//				Insert		- increments minValue and runs Marching Cubes
//				F12			- toggles the fullscreen mode
//				q/w			- decrements/increments number of subdivisions on X-axis
//				a/s			- decrements/increments number of subdivisions on Y-axis
//				z/x			- decrements/increments number of subdivisions on Z-axis
//				e/r			- decreses/increases number of subdivisions on all axis simultaneously
//
//	
//	email me with any suggestions or bugs...
//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gl/freeglut.h>
#include "mc.h"

//camera movement variables
float stepZ, stepX, stepY;		//increment step for moving camera	
float transZ, transX, transY;	//translates camera by this amount on each axis
float angleX, angleY, angleZ;	//angles of rotation
float saX, saY, saZ;			//increments on angles

//minimal value and number of cells on each axis: passed into Marching Cubes
float minValue;
int nX, nY, nZ;
//data points passed to Marching Cubes
mp4Vector * mcPoints;
//data returned by Marching Cubes
TRIANGLE * Triangles;
int numOfTriangles;

//boundary values for Marching Cubes
#define MINX -4.0
#define MAXX 5.0
#define MINY -8.0
#define MAXY 8.0
#define MINZ -8.0
#define MAXZ 8.0

//function which computes a value at point p
float Potential(mpVector p);

//draws coordinates of length s on all axis
void drawCoordinates(float s);

///////////////////////////////////////////////////////////////////////////////////////////////

// Glcanvas ///////////////////////////////////////////////////////////////////////////////////

/*(wxWindow*)parent, -1, wxPoint(0, 0), parent->GetSize()),
	*/
//constructor
void init()
{ 
	//init openGL
	//SetCurrent(); 
	glClearColor(0, 0, 0, 1); 
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	float light_color[] = {1, 1, 1, 1};
	float light_position[] = {5, 2, 7, 1};
	glLightfv(GL_LIGHT0, GL_COLOR, light_color);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	//init material properties
	float ambient_color[] = {0, 1, 1, 1};
	float diff_color[] = {1, 1, 1, 1};
	float spec_color[] = {1, 1, 1, 1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff_color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec_color);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0);
	glColor4fv(ambient_color);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//this->parent = parent;

	stepZ=0.2; stepX=0.2; stepY=0.2; 
	transZ=-8; transX=0; transY=0; 
	angleX=0; angleY=0; angleZ=0;
	saX=5; saY=5; saZ=5;
	nX=20; nY=20; nZ=20;
	minValue=1.8;

	//initialize data to be passed to Marching Cubes
	mcPoints = new mp4Vector[(nX+1)*(nY+1)*(nZ+1)];
	mpVector stepSize((MAXX-MINX)/nX, (MAXY-MINY)/nY, (MAXZ-MINZ)/nZ);
	for(int i=0; i < nX+1; i++)
		for(int j=0; j < nY+1; j++)
			for(int k=0; k < nZ+1; k++) {
				mp4Vector vert(MINX+i*stepSize.x, MINY+j*stepSize.y, MINZ+k*stepSize.z, 0);
				vert.val = Potential((mpVector)vert);
				mcPoints[i*(nY+1)*(nZ+1) + j*(nZ+1) + k] = vert;
			}
			//runs Marching Cubes
			Triangles = //MarchingCubes(nX, nY, nZ, minValue, mcPoints, LinearInterp, numOfTriangles);
			MarchingCubes(nX,nY, nZ,1.f, 1.f, 1.f,
							minValue, mcPoints, numOfTriangles);
}

//re-initializes the data to be used by Marching Cubes
void InitData()
{
	delete [] mcPoints;	//first free the previous allocated memory
	mcPoints = new mp4Vector[(nX+1)*(nY+1)*(nZ+1)];
	mpVector stepSize((MAXX-MINX)/nX, (MAXY-MINY)/nY, (MAXZ-MINZ)/nZ);
	for(int i=0; i < nX+1; i++)
		for(int j=0; j < nY+1; j++)
			for(int k=0; k < nZ+1; k++) {
				mp4Vector vert(MINX+i*stepSize.x, MINY+j*stepSize.y, MINZ+k*stepSize.z, 0);
				vert.val = Potential((mpVector)vert);
				mcPoints[i*(nY+1)*(nZ+1) + j*(nZ+1) + k] = vert;
			}
}

void RunMarchingCubesTest()
{
	delete [] Triangles;	//first free the previous allocated memory
	Triangles = MarchingCubes(nX,nY, nZ,1.f, 1.f, 1.f,
							minValue, mcPoints, numOfTriangles);
}

//moves the camera around
//increments and decrements the minimum value, runs Marching Cubes and switches to full screen mode
void keyboard (unsigned char key, int x, int y){

	switch(key) {
	case GLUT_KEY_UP: transZ += stepZ; break;
	case GLUT_KEY_DOWN:transZ -= stepZ;break;
	case GLUT_KEY_LEFT: transX -= stepX; break;
	case GLUT_KEY_RIGHT: transX += stepX; break;
	case GLUT_KEY_DELETE: transY += stepY; break;
	case GLUT_KEY_END: transY -= stepY; break;
	case 'Q': nX > 1 ? nX--:nX; InitData(); RunMarchingCubesTest(); break;
	case 'W': nX++; InitData(); RunMarchingCubesTest(); break;
	case 'A': nY > 1 ? nY--:nY; InitData(); RunMarchingCubesTest(); break;
	case 'S': nY++; InitData(); RunMarchingCubesTest(); break;
	case 'Z': nZ > 1 ? nZ--:nZ; InitData(); RunMarchingCubesTest(); break;
	case 'X': nZ++; InitData(); RunMarchingCubesTest(); break;
	case 'E': 
		if(nX > 3 && nY > 3 && nZ > 3) {	//make sure non are less or equal to zero
			nX -= 2; nY -= 2; nZ -= 2; 
			InitData(); 
			RunMarchingCubesTest(); 
		} 
		break;
	case 'R': nX += 2; nY += 2; nZ += 2; InitData(); RunMarchingCubesTest(); break;
	default: break;
	}
}

//controls the mouse (camera rotation)
void mouse_motion(int x, int y)
{
	static int xs = x, ys = y;

	angleY += x < xs ? -saY : (x == xs ? 0 : saY);
	angleX += y < ys ? -saX : (y == ys ? 0 : saX);

	xs = x; 
	ys = y;

}

//called by resize events
void reshape (int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float ratio = (float)h/(float)w;
	glFrustum(-1,1, -ratio, ratio, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//does rotation and translation before drawing the triangles returned by Marching Cubes
void display()
{
	//wxPaintDC dc(this);
	//clears the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(transX, transY, transZ);
	glRotatef(angleX, 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);
	glRotatef(angleZ, 0, 0, 1);
	//draws triangles
	glBegin(GL_TRIANGLES);
	for(int i=0; i < numOfTriangles; i++){
		for(int j=0; j < 3; j++){
			glNormal3f(Triangles[i].norm[j].x, Triangles[i].norm[j].y, Triangles[i].norm[j].z);
			glVertex3f(Triangles[i].p[j].x,Triangles[i].p[j].y,Triangles[i].p[j].z);
		}
	}
	glEnd();
	//draws coordinates
	drawCoordinates(6);
	glPopMatrix();

	//tells OpenGL to execute the above commands
	glFlush();
	glutSwapBuffers();
}
// END GLCanvas //////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////


float Potential(mpVector p)
{
	mpVector dp1 = mpVector( 0.0, -2.0,  0.0)-p;
	mpVector dp2 = mpVector( 0.0,  2.0,  0.0)-p;
	mpVector dp3 = mpVector( 2.0,  2.0,  0.0)-p;
	mpVector dp4 = mpVector( 0.0,  0.0,  4.0)-p;
	mpVector dp5 = mpVector(-0.5,  3.1, -1.0)-p;
	mpVector dp6 = mpVector( 0.0,  0.0, -4.0)-p;
	return 1/dp1.Magnitude() + 1/dp2.Magnitude() + 1/dp3.Magnitude() + 1/dp4.Magnitude() + 1/dp5.Magnitude() + 
		1/dp6.Magnitude();
}

void drawCoordinates(float s)
{
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);		//x coordinate
	glVertex3f(s, 0.0, 0.0);

	glVertex3f(0.0, 0.0, 0.0);		//y coordinate
	glVertex3f(0.0, s, 0.0);

	glVertex3f(0.0, 0.0, 0.0);		//z coordinate
	glVertex3f(0.0, 0.0, s);
	glEnd();
}

int main(int argc,char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	//TwInit(TW_OPENGL, NULL);

	glutInitWindowSize(800,600);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-800)>>1, (glutGet(GLUT_SCREEN_HEIGHT)-600)>>1);
	glutCreateWindow("Fractales");
	//glutInitContextVersion(3, 3);
	//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	//glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	/*glewInit();

	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GLEW_ARB_geometry_shader4)
	printf("Ready for GLSL\n");
	else 
	{
	printf("No GLSL support\n");
	cin.get();
	exit(1);
	}

	initShader();
	crear_textura("../files/pal.ppm");

	TwBar *bar;
	bar = TwNewBar("Barra");
	TwDefine(" GLOBAL help='Aqui se encuentra .' ");
	TwDefine(" Barra label='Barra' refresh=0.5 position='16 16' size='230 180' alpha=0 color='0 0 0'");
	genMenu(bar);
	*/

	InitData();
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	//glutMouseFunc(mouse);
	glutMotionFunc(mouse_motion);
	//glutPassiveMotionFunc(mouseMove);
	glutKeyboardFunc(keyboard);
	//glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);



	glutMainLoop();
	//TwTerminate();

	return 0;
}

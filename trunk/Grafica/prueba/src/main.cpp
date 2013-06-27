/* This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 or open http:#www.fsf.org/licensing/licenses/gpl.html*/
 /*

// Modified from the original source code by Peter Hughes,
// Durham University, 2009 with permission.

// Below is a demonstration of stereo viewing, implementing
// the parallel and Toed-in camera setup (parameters of which are 
// obtained by helper class stereoCameraParralle/Toedin.h)

// August 2010, Jimmy Kay, Durham University - Z0915644
// MSc Computer Science, Software Engineering Management
// Acknowledgements: Dr Nick Holliman Durham University



#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <GL/glut.h> 
#include "StereoCameraParallel.h"
#include "StereoCameraToedIn.h"


// Contains all classes, objects and functions of standard C++ lib
using namespace std;

int type = 0;

//Animation
float XRotate = 0;

//Initialising the Colours
GLfloat Red[] = {1.0f, 0.0f, 0.0f, 1.0f}; // variable for colour Red
GLfloat Green[] = {0.0f, 1.0f, 0.0f, 1.0f}; // variable for colour Green
GLfloat White[] = {1.0f, 1.0f, 1.0f, 1.0f}; // variable for colour White

//Type of 3D false so 'None' will be shown at start (void Display)
bool Parallel = false; // all false therefore display will show 'none' until they are true (pressing keyboard to trigger to be true)
bool ToedIn = false; // set not to show false at start in void Display at bottom of coding.
bool Shutter = false;


//StereoCamera initialisation
StereoCameraParallel SCP;
StereoCameraToedIn TI;


//initialisint value ints
int Boxsize = 50, // cube size. can change for different sizes of cube
Separate = 100; // sepration distance

//Initialising arrays. f has 6 values, l has 9 and p has 4.
GLfloat f[6]; //frustum
GLfloat l[9]; //look at
GLfloat p[4]; //perspective


//Standard OpenGL Timer
void MyTimer(int value) {
	
	XRotate+=0.5; 	

	glutPostRedisplay();
	glutTimerFunc(10, MyTimer, 1);
}

// Text Rendering for displaying type of 3D in text
void RenderBitmapString(float x, float y, float z, void *font, char *string) 
{  
	char *c;
	glPushMatrix();
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
	glPopMatrix();
}


// Set light colour to 'white', 'red' and 'green'
// if parallel and toedin are off, display white
void Light(int Side)
{

	if (Parallel == false && ToedIn == false || Side == 2) 
	{ 
		glLightfv( GL_LIGHT0, GL_AMBIENT, White ); 
		glLightfv( GL_LIGHT0, GL_DIFFUSE, White );
		glLightfv( GL_LIGHT0, GL_SPECULAR, White );
	}
	else if (Side == 1) 
	{

		glLightfv( GL_LIGHT0, GL_AMBIENT, Red );
		glLightfv( GL_LIGHT0, GL_DIFFUSE, Red );
		glLightfv( GL_LIGHT0, GL_SPECULAR, Red );
	}
	else // displays green if its not side == 1 
	{
		glLightfv( GL_LIGHT0, GL_AMBIENT, Green );
		glLightfv( GL_LIGHT0, GL_DIFFUSE, Green );
		glLightfv( GL_LIGHT0, GL_SPECULAR, Green );
	}
}

// Renders scene in either GLU_BACK_LEFT or GLU_BACK_RIGHT buffer
void Render (int side)
{
	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	if (ToedIn == true)
	{
		// i is initialised and used to receive the arrays from headerfiles and incremented so all calculated values are in place.
						
		// e.g. int i= 0 and 'i' is lower than 4 (4 values). 'i' increments to 4 to receive all values. 
		// 'p' initilised as perspective, garners the values incremented from Toedin Perspective (4 values).
		// this method also applies for the other values e.g. frustum and lookats.
		for (int i = 0; i < 4; i++)		
		{
			p[i] = TI.Perspective[i];
		}

		if (side == 1) 
		{    //render toedin lookatleft 
			for (int i = 0; i < 9; i++)
			{
				l[i] = TI.LookAtLeft[i];
			}
		} 
		else
		{ //render toedin lookatright
					
			for (int i = 0; i < 9; i++)
			{
				l[i] = TI.LookAtRight[i];
			}
		}
	}
	else 
		if (Parallel == true)
		{
			if (side == 1) 
			{ //If parallel render frustumleft and look at left
				for (int i = 0; i < 6; i++) 
				{
					f[i] = SCP.FrustumLeft[i];
				}
				for (int i = 0; i < 9; i++)
				{
					l[i] = SCP.LookAtLeft[i];
				}
			} 
			else 
				{ 
					//render frustum right and look at right
				for (int i = 0; i < 6; i++)
				{
					f[i] = SCP.FrustumRight[i];
				}
				for (int i = 0; i < 9; i++)
				{
					l[i] = SCP.LookAtRight[i];
				}
			}
		}
		else 
		{ 
			// if neither just render the left eye view so only one side needed (Monoscopic)
			for (int i = 0; i < 6; i++)
			{
				f[i] = SCP.FrustumLeft[i];
			}
			for (int i = 0; i < 9; i++)
			{
				l[i] = SCP.LookAtLeft[i];
			} 
		}
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

	if (ToedIn == true)
	{   // Toed-in = gluPerspective
		gluPerspective(p[0], p[1], p[2], p[3]);
	}
	else
	{
		// or parallel true = gluPerspective
		glFrustum(f[0],f[1],f[2],f[3],f[4],f[5]);
	}

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(l[0], l[1], l[2], l[3], l[4], l[5], l[6], l[7], l[8]); 

		// draw the array of cubes at varying positions across the screen
		// rotating centre cube
		glPushMatrix();
		glTranslatef (0.0f, 0.0f, -900.0f); // x, y, z
		glRotatef(XRotate,0.2, 0.7, 0.3); 
		glutSolidCube(Boxsize);
		glPopMatrix();
		
		glPushMatrix();
		glTranslatef( -Separate, 0.0, -1000); 
		glutSolidCube(Boxsize);
		glPopMatrix(); 

		glPushMatrix();
		glTranslatef( Separate, 0.0, -1000 );
		glutSolidCube( Boxsize );
		glPopMatrix();

		glPushMatrix();
		glTranslatef( -Separate, Separate, -1000 );
		glutSolidCube( Boxsize );
		glPopMatrix();

		glPushMatrix();
		glTranslatef( 0.0, Separate, -1000 );
		glutSolidCube( Boxsize );
		glPopMatrix();

		glPushMatrix();
		glTranslatef( Separate, Separate, -1000 );
		glutSolidCube( Boxsize );
		glPopMatrix();

		glPushMatrix();
		glTranslatef( -Separate, -Separate, -1000 );
		glutSolidCube( Boxsize );
		glPopMatrix();

		glPushMatrix();
		glTranslatef( 0.0, -Separate, -1000 );
		glutSolidCube( Boxsize );
		glPopMatrix();

		glPushMatrix();
		glTranslatef( Separate, -Separate, -1000 );
		glutSolidCube( Boxsize );
		glPopMatrix();

		glPushMatrix();
		glTranslatef( Separate*2, Separate, -1000 );
		glutSolidCube( Boxsize );
		glPopMatrix();

		glPushMatrix();
		glTranslatef( Separate*2, 0.0, -1000 );
		glutSolidCube( Boxsize );
		glPopMatrix();

		glPushMatrix();
		glTranslatef( Separate*2, -Separate, -1000 );
		glutSolidCube( Boxsize );
		glPopMatrix();

		glPushMatrix();
		glTranslatef( -Separate*2, Separate, -1000 );
		glutSolidCube( Boxsize );
		glPopMatrix();

		glPushMatrix();
		glTranslatef( -Separate*2, 0.0, -1000 );
		glutSolidCube( Boxsize );
		glPopMatrix();

		glPushMatrix();
		glTranslatef( -Separate*2, -Separate, -1000 );
		glutSolidCube( Boxsize );
		glPopMatrix();
		
}
	// Glut display function
void display()
{ 

	// Render (0) = Render 'GL BACK RIGHT'
	// Render (1) = Render 'GL BACK LEFT'
	// Light (0) = Green (from above, void light, anything other than 1 and 2 is green) 
	// Light (1) = Red (from above, void light, side==1 is red)
	// Light (2) = White 
	
	glDrawBuffer (GL_BACK_LEFT);
	
	glDrawBuffer( GL_BACK_RIGHT );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
if (Shutter == true) // requires Quad buffered hardware
{
	// If Shutter chosen, display the following

	Light(2);
	glDrawBuffer (GL_BACK_LEFT);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	Render(1);
	glDrawBuffer( GL_BACK_RIGHT );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	Render(0);
}

else
{

		
if (Parallel == true || ToedIn == true) 
{
	// If Parallel or Toedin (Anaglyph) is chosen, display the following

	//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawBuffer(GL_BACK_LEFT);
	glColorMask( true, false, false, false);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	Light(2); 
	Render(0); 
	glClear( GL_DEPTH_BUFFER_BIT) ;
	glDrawBuffer( GL_BACK_RIGHT );
	glColorMask( false, true, true, false);
	Render(1);
	glClear( GL_DEPTH_BUFFER_BIT) ;


} 
else 
{ 
		
	// If none, render monoscopic(left eye only)
	glColorMask( true, true, true, true);
	glDrawBuffer(GL_BACK_LEFT);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Light(2);
	Render(1);  
}
	
}
	
	// Text showing what you are viewing on bottom right of the window.
	// Variables are defined above (X,Y,Z,Font,String)

	Light(2);  // set text colour to white
	
if (Parallel == true)
{ 
	RenderBitmapString(180, -150, -1000, GLUT_BITMAP_TIMES_ROMAN_24 ,"Parallel");
} 
else if (ToedIn == true)
{
	RenderBitmapString(180, -150, -1000, GLUT_BITMAP_TIMES_ROMAN_24 ,"ToedIn");
}
else
{
	RenderBitmapString(180, -150, -1000, GLUT_BITMAP_TIMES_ROMAN_24 ,"None");
}

if (Shutter == true)
{ 
	RenderBitmapString(110, -150, -1000, GLUT_BITMAP_TIMES_ROMAN_24 ,"Shutter");
} 
else if (Parallel != false || ToedIn != false)
{
	RenderBitmapString(110, -150, -1000, GLUT_BITMAP_TIMES_ROMAN_24 ,"Anaglyph");
}

	glutSwapBuffers();
	glutPostRedisplay();
}


// Keyboard input keys
void keyboard (unsigned char key, int x, int y)
{
switch (key)

{

case 't':			
		
	ToedIn = !ToedIn;
	Parallel = false;

	glutPostRedisplay();

break;

case 'p':			
		
	Parallel = !Parallel;
	ToedIn = false;

	glutPostRedisplay();

break;
	
}
}


// OpenGL Functions setting light, colour, texture etc
void Init()

{	
	//Clear screen of all colours
	glClearColor(0,0,0,0);

    //Depth Test
	glEnable(GL_DEPTH_TEST);

	//Shade model
	glShadeModel(GL_SMOOTH);

	//Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); 
	glLightModeli (GL_LIGHT_MODEL_TWO_SIDE, 0);

	//Specify different types of light
	GLfloat Light0Pos[] = {4.0f, 4.0f, 4.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, Light0Pos );
	
	GLfloat Light0Amb[] = {0.8f, 0.8f, 0.8f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, Light0Amb ); 

	GLfloat Light0Diff[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light0Diff ); 

	GLfloat Light0Spec[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_SPECULAR, Light0Spec ); 

	//Set Materials
	GLfloat MaterialAmb[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT , MaterialAmb);

	GLfloat MaterialDif[] = {0.7f, 0.7f, 0.7f, 1.0f};
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE , MaterialDif);

	GLfloat MaterialSpec[] = {0.5f, 0.5f, 0.5f, 1.0f};
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR , MaterialSpec);

	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS , 50.0f);

	SCP.Update();
	TI.Update();

}

// Main Function
int main ( int argc, char * argv[] ) 

{
	//cout = standard output stream
    cout << "Choose the Display type - press ENTER after number" << endl;
	cout << "1 for SHUTTER" << endl;
	cout << "2 for ANAGLYPH" << endl;
	// cin = standard input stream
	cin >> type; 

    glutInit(&argc,argv);
    glutInitWindowSize(800,600); 
	
	// if type (the input) is 1, display Shutter display mode
if (type == 1)

{
	Shutter = true;
	glutInitDisplayMode( GLUT_STEREO | GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
} 

else
	// if not then 1 then display anaglyph display mode
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH |GLUT_STEREO );
}
    
    glutCreateWindow("C++ Binocularity"); 
	Init();
	glutKeyboardFunc(keyboard); // enable keybaord functions
    glutDisplayFunc(display); 
	glutTimerFunc(10, MyTimer, 1); 
    glutMainLoop();
}
*/

	#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <GL/glut.h>
#include "anaglyph.h"

/*
   Anaglyphs implemented in OpenGL without stereo buffer support
   Adds the two images in the accumulation buffer
   Note
   1. All objects must be drawn as greyscale!
   2. This is written for illustrative purposes...not efficiency!
   3. Example of correct stereo projections
*/

/* Misc globals */
int debug = FALSE;
int fullscreen = FALSE;
int currentbutton = -1;
float rotatespeed = 0.5;     /* Each object can autorotate      */
double dtheta = 1.0;         /* Camera rotation angle increment */
CAMERA camera;
XYZ origin = {0.0,0.0,0.0};

/* Image saving options */
int windowdump = FALSE;
int movierecord = FALSE;

/* Model types */
#define MESH     1
#define BOX      2
#define SPHERE   3
#define PULSAR   4
#define KNOT     5
#define TRITORUS 6
#define LORENZ   7
int modeltype = MESH;

/* Glasses filter types */
#define REDBLUE  1
#define REDGREEN 2
#define REDCYAN  3
#define BLUERED  4
#define GREENRED 5
#define CYANRED  6
int glassestype = REDBLUE;

int main(int argc,char **argv)
{
   int i;
   int mainmenu,modelmenu,speedmenu,cameramenu,glassesmenu;

   /* Parse the command line arguments */
   for (i=1;i<argc;i++) {
      if (strcmp(argv[i],"-h") == 0) 
         GiveUsage(argv[0]);
      if (strcmp(argv[i],"-d") == 0)
         debug = TRUE;
      if (strcmp(argv[i],"-f") == 0)
         fullscreen = TRUE;
   }

   /* Set things (glut) up */
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_ACCUM | GLUT_RGB | GLUT_DEPTH);

   /* Create the window and handlers */
   glutCreateWindow("Anaglyph simulator");
   camera.screenwidth = 400;
   camera.screenheight = 300;
   glutReshapeWindow(camera.screenwidth,camera.screenheight);
   if (fullscreen)
      glutFullScreen();
   glutDisplayFunc(HandleDisplay);
   glutReshapeFunc(HandleReshape);
   glutVisibilityFunc(HandleVisibility);
   glutKeyboardFunc(HandleKeyboard);
   glutSpecialFunc(HandleSpecialKeyboard);
   glutMouseFunc(HandleMouse);
   glutMotionFunc(HandleMouseMotion);
   CreateEnvironment();
   CameraHome(0);

   /* 
      Leaving the mouse on is often a nice way of determining
      the depth of various parts of the model, the mouse will
      appear to have 0 parallex and therefore in the focal plane. 
      Object shold appear infront of or behind the depth of the
      mouse depending on whether thay have negative or positive
      parallax.
   */
   /* glutSetCursor(GLUT_CURSOR_NONE); */

   /* Set up the model menu */
   modelmenu = glutCreateMenu(HandleModelMenu);
   glutAddMenuEntry("Mesh",MESH);
   glutAddMenuEntry("Box",BOX);
   glutAddMenuEntry("Sphere",SPHERE);
   glutAddMenuEntry("Pulsar model",PULSAR);
   glutAddMenuEntry("Knot",KNOT);
   glutAddMenuEntry("TriTorus surface",TRITORUS);
   glutAddMenuEntry("Lorenz Attractor",LORENZ);

   /* Set up the glasses menu */
   glassesmenu = glutCreateMenu(HandleGlassesMenu);
   glutAddMenuEntry("Red-Blue",REDBLUE);
   glutAddMenuEntry("Red-Green",REDGREEN);
   glutAddMenuEntry("Red-Cyan",REDCYAN);
   glutAddMenuEntry("Blue-Red",BLUERED);
   glutAddMenuEntry("Green-Red",GREENRED);
   glutAddMenuEntry("Cyan-Red",CYANRED);

   /* Set up the rotate speed menu */
   speedmenu = glutCreateMenu(HandleSpeedMenu);
   glutAddMenuEntry("Stop",1);
   glutAddMenuEntry("Slow",2);
   glutAddMenuEntry("Medium",5);
   glutAddMenuEntry("Fast",9);

   /* Set up the camera menu */
   cameramenu = glutCreateMenu(HandleCameraMenu);
   glutAddMenuEntry("In front of screen",1);
   glutAddMenuEntry("Center",2);
   glutAddMenuEntry("Behind screen",3);
   glutAddMenuEntry("No Parallax",4);

   /* Set up the main menu */
   mainmenu = glutCreateMenu(HandleMainMenu);
   glutAddSubMenu("Model type",modelmenu);
   glutAddSubMenu("Rotate Speed",speedmenu);
   glutAddSubMenu("Focal point",cameramenu);
   glutAddSubMenu("Filter colour",glassesmenu);
   glutAddMenuEntry("Quit",9);
   glutAttachMenu(GLUT_RIGHT_BUTTON);

   /* Ready to go! */
   glutMainLoop();
   return(0);
}

/*
   This is where global OpenGL/GLUT settings are made, 
   that is, things that will not change in time 
*/
void CreateEnvironment(void)
{
   int num[2];

   glEnable(GL_DEPTH_TEST);
   glDisable(GL_LINE_SMOOTH);
   glDisable(GL_POINT_SMOOTH);
   glDisable(GL_POLYGON_SMOOTH); 
   glDisable(GL_DITHER);
   glDisable(GL_CULL_FACE);
   glEnable(GL_BLEND);    /* Not necessary but for bug in PS350 driver */
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glLineWidth(1.0);
   glPointSize(1.0);
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   glFrontFace(GL_CW);
   glClearColor(0.0,0.0,0.0,0.0);
   glClearAccum(0.0,0.0,0.0,0.0);   /* The default */
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);
   glPixelStorei(GL_UNPACK_ALIGNMENT,1);

   /* 
      In case you want to check what the colour depth of your
      accumulation buffer is, hopefully it is 16 bits
   */
   if (debug) {
      glGetIntegerv(GL_ACCUM_RED_BITS,num);
      fprintf(stderr,"Red bits: %d\n",num[0]);
      glGetIntegerv(GL_ACCUM_GREEN_BITS,num);
      fprintf(stderr,"Green bits: %d\n",num[0]);
      glGetIntegerv(GL_ACCUM_BLUE_BITS,num);
      fprintf(stderr,"Blue bits: %d\n",num[0]);
      glGetIntegerv(GL_ACCUM_ALPHA_BITS,num);
      fprintf(stderr,"Alpha bits: %d\n",num[0]);
   }
}

/*
   This is the basic display callback routine
   It creates the geometry, lighting, and viewing position
*/
void HandleDisplay(void)
{
   int i,j;
   XYZ r;
   double dist,ratio,radians,scale,wd2,ndfl;
   double left,right,top,bottom,nearp=0.1,farp=10000;

   /* Clip to avoid extreme stereo */
   nearp = camera.focallength / 5;

   /* Derive the the "right" vector */
   CROSSPROD(camera.vd,camera.vu,r);
   Normalise(&r);
   r.x *= camera.eyesep / 2.0;
   r.y *= camera.eyesep / 2.0;
   r.z *= camera.eyesep / 2.0;

   /* Misc stuff */
   ratio  = camera.screenwidth / (double)camera.screenheight;
   radians = DTOR * camera.aperture / 2;
   wd2     = nearp * tan(radians);
   ndfl    = nearp / camera.focallength;

   /* Set the buffer for writing and reading */
   glDrawBuffer(GL_BACK);
   glReadBuffer(GL_BACK);

   /* Clear things */
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClear(GL_ACCUM_BUFFER_BIT); /* Not strictly necessary */

   glViewport(0,0,camera.screenwidth,camera.screenheight);

   /* Left eye filter */
   glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
   switch (glassestype) {
   case REDBLUE:
   case REDGREEN:
   case REDCYAN:
      glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_TRUE);
      break;
   case BLUERED:
      glColorMask(GL_FALSE,GL_FALSE,GL_TRUE,GL_TRUE);
      break;
   case GREENRED:
      glColorMask(GL_FALSE,GL_TRUE,GL_FALSE,GL_TRUE);
      break;
   case CYANRED:
      glColorMask(GL_FALSE,GL_TRUE,GL_TRUE,GL_TRUE);
      break;
   } 

   /* Create the projection */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   left  = - ratio * wd2 + 0.5 * camera.eyesep * ndfl;
   right =   ratio * wd2 + 0.5 * camera.eyesep * ndfl;
   top    =   wd2;
   bottom = - wd2;
   glFrustum(left,right,bottom,top,nearp,farp);

   /* Create the model */
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(camera.vp.x - r.x,camera.vp.y - r.y,camera.vp.z - r.z,
             camera.vp.x - r.x + camera.vd.x,
             camera.vp.y - r.y + camera.vd.y,
             camera.vp.z - r.z + camera.vd.z,
             camera.vu.x,camera.vu.y,camera.vu.z);
   CreateWorld();
   glFlush();
   glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE); 

   /* Write over the accumulation buffer */
   glAccum(GL_LOAD,1.0); /* Could also use glAccum(GL_ACCUM,1.0); */

   glDrawBuffer(GL_BACK);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   /* The projection */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   left  = - ratio * wd2 - 0.5 * camera.eyesep * ndfl;
   right =   ratio * wd2 - 0.5 * camera.eyesep * ndfl;
   top    =   wd2;
   bottom = - wd2;
   glFrustum(left,right,bottom,top,nearp,farp);

   /* Right eye filter */
   glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
   switch (glassestype) {
   case REDBLUE:
      glColorMask(GL_FALSE,GL_FALSE,GL_TRUE,GL_TRUE);
      break;
   case REDGREEN:
      glColorMask(GL_FALSE,GL_TRUE,GL_FALSE,GL_TRUE);
      break;
   case REDCYAN:
      glColorMask(GL_FALSE,GL_TRUE,GL_TRUE,GL_TRUE);
      break;
   case BLUERED:
   case GREENRED:
   case CYANRED:
      glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_TRUE);
      break;
   } 

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(camera.vp.x + r.x,camera.vp.y + r.y,camera.vp.z + r.z,
             camera.vp.x + r.x + camera.vd.x,
             camera.vp.y + r.y + camera.vd.y,
             camera.vp.z + r.z + camera.vd.z,
             camera.vu.x,camera.vu.y,camera.vu.z);
   CreateWorld();
   glFlush();
   glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE); 

   /* Addin the new image and copy the result back */
   glAccum(GL_ACCUM,1.0);
   glAccum(GL_RETURN,1.0);
   
   /* Optionally dump image */
   if (windowdump || movierecord) {
      WindowDump(camera.screenwidth,camera.screenheight);
      windowdump = FALSE;
   }

   /* Let's look at it */
   glutSwapBuffers(); 
}

/*
   Create one of the possible models
   Handle the rotation of the model, about the y axis
*/
void CreateWorld(void)
{
   static double rotateangle = 0.0;

   glPushMatrix();
   glRotatef(rotateangle,0.0,1.0,0.0);
   switch (modeltype) {
   case MESH:     MakeMesh();     break;
   case BOX:      MakeBox();      break;
   case SPHERE:   MakeSphere();   break;
   case KNOT:     MakeKnot();     break;
   case PULSAR:   MakePulsar();   break;
   case TRITORUS: MakeTriTorus(); break;
   case LORENZ:   MakeLorenz();   break;
   }
   glPopMatrix();

   rotateangle += rotatespeed;
   MakeLighting();
}

/*
   Create the geometry for a wireframe box
*/
void MakeBox(void)
{
   XYZ pmin = {-3,-3,-3},pmax = {3,3,3};

   glColor3f(1.0,1.0,1.0);
   glBegin(GL_LINE_STRIP);
   glVertex3f(pmin.x,pmin.y,pmin.z);
   glVertex3f(pmax.x,pmin.y,pmin.z);
   glVertex3f(pmax.x,pmin.y,pmax.z);
   glVertex3f(pmin.x,pmin.y,pmax.z);
   glVertex3f(pmin.x,pmin.y,pmin.z);
   glVertex3f(pmin.x,pmax.y,pmin.z);
   glVertex3f(pmax.x,pmax.y,pmin.z);
   glVertex3f(pmax.x,pmax.y,pmax.z);
   glVertex3f(pmin.x,pmax.y,pmax.z);
   glVertex3f(pmin.x,pmax.y,pmin.z);
   glEnd();
   glBegin(GL_LINES);
   glVertex3f(pmax.x,pmin.y,pmin.z); glVertex3f(pmax.x,pmax.y,pmin.z);
   glEnd();
   glBegin(GL_LINES);
   glVertex3f(pmax.x,pmin.y,pmax.z); glVertex3f(pmax.x,pmax.y,pmax.z);
   glEnd();
   glBegin(GL_LINES);
   glVertex3f(pmin.x,pmin.y,pmax.z); glVertex3f(pmin.x,pmax.y,pmax.z);
   glEnd();
}

/*
   Create the geometry for a sphere
*/
void MakeSphere(void)
{
   int i,j,n=32;
   double t1,t2,t3,r=4;
   XYZ e,p,c={0,0,0};

   glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
   glColor3f(1.0,1.0,1.0);
   for (j=0;j<n/2;j++) {
      t1 = -PID2 + j * PI / (n/2);
      t2 = -PID2 + (j + 1) * PI / (n/2);

      glBegin(GL_QUAD_STRIP);
      for (i=0;i<=n;i++) {
         t3 = i * TWOPI / n;

         e.x = cos(t1) * cos(t3);
         e.y = sin(t1);
         e.z = cos(t1) * sin(t3);
         p.x = c.x + r * e.x;
         p.y = c.y + r * e.y;
         p.z = c.z + r * e.z;
         glNormal3f(e.x,e.y,e.z);
         glTexCoord2f(i/(double)n,2*j/(double)n);
         glVertex3f(p.x,p.y,p.z);

         e.x = cos(t2) * cos(t3);
         e.y = sin(t2);
         e.z = cos(t2) * sin(t3);
         p.x = c.x + r * e.x;
         p.y = c.y + r * e.y;
         p.z = c.z + r * e.z;

         glNormal3f(e.x,e.y,e.z);
         glTexCoord2f(i/(double)n,2*(j+1)/(double)n);
         glVertex3f(p.x,p.y,p.z);

      }
      glEnd();
   }
}

/*
   Create the geometry for the knot
*/
void MakeKnot(void)
{
   int i,n=200;
   double x,y,z;
   double mu;

   glLineWidth(2.0);
   glColor3f(1.0,1.0,1.0);

   glBegin(GL_LINE_STRIP);
   for (i=0;i<=n;i++) {
      mu = i * TWOPI / (double)n;
      x = 10 * (cos(mu) + cos(3*mu)) + cos(2*mu) + cos(4*mu);
      y = 6 * sin(mu) + 10 * sin(3*mu);
      z = 4 * sin(3*mu) * sin(5*mu/2) + 4*sin(4*mu) - 2 * sin(6*mu);
      glVertex3f(x/4,y/4,z/4);
   }
   glEnd();

   glLineWidth(1.0);
}

/*
   Create the geometry for the lorenz attractor
*/
void MakeLorenz(void)
{
   int i,n=10000;
   double x0=0.1,y0=0,z0=0,x1,y1,z1;
   double h = 0.005;
   double a = 10.0;
   double b = 28.0;
   double c = 8.0 / 3.0;

   glColor3f(1.0,1.0,1.0);
   glBegin(GL_POINTS);
   for (i=0;i<=n;i++) {
      x1 = x0 + h * a * (y0 - x0);
      y1 = y0 + h * (x0 * (b - z0) - y0);
      z1 = z0 + h * (x0 * y0 - c * z0);
      x0 = x1;
      y0 = y1;
      z0 = z1;
      if (i > 100)
         glVertex3f((x0-0.95)/5,(y0-1.78)/5,(z0-26.7)/5);
   }
   glEnd();
}

/*
   Create the geometry for a tritorus
*/
void MakeTriTorus(void)
{
   int m = 51;
   int i,j,k;
   double u,v,u1,v1,delta=0.001;
   XYZ p[4],n[4],p1,p2;

   glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
   glColor3f(1.0,1.0,1.0);

   glBegin(GL_QUADS);
   for (i=0;i<m;i++) {
      for (j=0;j<m;j++) {

         u  = -PI + i * TWOPI / m;
         u1 = -PI + (i+1) * TWOPI / m;
         v  = -PI + j * TWOPI / m;
         v1 = -PI + (j+1) * TWOPI / m;

         p[0] = TriTorusEval(u,v);
         p1 = TriTorusEval(u+delta,v);
         p2 = TriTorusEval(u,v+delta);
         n[0] = CalcNormal(p[0],p1,p2);

         p[1] = TriTorusEval(u1,v);
         p1 = TriTorusEval(u1+delta,v);
         p2 = TriTorusEval(u1,v+delta);
         n[1] = CalcNormal(p[1],p1,p2);

         p[2] = TriTorusEval(u1,v1);
         p1 = TriTorusEval(u1+delta,v1);
         p2 = TriTorusEval(u1,v1+delta);
         n[2] = CalcNormal(p[2],p1,p2);

         p[3] = TriTorusEval(u,v1);
         p1 = TriTorusEval(u+delta,v1);
         p2 = TriTorusEval(u,v1+delta);
         n[3] = CalcNormal(p[3],p1,p2);

         for (k=0;k<4;k++) {
            glNormal3f(n[k].x,n[k].y,n[k].z);
            glVertex3f(2.5*p[k].x,2.5*p[k].y,2.5*p[k].z);
         }
      }
   }
   glEnd();
}
XYZ TriTorusEval(double u,double v)
{
   XYZ p;

   p.x = sin(u) * (1 + cos(v));
   p.y = sin(u + 2 * PI / 3) * (1 + cos(v + 2 * PI / 3));
   p.z = sin(u + 4 * PI / 3) * (1 + cos(v + 4 * PI / 3));

   return(p);
}

/*
   Create the geometry for the mesh
*/
void MakeMesh(void)
{
   int i,j,n=1,w=4;
   static double rotateangle = 0.0;

   glColor3f(1.0,1.0,1.0);
   glBegin(GL_LINES);
   for (i=-w;i<=w;i+=n) {
      for (j=-w;j<w;j+=n) {
         glVertex3f(0.0,(double)i,(double)j);
         glVertex3f(0.0,(double)i,(double)j+n);
      }
   }
   for (j=-w;j<=w;j+=n) {
      for (i=-w;i<w;i+=n) {
         glVertex3f(0.0,(double)i,(double)j);
         glVertex3f(0.0,(double)i+n,(double)j);
      }
   }
   glEnd();
}


/*
   Create the geometry for the pulsar
*/
void MakePulsar(void)
{
   int i,j,k;
   double cradius = 1;            /* Final radius of the cone */
   double clength = 5;            /* Cone length */
   double sradius = 1.3;         /* Final radius of sphere */
   double r1=1.4,r2=1.8;         /* Min and Max radius of field lines */
   double x,y,z;
   XYZ p[4],n[4];
   static double rotateangle = 0.0;

   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

   /* Top level rotation  - spin */
   glPushMatrix();
   glRotatef(rotateangle,0.0,1.0,0.0);

   /* Rotation about spin axis */
   glPushMatrix();
   glRotatef(45.0,0.0,0.0,1.0);

   /* Light in center */
   glutSolidSphere(0.9,16,8);

   /* Spherical center */
   for (i=0;i<360;i+=5) {
      for (j=-80;j<80;j+=5) {

         p[0].x = sradius * cos(j*DTOR) * cos(i*DTOR);
         p[0].y = sradius * sin(j*DTOR);
         p[0].z = sradius * cos(j*DTOR) * sin(i*DTOR);
         n[0] = p[0];

         p[1].x = sradius * cos((j+5)*DTOR) * cos(i*DTOR);
         p[1].y = sradius * sin((j+5)*DTOR);
         p[1].z = sradius * cos((j+5)*DTOR) * sin(i*DTOR);
         n[1] = p[1];

         p[2].x = sradius * cos((j+5)*DTOR) * cos((i+5)*DTOR);
         p[2].y = sradius * sin((j+5)*DTOR);
         p[2].z = sradius * cos((j+5)*DTOR) * sin((i+5)*DTOR);
         n[2] = p[2];

         p[3].x = sradius * cos(j*DTOR) * cos((i+5)*DTOR);
         p[3].y = sradius * sin(j*DTOR);
         p[3].z = sradius * cos(j*DTOR) * sin((i+5)*DTOR);
         n[3] = p[3];

         glBegin(GL_POLYGON);
         if (i % 20 == 0)
            glColor3f(1.0,1.0,1.0);
         else
            glColor3f(0.5,0.5,0.5);
         for (k=0;k<4;k++) {
            glNormal3f(n[k].x,n[k].y,n[k].z);
            glVertex3f(p[k].x,p[k].y,p[k].z);
         }
         glEnd();
      }
   }

   /* Draw the cones */
   for (j=-1;j<=1;j+=2) {
      for (i=0;i<360;i+=10) {
         
         p[0]   = origin;
         n[0]   = p[0];
         n[0].y = -1;

         p[1].x = cradius * cos(i*DTOR);
         p[1].y = j*clength;
         p[1].z = cradius * sin(i*DTOR);
         n[1]   = p[1];
         n[1].y = 0;

         p[2].x = cradius * cos((i+10)*DTOR);
         p[2].y = j*clength;
         p[2].z = cradius * sin((i+10)*DTOR);
         n[2]   = p[2];
         n[2].y = 0;

         glBegin(GL_POLYGON);
         if (i % 30 == 0)
            glColor3f(0.2,0.2,0.2);
         else
            glColor3f(0.5,0.5,0.5);
         for (k=0;k<3;k++) {
            glNormal3f(n[k].x,n[k].y,n[k].z);
            glVertex3f(p[k].x,p[k].y,p[k].z);
         }
         glEnd();
      }
   }

   /* Draw the field lines */
   for (j=0;j<360;j+=20) {
      glPushMatrix();
      glRotatef((double)j,0.0,1.0,0.0);
      glBegin(GL_LINE_STRIP);
      glColor3f(0.7,0.7,0.7);
      for (i=-140;i<140;i++) {
         x = r1 + r1 * cos(i*DTOR);
         y = r2 * sin(i*DTOR);
         z = 0;
         glVertex3f(x,y,z);   
      }   
      glEnd();
      glPopMatrix();      
   }

   glPopMatrix(); /* Pulsar axis rotation */
   glPopMatrix(); /* Pulsar spin */
   rotateangle += rotatespeed;
}

/*
   Set up the lighing environment
*/
void MakeLighting(void)
{
   GLfloat fullambient[4] = {1.0,1.0,1.0,1.0};
   GLfloat position[4] = {0.0,0.0,0.0,0.0};
   GLfloat ambient[4]  = {0.2,0.2,0.2,1.0};
   GLfloat diffuse[4]  = {1.0,1.0,1.0,1.0};
   GLfloat specular[4] = {0.0,0.0,0.0,1.0};

   /* Turn off all the lights */
   glDisable(GL_LIGHT0);
   glDisable(GL_LIGHT1);
   glDisable(GL_LIGHT2);
   glDisable(GL_LIGHT3);
   glDisable(GL_LIGHT4);
   glDisable(GL_LIGHT5);
   glDisable(GL_LIGHT6);
   glDisable(GL_LIGHT7);
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);

   /* Turn on the appropriate lights */
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT,fullambient);
   glLightfv(GL_LIGHT0,GL_POSITION,position);
   glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
   glEnable(GL_LIGHT0);

   /* Sort out the shading algorithm */
   glShadeModel(GL_SMOOTH);

   /* Turn lighting on */
   glEnable(GL_LIGHTING);
}

/*
   Deal with plain key strokes
*/
void HandleKeyboard(unsigned char key,int x, int y)
{
   switch (key) {
   case ESC:                            /* Quit */
   case 'Q':
   case 'q': 
      exit(0); 
      break;
   case 'h':                           /* Go home     */
   case 'H':
      CameraHome(0);
      break;
   case 'w':                           /* Save one image */
   case 'W':
      windowdump = TRUE;
      break;
   case 'r':                           /* Toggle image recording */
   case 'R':
      movierecord = !movierecord;
      break;
   case '[':                           /* Roll anti clockwise */
      RotateCamera(0,0,-1);
      break;
   case ']':                           /* Roll clockwise */
      RotateCamera(0,0,1);
      break;
   case 'i':                           /* Translate camera up */
   case 'I':
      TranslateCamera(0,1);
      break;
   case 'k':                           /* Translate camera down */
   case 'K':
      TranslateCamera(0,-1);
      break;
   case 'j':                           /* Translate camera left */
   case 'J':
      TranslateCamera(-1,0);
      break;
   case 'l':                           /* Translate camera right */
   case 'L':
      TranslateCamera(1,0);
      break;
   }
}

/*
   Deal with special key strokes
*/
void HandleSpecialKeyboard(int key,int x, int y)
{
   switch (key) {
   case GLUT_KEY_LEFT:
      RotateCamera(-1,0,0);
      break;
   case GLUT_KEY_RIGHT:
      RotateCamera(1,0,0);
      break;
   case GLUT_KEY_UP:
      RotateCamera(0,1,0);
      break;
   case GLUT_KEY_DOWN:
      RotateCamera(0,-1,0);
      break;
   }
}

/*
   Rotate (ix,iy) or roll (iz) the camera about the focal point
   ix,iy,iz are flags, 0 do nothing, +- 1 rotates in opposite directions
   Correctly updating all camera attributes
*/
void RotateCamera(int ix,int iy,int iz)
{
   XYZ vp,vu,vd;
   XYZ right;
   XYZ newvp,newr;
   double radius,dd,radians;
   double dx,dy,dz;

   vu = camera.vu;
   Normalise(&vu);
   vp = camera.vp;
   vd = camera.vd;
   Normalise(&vd);
   CROSSPROD(vd,vu,right);
   Normalise(&right);
   radians = dtheta * PI / 180.0;

   /* Handle the roll */
   if (iz != 0) {
      camera.vu.x += iz * right.x * radians;
      camera.vu.y += iz * right.y * radians;
      camera.vu.z += iz * right.z * radians;
      Normalise(&camera.vu);
      return;
   }

   /* Distance from the rotate point */
   dx = camera.vp.x - camera.pr.x;
   dy = camera.vp.y - camera.pr.y;
   dz = camera.vp.z - camera.pr.z;
   radius = sqrt(dx*dx + dy*dy + dz*dz);

   /* Determine the new view point */
   dd = radius * radians;
   newvp.x = vp.x + dd * ix * right.x + dd * iy * vu.x - camera.pr.x;
   newvp.y = vp.y + dd * ix * right.y + dd * iy * vu.y - camera.pr.y;
   newvp.z = vp.z + dd * ix * right.z + dd * iy * vu.z - camera.pr.z;
   Normalise(&newvp);
   camera.vp.x = camera.pr.x + radius * newvp.x;
   camera.vp.y = camera.pr.y + radius * newvp.y;
   camera.vp.z = camera.pr.z + radius * newvp.z;

   /* Determine the new right vector */
   newr.x = camera.vp.x + right.x - camera.pr.x;
   newr.y = camera.vp.y + right.y - camera.pr.y;
   newr.z = camera.vp.z + right.z - camera.pr.z;
   Normalise(&newr);
   newr.x = camera.pr.x + radius * newr.x - camera.vp.x;
   newr.y = camera.pr.y + radius * newr.y - camera.vp.y;
   newr.z = camera.pr.z + radius * newr.z - camera.vp.z;

   camera.vd.x = camera.pr.x - camera.vp.x;
   camera.vd.y = camera.pr.y - camera.vp.y;
   camera.vd.z = camera.pr.z - camera.vp.z;
   Normalise(&camera.vd);

   /* Determine the new up vector */
   CROSSPROD(newr,camera.vd,camera.vu);
   Normalise(&camera.vu);
}

/*
   Translate (pan) the camera view point
   In response to i,j,k,l keys
   Also move the camera rotate location in parallel
*/
void TranslateCamera(int ix,int iy)
{
   XYZ vp,vu,vd;
   XYZ right;
   XYZ newvp,newr;
   double radians,delta;

   vu = camera.vu;
   Normalise(&vu);
   vp = camera.vp;
   vd = camera.vd;
   Normalise(&vd);
   CROSSPROD(vd,vu,right);
   Normalise(&right);
   radians = dtheta * PI / 180.0;
   delta = dtheta * camera.focallength / 90.0;

   camera.vp.x += iy * vu.x * delta;
   camera.vp.y += iy * vu.y * delta;
   camera.vp.z += iy * vu.z * delta;
   camera.pr.x += iy * vu.x * delta;
   camera.pr.y += iy * vu.y * delta;
   camera.pr.z += iy * vu.z * delta;

   camera.vp.x += ix * right.x * delta;
   camera.vp.y += ix * right.y * delta;
   camera.vp.z += ix * right.z * delta;
   camera.pr.x += ix * right.x * delta;
   camera.pr.y += ix * right.y * delta;
   camera.pr.z += ix * right.z * delta;
}

/*
   Handle mouse events
   Right button events are passed to menu handlers
*/
void HandleMouse(int button,int state,int x,int y)
{
   if (state == GLUT_DOWN) {
      if (button == GLUT_LEFT_BUTTON) {
         currentbutton = GLUT_LEFT_BUTTON;
      } else if (button == GLUT_MIDDLE_BUTTON) {
         currentbutton = GLUT_MIDDLE_BUTTON;
      } 
   }
}

/*
   Handle the main menu
*/
void HandleMainMenu(int whichone)
{
   switch (whichone) {
   case 9: 
      exit(0); 
      break;
   }
}

/*
   Handle the rotation speed menu
   It is in degrees
*/
void HandleSpeedMenu(int whichone)
{
   rotatespeed = (whichone - 1) / 2.0;
}

/*
   Handle the glasses type menu
*/
void HandleGlassesMenu(int whichone)
{
   glassestype = whichone;
}

/*
   Handle the camera menu
*/
void HandleCameraMenu(int whichone)
{
   CameraHome(whichone);   
}

/*
   Handle the model menu
*/
void HandleModelMenu(int whichone)
{
   modeltype = whichone; 
}

/*
   How to handle visibility
*/
void HandleVisibility(int visible)
{
   if (visible == GLUT_VISIBLE)
      HandleTimer(0);
   else
      ;
}

/*
   What to do on an timer event
*/
void HandleTimer(int value)
{
   glutPostRedisplay();
   glutTimerFunc(30,HandleTimer,0);
}

/*
   Handle a window reshape/resize
   Keep it a power of 2 for the textures
   Keep it square
*/
void HandleReshape(int w,int h)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glViewport(0,0,(GLsizei)w,(GLsizei)h);
   camera.screenwidth = w;
   camera.screenheight = h;
}

/*
   Display the program usage information
*/
void GiveUsage(char *cmd)
{
   fprintf(stderr,"Usage: %s [-h] [-f]\n",cmd);
   fprintf(stderr,"          -h   this text\n");
   fprintf(stderr,"          -f   full screen mode\n");
   fprintf(stderr,"Key Strokes\n");
   fprintf(stderr,"  arrow keys   rotate left/right/up/down\n");
   fprintf(stderr,"  left mouse   rotate\n");
   fprintf(stderr,"middle mouse   roll\n");
   fprintf(stderr," right mouse   activates the menus\n");
   fprintf(stderr,"           i   translate up\n");
   fprintf(stderr,"           k   translate down\n");
   fprintf(stderr,"           j   translate left\n");
   fprintf(stderr,"           l   translate right\n");
   fprintf(stderr,"           [   roll clockwise\n");
   fprintf(stderr,"           ]   roll anti clockwise\n");
   fprintf(stderr,"           w   write a frame as a PPM file\n");
   fprintf(stderr,"           r   toggle movie recording on and off\n");
   fprintf(stderr,"           q   quit\n");
   exit(-1);
}

/*
   Move the camera to the home position 
   Or to a predefined stereo configuration
   The model is assumed to be in a 10x10x10 cube
   Centered at the origin
*/
void CameraHome(int mode)
{
   camera.aperture = 60;
   camera.pr = origin;

   camera.vd.x = 1;
   camera.vd.y = 0;
   camera.vd.z = 0;

   camera.vu.x = 0;
   camera.vu.y = 1;
   camera.vu.z = 0;

   camera.vp.x = -10;
   camera.vp.y = 0;
   camera.vp.z = 0;

   switch (mode) {
   case 0:
   case 2:
   case 4:
      camera.focallength = 10;
      break;
   case 1:
      camera.focallength = 5;
      break;
   case 3:
      camera.focallength = 15;
      break;
   }
   
   /* Non stressful stereo setting */
   camera.eyesep = camera.focallength / 30.0;
   if (mode == 4)
      camera.eyesep = 0;
}

/*
   Write the current view to an image file
*/
int WindowDump(int width,int height)
{
   int i,j;
   FILE *fptr;
   static int counter = 0;
   int status = TRUE;
   char fname[32];
   char *image;

   /* Allocate our buffer for the image */
   if ((image = (char*)malloc(3*width*height*sizeof(char))) == NULL) {
      fprintf(stderr,"WindowDump - Failed to allocate memory for image\n");
      return(FALSE);
   }

   glPixelStorei(GL_PACK_ALIGNMENT,1); /* Just in case */

   sprintf(fname,"anaglyph_%04d.ppm",counter++);
   if ((fptr = fopen(fname,"w")) == NULL) {
      fprintf(stderr,"WindowDump - Failed to open file for window dump\n");
      return(FALSE);
   }

   /* Copy the image into our buffer */
   glReadBuffer(GL_BACK);
   glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,image);

   /* Write the file */
   fprintf(fptr,"P6\n%d %d\n255\n",width,height);
   if (fwrite(image,3*width*height*sizeof(char),1,fptr) != 1) 
      status = FALSE;
   fclose(fptr);

   free(image);
   return(status);
}

/*
   Handle mouse motion
*/
void HandleMouseMotion(int x,int y)
{
   static int xlast=-1,ylast=-1;
   int dx,dy;

   dx = x - xlast;
   dy = y - ylast;
   if (dx < 0)      dx = -1;
   else if (dx > 0) dx =  1;
   if (dy < 0)      dy = -1;
   else if (dy > 0) dy =  1;

   if (currentbutton == GLUT_LEFT_BUTTON)
      RotateCamera(-dx,dy,0);
   else if (currentbutton == GLUT_MIDDLE_BUTTON)
      RotateCamera(0,0,dx);

   xlast = x;
   ylast = y;
}

/*
   Normalise a vector
*/
void Normalise(XYZ *p)
{
   double length;

   length = sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
   if (length != 0) {
      p->x /= length;
      p->y /= length;
      p->z /= length;
   } else {
      p->x = 0;
      p->y = 0;
      p->z = 0;
   }
}

/*
   Calculate the unit normal at p given two other points
   p1,p2 on the surface. The normal points in the direction
   of p1 crossproduct p2
*/
XYZ CalcNormal(XYZ p,XYZ p1,XYZ p2)
{
   XYZ n,pa,pb;

   pa.x = p1.x - p.x;
   pa.y = p1.y - p.y;
   pa.z = p1.z - p.z;
   pb.x = p2.x - p.x;
   pb.y = p2.y - p.y;
   pb.z = p2.z - p.z;
   Normalise(&pa);
   Normalise(&pb);

   n.x = pa.y * pb.z - pa.z * pb.y;
   n.y = pa.z * pb.x - pa.x * pb.z;
   n.z = pa.x * pb.y - pa.y * pb.x;
   Normalise(&n);

   return(n);
}



class StereoCamera
{
public:
    StereoCamera(   
        float Convergence, 
        float EyeSeparation, 
        float AspectRatio, 
        float FOV, 
        float NearClippingDistance, 
        float FarClippingDistance
        )
    {
        mConvergence            = Convergence; 
        mEyeSeparation          = EyeSeparation; 
        mAspectRatio            = AspectRatio; 
        mFOV                    = FOV * PI / 180.0f; 
        mNearClippingDistance   = NearClippingDistance;
        mFarClippingDistance    = FarClippingDistance;
    }

    void ApplyLeftFrustum()
    {
        float top, bottom, left, right;

        top     = mNearClippingDistance * tan(mFOV/2);
        bottom  = -top;

        float a = mAspectRatio * tan(mFOV/2) * mConvergence;

        float b = a - mEyeSeparation/2;
        float c = a + mEyeSeparation/2;

        left    = -b * mNearClippingDistance/mConvergence;
        right   =  c * mNearClippingDistance/mConvergence;

        // Set the Projection Matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();   
        glFrustum(left, right, bottom, top, 
                  mNearClippingDistance, mFarClippingDistance);

        // Displace the world to right
        glMatrixMode(GL_MODELVIEW);                     
        glLoadIdentity();   
        glTranslatef(mEyeSeparation/2, 0.0f, 0.0f);
    }

    void ApplyRightFrustum()
    {
        float top, bottom, left, right;

        top     = mNearClippingDistance * tan(mFOV/2);
        bottom  = -top;

        float a = mAspectRatio * tan(mFOV/2) * mConvergence;

        float b = a - mEyeSeparation/2;
        float c = a + mEyeSeparation/2;

        left    =  -c * mNearClippingDistance/mConvergence;
        right   =   b * mNearClippingDistance/mConvergence;

        // Set the Projection Matrix
        glMatrixMode(GL_PROJECTION);                        
        glLoadIdentity();   
        glFrustum(left, right, bottom, top, 
                  mNearClippingDistance, mFarClippingDistance);

        // Displace the world to left
        glMatrixMode(GL_MODELVIEW);                     
        glLoadIdentity();   
        glTranslatef(-mEyeSeparation/2, 0.0f, 0.0f);
    }

private:
    float mConvergence;
    float mEyeSeparation;
    float mAspectRatio;
    float mFOV;
    float mNearClippingDistance;
    float mFarClippingDistance;

};

// main rendering function
void DrawGLScene(GLvoid)                                    
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up the stereo camera system
    StereoCamera cam(
        2000.0f,     // Convergence 
        35.0f,       // Eye Separation
        1.3333f,     // Aspect Ratio
        45.0f,       // FOV along Y in degrees
        10.0f,       // Near Clipping Distance
        20000.0f);   // Far Clipping Distance

    cam.ApplyLeftFrustum();
    glColorMask(true, false, false, false);

    PlaceSceneElements();

    glClear(GL_DEPTH_BUFFER_BIT) ;

    cam.ApplyRightFrustum();
    glColorMask(false, true, true, false);

    PlaceSceneElements();

    glColorMask(true, true, true, true);
}


void PlaceSceneElements()
{
    // translate to appropriate depth along -Z
    glTranslatef(0.0f, 0.0f, -1800.0f);

    // rotate the scene for viewing
    glRotatef(-60.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);

    // draw intersecting tori
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 240.0f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glColor3f(0.2, 0.2, 0.6);
        glutSolidTorus(40, 200, 20, 30);
        glColor3f(0.7f, 0.7f, 0.7f);
        glutWireTorus(40, 200, 20, 30);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(240.0f, 0.0f, 240.0f);
        glColor3f(0.2, 0.2, 0.6);
        glutSolidTorus(40, 200, 20, 30);
        glColor3f(0.7f, 0.7f, 0.7f);
        glutWireTorus(40, 200, 20, 30);
    glPopMatrix();
}

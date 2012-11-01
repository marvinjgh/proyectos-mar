#include "main.h"
#include "off.h"

float angle=0;

GLfloat light_ambient[]={0.0f, 0.0f, 0.0f, 1.0f};
GLfloat light_diffuse[]={1.0f, 1.0f, 1.2f, 1.0f};
GLfloat light_specular[]={0.0f, 0.0f, 0.0f, 1.0f};
GLfloat light_position[]={0.0f, 0.0f,20.0f, 1.0f};
float x_ls, y_ls;
float x_s, y_s, z_s;
float x_t, y_t, z_t;
float x_r, y_r;
const float min_scalar = 0.3f; 
const float max_scalar = 5.f;
const float mouse_rotar_speed = 0.30f;
const float mouse_scalar_speed = 0.02f;
const float mouse_traladar_speed = 0.01f;


enum  BUTTON {LEFT_BUTTON,MIDDLE_BUTTON,RIGHT_BUTTON,NONE} boton;

bool scalet[3];

GLUquadric* GlightSphere;
GLhandleARB v,f,p;

void CheckSLError(GLhandleARB &object)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetObjectParameterivARB(object, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infologLength);

	if (infologLength > 0)
	{
    infoLog = (char *)malloc(infologLength);
    glGetInfoLogARB(object, infologLength, &charsWritten, infoLog);
    free(infoLog);
	}
}

void init(void)
{   

	
	glEnable(GL_NORMALIZE);
	
	//glEnable(GL_CULL_FACE);
	glEnable(GL_FLAT);
	//glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
     
     // Light model parameters:
     // -------------------------------------------
     
     GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
     
     glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
     glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);
     
     // -------------------------------------------
     // Spotlight Attenuation
     
     GLfloat spot_direction[] = {1.0, -1.0, -1.0 };
     GLint spot_exponent = 30;
     GLint spot_cutoff = 180;
     
     glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
     glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
     glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
    
     GLfloat Kc = 1.0;
     GLfloat Kl = 0.0;
     GLfloat Kq = 0.0;
     
     glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
     glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
     glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);
     
     
     // ------------------------------------------- 
     // Lighting parameters:

     GLfloat light_pos[] = {5.0f, 5.0f, 5.0f, 1.0f};
     GLfloat light_Ka[]  = {1.0f, 0.5f, 0.5f, 1.0f};
     GLfloat light_Kd[]  = {1.0f, 0.1f, 0.1f, 1.0f};
     GLfloat light_Ks[]  = {1.0f, 1.0f, 1.0f, 1.0f};

     glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
     glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
     glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
     glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

     // -------------------------------------------
     // Material parameters:

     GLfloat material_Ka[] = {0.5f, 0.0f, 0.0f, 1.0f};
     GLfloat material_Kd[] = {0.4f, 0.4f, 0.5f, 1.0f};
     GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f};
     GLfloat material_Ke[] = {0.1f, 0.0f, 0.0f, 0.0f};
     GLfloat material_Se = 20.0f;

     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
     glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
     glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);


	 read("space_station.off");
	 normalizar();
	GlightSphere =  gluNewQuadric();

	
	x_s = y_s = z_t = 1.0;
	x_t = y_t = z_t = 0.0;
	scalet[0]=scalet[1]=scalet[2]=true;
}

void display (void){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
    GLfloat pos[]={0.0f,0.0f,0.0f,1.0f};
	gluLookAt(0.0f,1.0f,10.0f,
				0.0,0.0,-1.0,			       
				0.0f,1.0f,0.0f);
	glRotatef(angle, 0, 1, 0);
	glTranslatef(0,0,-10.f);
	
	glUseProgramObjectARB(p);
	angle+=0.1f;
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	gluSphere(GlightSphere,0.5,20,20);
	glLoadIdentity();
	gluLookAt(0.0f,1.0f,10.0f,
			0.0,0.0,-1.0,			       
			0.0f,1.0f,0.0f);
	glTranslatef(0,0,-5.f);
	glTranslatef(x_t,y_t,z_t);
	glScalef(x_s,y_s,1.0);
	glRotatef(x_r, 0, 1, 0);
	glRotatef(y_r, 1, 0, 0);
	pintar();
	angle+=0.1f;
	
	//TwDraw();
	glUseProgramObjectARB(NULL);
	glutSwapBuffers();
	glutPostRedisplay();
	glLoadIdentity();
}

void reshape (int width, int height)
{
	if(height == 0)
		height = 1;
	
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	gluPerspective(45.0, (float)width / (float)height, 1.0, 200.0);
	//TwWindowSize(width, height);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutSwapBuffers();
	
}

char* readTextFile( char *f ){

	FILE *file;
	char *shaderReaded = NULL;
	int byteCount = 0;

	fopen_s( &file, f, "rt" );
	if( file != NULL ){
		fseek(file, 0, SEEK_END);
		byteCount = ftell( file );
		rewind( file );

		if( byteCount ){
			shaderReaded = (char *) malloc( sizeof(char)*( byteCount+1 ) );
			byteCount = (int)fread( shaderReaded, sizeof(char), byteCount, file );
			shaderReaded[ byteCount ] = '\0';
		}
		fclose(file);
	}

	return shaderReaded;
} 

void setShaders() 
{
	char *vs,*fs;

	v = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);CheckSLError(v);
	f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);CheckSLError(f);

	vs = readTextFile("pintar.vs");
	fs = readTextFile("pintar.fs");

	const char * ff = fs;
	const char * vv = vs;

	glShaderSourceARB(v, 1, &vv,NULL);CheckSLError(v);
	glShaderSourceARB(f, 1, &ff,NULL);CheckSLError(f);

	free(vs);
	free(fs);

	glCompileShaderARB(v); CheckSLError(v);
	glCompileShaderARB(f); CheckSLError(f);

	p = glCreateProgramObjectARB();CheckSLError(p);
	glAttachObjectARB(p,f);CheckSLError(p);
	glAttachObjectARB(p,v);CheckSLError(p);
	glLinkProgramARB(p);CheckSLError(p);
}

void pasivo(int x, int y)
{
	boton=NONE;
}

void motion_mouse(int x, int y)
 {
	float dx = (x - x_ls), dy = (y - y_ls);
	
	if (boton == LEFT_BUTTON)
	{
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
		{
			z_t -= dy * mouse_traladar_speed;

		}else{
			x_t += dx * mouse_traladar_speed;
			y_t -= dy * mouse_traladar_speed;
		}
		x_ls=(float)x;
		y_ls=(float)y;
	}

	if (boton == MIDDLE_BUTTON)
	{
		if (scalet[0])
		{
			x_s -= dy * mouse_scalar_speed; 
			if (x_s < min_scalar)
				x_s = min_scalar;
			if (x_s > max_scalar)
				x_s = max_scalar;
		}
		if (scalet[1])
		{
			y_s -= dy * mouse_scalar_speed; 
			if (y_s < min_scalar)
				y_s = min_scalar;
			if (y_s > max_scalar)
				y_s = max_scalar;
		}
		if (scalet[2])
		{
			z_s -= dy * mouse_scalar_speed; 
			if (z_s < min_scalar)
				z_s = min_scalar;
			if (z_s > max_scalar)
				z_s = max_scalar;
		}
		y_ls=(float)y;
	}
	if (boton == RIGHT_BUTTON)
	{

		x_r += dx * mouse_rotar_speed;
		y_r += dy * mouse_rotar_speed;

		x_ls=(float)x;
		y_ls=(float)y;
	}
 }

void mouse(int button, int state, int x, int y) 
{	
	if (button == GLUT_LEFT_BUTTON){
		x_ls = (float)x;
		y_ls = (float)y;
		boton = LEFT_BUTTON;
	}
	if (button == GLUT_MIDDLE_BUTTON){
		x_ls = (float)x;
		y_ls = (float)y;
		boton = MIDDLE_BUTTON;
	}
	if (button == GLUT_RIGHT_BUTTON){
		x_ls = (float)x;
		y_ls = (float)y;
		boton = RIGHT_BUTTON;
	}
}
void keyboard(unsigned char key, int, int y){
	if (key == 27)
			exit(0);

	if (key == 'y' || key == 'Y')
	{
		scalet[0]=scalet[2]=false;
		scalet[1]=true;
	}
	if (key == 'x' || key == 'X')
	{
		scalet[1]=scalet[2]=false;
		scalet[0]=true;
	}
	if (key == 'z' || key == 'Z')
	{
		scalet[1]=scalet[2]=false;
		scalet[0]=true;
	}
}

void releasekeyboard (unsigned char key, int x, int y){ 	

	if (key == 'y' || key == 'Y' || key == 'x' || key == 'X' || key == 'z' || key == 'Z')
		scalet[0]=scalet[1]=scalet[2]=true;
}

int main(int argc, char **argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-800)>>1, (glutGet(GLUT_SCREEN_HEIGHT)-600)>>1);
	glutCreateWindow("Proyecto 1");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion_mouse);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(releasekeyboard);

	glutPassiveMotionFunc(pasivo);


	glewInit();
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
		printf("Ready for GLSL\n");
	else 
	{
		printf("No GLSL support\n");
		exit(1);
	}

	setShaders();
	init();
	glutMainLoop();

	return 0;
}
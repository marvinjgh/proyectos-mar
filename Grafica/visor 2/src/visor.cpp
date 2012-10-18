#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cstdio>
#include <Geom\mat4x4.h>

GLuint positionBufferObject;


const float vertexPositions[] = 
{
0.50f, 0.50f, 0.0f, 1.0f,
0.50f, -0.50f, 0.0f, 1.0f,
-0.50f, -0.50f, 0.0f, 1.0f,
};

void init(void)
{   
	glGenBuffers(1, &positionBufferObject);
glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), 
vertexPositions, GL_STATIC_DRAW);
glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display (void){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
glClear(GL_COLOR_BUFFER_BIT);
//glUseProgram(theProgram);
glColor3f(1.0,1.0,1.0);
glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
glDrawArrays(GL_TRIANGLES, 0, 3);
glDisableVertexAttribArray(0);
glUseProgram(0);
glutSwapBuffers();
	

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


void pasivo(int x, int y)
{
	
}

void motion_mouse(int x, int y)
 {
	
 }

void mouse(int button, int state, int x, int y) 
{	
	
}

void keyboard(unsigned char key, int, int y){
	
}

void releasekeyboard (unsigned char key, int x, int y){ 	

	
}

int main(int argc, char **argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-800)>>1, (glutGet(GLUT_SCREEN_HEIGHT)-600)>>1);
	glutCreateWindow("Proyecto 1");
	glewInit();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion_mouse);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(releasekeyboard);

	glutPassiveMotionFunc(pasivo);


	
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
		printf("Ready for GLSL\n");
	else 
	{
		printf("No GLSL support\n");
		exit(1);
	}
	Mat4x4 t;
	init();
	glutMainLoop();

	return 0;
}
#include "GL\glew.h"
#include "GL\freeglut.h"


const float vertexPositions[] = 
{
0.50f, 0.50f, 0.0f, 1.0f,
0.50f, -0.50f, 0.0f, 1.0f,
-0.50f, -0.50f, 0.0f, 1.0f,
};

void display(){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//glUseProgram(theProgram);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	//glUseProgram(0);
	glutSwapBuffers();
}

void initializeVertexBuffer(){
	glGenBuffers(1, &positionBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), 
	vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}



int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-800)>>1, (glutGet(GLUT_SCREEN_HEIGHT)-600)>>1);
	glutCreateWindow("Tarea 3"); 
	
	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	//glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
    //glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
    //glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
    //glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
    //glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);

	glutMainLoop();

	return 0;
}
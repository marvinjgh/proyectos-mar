#include "glsl\Shader.h"
#include "Modelo.h"
#include <AntTweakBar.h>
GLuint positionBufferObject,p2,p3;

Shader t;
Mat4x4 m,p;
Modelo mod;

GLfloat q_rotate[] = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat traslacion[] = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat c_line[]={ 1.0f, 0.0f, 0.0f,1.0f };
GLfloat c_fon1[]={ 0.0f, 1.0f, 0.0f };
GLfloat c_fon2[]={ 0.0f, 1.0f, 0.0f };
GLfloat zoom;
bool bools[]={false,false,false};

const GLuint index[] = 
{
	1,0,2,1,2,3
};

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

void init(void)
{   
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	zoom=1.0f;
	t.loadShader(VERTEX_SHADER,"files/t1.vert");
	t.loadShader(FRAGMENT_SHADER,"files/t1.frag");
	t.create_Link();
	t.enable();
	t.AddUniform("m");
	t.AddUniform("tc");
	t.AddUniform("mysampler1");
	t.AddUniform("mysampler2");
	t.AddUniform("mysampler3");
	t.AddUniform("mysampler4");
	t.AddUniform("mysampler5");
	glUniform1i(t["mysampler1"],0);
	glUniform1i(t["mysampler2"],1);
	glUniform1i(t["mysampler3"],2);
	glUniform1i(t["mysampler4"],3);
	glUniform1i(t["mysampler5"],4);

	t.disable();

	mod.cargarModelo("files/daedric.obj");

}

void PlaceSceneElements()
{
	
    // translate to appropriate depth along -Z
    glTranslatef(0.0f, 0.0f, -800.0f);

    // rotate the scene for viewing
    glRotatef(-60.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);

    // draw intersecting tori
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 140.0f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glColor3f(0.2, 0.2, 0.2);
        glutSolidTorus(40, 200, 20, 30);
        glColor3f(0.7f, 0.7f, 0.7f);
        glutWireTorus(40, 200, 20, 30);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(240.0f, 0.0f, 140.0f);
        glColor3f(0.2, 0.2, 0.2);
        glutSolidTorus(40, 200, 20, 30);
        glColor3f(0.7f, 0.7f, 0.7f);
        glutWireTorus(40, 200, 20, 30);
    glPopMatrix();
	/*
	t.enable();
	glUniformMatrix4fv(t["m"],1,0,m.mat);
	
	for (size_t i = 0; i < mod.texturas.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, mod.texturas[i]); 	
	}
	
	glUniform4f(t["tc"],-1,-1,-1,-1);
	glBindBuffer(GL_ARRAY_BUFFER, mod.BufferObject);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)*10, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*10, (void*)16);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*10, (void*)28);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float)*10, (void*)36);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	glDrawArrays(GL_TRIANGLES,0,mod.total*3);
	if (bools[0]){
		glUniform4fv(t["tc"],1,c_line);
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glEnable(GL_PROGRAM_POINT_SIZE);
		glPointSize(2.f);
	}
	glDrawArrays(GL_TRIANGLES,0,mod.total*3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	t.disable();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	*/
}


void display (void){

	glClearColor(0.0f, 0.0f, 0.0f,0.f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	//Model
	m=MatTranslate(traslacion[0],traslacion[1],traslacion[2])* (MatScale(zoom,zoom,zoom) * (MatRotar(q_rotate)*mod.centro ));
	m =( p * (MatTranslate(0,0,-3)* m));
	//MVP
	
	StereoCamera cam(
        2000.0f,     // Convergence 
        15.0f,       // Eye Separation
        1.3333f,     // Aspect Ratio
        45.0f,       // FOV along Y in degrees
        1.0f,       // Near Clipping Distance
        2000.0f);   // Far Clipping Distance

	cam.ApplyLeftFrustum();
    glColorMask(true, false, false, false);

    PlaceSceneElements();
	
	
	
    glClear(GL_DEPTH_BUFFER_BIT) ;

    cam.ApplyRightFrustum();
    glColorMask(false, false, true, false);

    PlaceSceneElements();

    glColorMask(true, true, true, true);

	/**/

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

	// Set the correct perspective.
	p=buildPerpectiva(45.0,ratio,1.0,100.0);
	TwWindowSize(width, height);
	glutSwapBuffers();
}

void genMenu(TwBar *bar)
{
	//bloque de la rotacion
	TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &q_rotate, 
		" label='Object rotation' opened=true help='Change the object orientation.' ");
	
	//traslacion
	TwAddVarRW(bar, "x", TW_TYPE_FLOAT, &traslacion[0], 
		"label='Mover en x' step=0.08 keyIncr='d' keyDecr='a' help='Permite mover el objeto en el eje x.'");
	TwAddVarRW(bar, "y", TW_TYPE_FLOAT, &traslacion[1], 
		"label='Mover en y' step=0.08 keyIncr='q' keyDecr='e' help='Permite mover el objeto en el eje y.'");
	TwAddVarRW(bar, "z", TW_TYPE_FLOAT, &traslacion[2], 
		"label='Mover en z' step=0.08 keyIncr='w' keyDecr='s' help='Permite mover el objeto en el eje z.'");
	//zoom
	TwAddVarRW(bar, "zoom", TW_TYPE_FLOAT, &zoom, 
		"label='Zoom' min=0.01 max=2.5 step=0.01 keyIncr='+' keyDecr='-' help='Escalamiento de la figura.'");
	//bloque linea*/
	TwAddVarRW(bar, "a1", TW_TYPE_BOOLCPP, &bools[0],
		" label='Activar' help='Activa dibujar las lineas del objeto.' key=l group='Lineas' ");
	TwAddVarRW(bar, "c1", TW_TYPE_COLOR4F, &c_line, 
		"label='Color' help='Cambia el color de las lineas.' group='Lineas' ");
	/*
	//bloque box
	TwAddVarRW(bar, "a4", TW_TYPE_BOOLCPP, &bools[1], 
		" label='Activar' help='Activa dibujar las lineas del objeto.' key=b group='Bounding box' ");
	TwAddVarRW(bar, "c4", TW_TYPE_COLOR3F, &c_box, 
		"label='Color' help='Cambia el color de las lineas.' group='Bounding box' ");

	//bloque del background
	TwAddVarRW(bar, "BgTop", TW_TYPE_COLOR3F, c_bgtop, 
               " group='Background' help='Cambia el color de superior del background.' ");  // 'BgTop' and 'BgBottom' are put in the group 'Background' (which is then created)
    TwAddVarRW(bar, "BgBottom", TW_TYPE_COLOR3F, c_bgbottom, 
               " group='Background' help='Cambia el color inferior del background.' ");
	*/
}

int main(int argc, char **argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-800)>>1, (glutGet(GLUT_SCREEN_HEIGHT)-600)>>1);
	glutCreateWindow("Visor obj");
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS); 
	glewInit();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
	glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
	glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);
	TwGLUTModifiersFunc(glutGetModifiers);

	TwInit(TW_OPENGL, NULL);
	TwBar *bar;
	bar = TwNewBar("Barra");
	TwDefine(" GLOBAL help='Aqui se encuentra .' ");
	TwDefine(" Barra label='Barra' refresh=0.5 position='16 16' size='230 250' alpha=0 color='0 0 0'");

	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
		printf("Ready for GLSL\n");
	else 
	{
		printf("No GLSL support\n");
		cin.get();
		exit(1);
	}
	printf("%d\n",sizeof(short));
	init();
	genMenu(bar);
	glutMainLoop();
	TwTerminate();
	return 0;
}
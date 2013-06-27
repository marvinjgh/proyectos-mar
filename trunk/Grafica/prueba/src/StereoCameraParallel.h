//helper class for stereo visualisation using OpenGL

class StereoCameraParallel {
public:
	StereoCameraParallel();
	void Update();

/* Headerfile gathering info from StereoCameraParallel.cpp 
for use within Source Code.cpp */

public:
	float w,h,z,a,Near,Far;
	float L_l, L_r, L_b ,L_t;
	float R_l ,R_r ,R_b ,R_t;

	GLfloat LookAtLeft[9]; // Lookat Left 9 arrays 
	GLfloat LookAtRight[9]; // Lookat Left 9 arrays 
	GLfloat FrustumLeft[6]; // FrustumLeft 6 arrays 
	GLfloat FrustumRight[6]; // FrustumLeft 6 arrays 
};
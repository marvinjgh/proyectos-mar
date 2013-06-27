//helper class for stereo visualisation using OpenGL

class StereoCameraToedIn {
public:
	StereoCameraToedIn();
	void Update();

/* Headerfile gathering info from StereoCameraToedin.cpp 
for use within Source Code.cpp */

public:
	float w,h,z,a,Near,Far; 
	float half_fov, fov;
	float R_l ,R_r ,R_b ,R_t;

	GLfloat LookAtLeft[9]; // Lookat Left 9 arrays 
	GLfloat LookAtRight[9]; // Lookat Right 9 arrays 
	GLfloat Perspective[4]; // Perspective 4 arrays 
};
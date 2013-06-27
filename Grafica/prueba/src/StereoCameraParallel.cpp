// C++ class for stereo visualisation using OpenGL
// The underlying equations and their implementation are by courtesy of 
// Peter Hughes, Durham University 2009.

// Below is a C++ class for class Source Code.cpp
// a demonstration of stereo viewing in OpenGL using the
// parallel camera's approach


// August 2010, Jimmy Kay, Durham University - Z0915644
// MSc Computer Science, Software Engineering Management
// Acknowledgements: Dr Nick Holliman Durham University

#include <GL/glut.h> 
#include "StereoCameraParallel.h"

//Constructor
StereoCameraParallel::StereoCameraParallel() {
	
}

void StereoCameraParallel::Update() {
	w = 518.4f; // Physical display dimension in mm (width)
	h = 324.0f; // height

	z = 1000.0; // Distance in the scene from the camera to the display plane.
	a = 65.0f; // Camera inter-axial seperation (eye seperation).
	Near = 500.0; // Distance in the scene from the camera to the near plane.
	Far = 1200.0f; //Distance in the scene from the camera to the far plane.

	//Calculations for Left eye/camera frustum
	L_l = -( Near * ( ( w/2 - a/2) / z) ); // Left clipping pane
	L_r = ( Near * ( ( w/2.0 + a/2.0) / z) ); // Right clipping pane
	L_b = -( Near * ( ( h/2.0) / z) ); // Bottom clipping pane
	L_t = ( Near * ( ( h/2.0) / z) ); // Top clipping pane

	//Calculations for Right eye/camera frustum
	R_l = -( Near * ( ( w/2.0 + a/2.0) / z) ); // Left clipping pane
	R_r = ( Near * ( ( w/2.0 - a/2.0) / z) ); // Right clipping pane
	R_b = -( Near * ( ( h/2.0) / z) ); // Bottom clipping pane
	R_t = ( Near * ( ( h/2.0) / z) );// Top clipping pane

	// Lookat points for left eye/camera
	LookAtLeft[0] = (-a/2);
	LookAtLeft[1] = 0.0f;
	LookAtLeft[2] = 0.0f;
	LookAtLeft[3] = (-a/2);
	LookAtLeft[4] = 0.0f;
	LookAtLeft[5] = -z;
	LookAtLeft[6] = 0.0f;
	LookAtLeft[7] = 1.0f;
	LookAtLeft[8] = 0.0f;

	// Lookat points for right eye/camera
	LookAtRight[0] = (a/2);
	LookAtRight[1] = 0.0f;
	LookAtRight[2] = 0.0f;
	LookAtRight[3] = (a/2);
	LookAtRight[4] = 0.0f;
	LookAtRight[5] = -z;
	LookAtRight[6] = 0.0f;
	LookAtRight[7] = 1.0f;
	LookAtRight[8] = 0.0f;

	// Parameters for glFrustum (Left)
	FrustumLeft[0] = L_l;
	FrustumLeft[1] = L_r;
	FrustumLeft[2] = L_b;
	FrustumLeft[3] = L_t;
	FrustumLeft[4] = Near;
	FrustumLeft[5] = Far;

	// Parameters for glFrustums (Right)
	FrustumRight[0] = R_l;
	FrustumRight[1] = R_r;
	FrustumRight[2] = R_b;
	FrustumRight[3] = R_t;
	FrustumRight[4] = Near;
	FrustumRight[5] = Far;
}
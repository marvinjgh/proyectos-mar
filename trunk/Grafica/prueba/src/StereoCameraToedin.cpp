// C++ class for stereo visualisation using OpenGL
// The underlying equations and their implementation are by courtesy of 
// Peter Hughes, Durham University 2009.

// Below is a C++ class for class Source Code.cpp
// a demonstration of stereo viewing in OpenGL using the
// Toed-in camera's approach


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

// August 2010, Jimmy Kay, Durham University - Z0915644
// MSc Computer Science, Software Engineering Management
// Acknowledgements: Dr Nick Holliman Durham University


#include <GL/glut.h> 
#include <math.h>
#include "StereoCameraToedIn.h"

#define M_PI       3.14159265358979323846

// Constructor
StereoCameraToedIn::StereoCameraToedIn() {
	
}

void StereoCameraToedIn::Update() {
	w = 518.4f; // Physical display dimensions in mm (width)
	h = 324.0f; //Height

	z = 1000.0; // Distance in the scene from the camera to the display plane.
	a = 65.0f; // Camera inter-axial seperation (eye seperation).
	Near = 500.0; // Distance in the scene from the camera to the near plane.
	Far = 1200.0f; // Distance in the scene from the camera to the far plane.

	//FOV
	half_fov = atan( (h/2) / z);
	fov = ( ( 2* half_fov) * 180 / M_PI ); // Field of view (fov) in y (vertical axis) direction.

	// Lookat points for left eye/camera
	LookAtLeft[0] = (-a/2);
	LookAtLeft[1] = 0.0f;
	LookAtLeft[2] = 0.0f;
	LookAtLeft[3] = 0.0f;
	LookAtLeft[4] = 0.0f;
	LookAtLeft[5] = -z;
	LookAtLeft[6] = 0.0f;
	LookAtLeft[7] = 1.0f;
	LookAtLeft[8] = 0.0f;

	// Lookat points for right eye/camera
	LookAtRight[0] = (a/2);
	LookAtRight[1] = 0.0f;
	LookAtRight[2] = 0.0f;
	LookAtRight[3] = 0.0f;
	LookAtRight[4] = 0.0f;
	LookAtRight[5] = -z;
	LookAtRight[6] = 0.0f;
	LookAtRight[7] = 1.0f;
	LookAtRight[8] = 0.0f;

	// Parameters for gluPerspective (Left and Right)
	Perspective[0] = fov;
	Perspective[1] = w/h;
	Perspective[2] = Near;
	Perspective[3] = Far;
}
#pragma once
#include "Shader.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#define PI      3.141592654
#define _2PI    6.283185308     // 2 * PI
#define PI_2    1.570796327     // PI / 2
#define PI_3	1.047197551		// PI / 3
#define PI_4    0.785398163     // PI / 4 

class StereoCamera
{
public:
	StereoCamera(   
		float Convergence, 
		float EyeSeparation, 
		float AspectRatio, 
		float FOV, 
		float NearClippingDistance, 
		float FarClippingDistance, 
		float z
		)
	{
		mConvergence            = Convergence; 
		mEyeSeparation          = EyeSeparation; 
		mAspectRatio            = AspectRatio; 
		mFOV                    = FOV * PI / 180.0f; 
		mNearClippingDistance   = NearClippingDistance;
		mFarClippingDistance    = FarClippingDistance;
		zmodel=z;
	}

	void ApplyLeftFrustum(glm::mat4* proyection, glm::mat4* modelview)
	{
		float top, bottom, left, right;

		top     = mNearClippingDistance * tan(mFOV/2);
		bottom  = -top;

		float a = mAspectRatio * tan(mFOV/2) * mConvergence;

		float b = a - mEyeSeparation;
		float c = a + mEyeSeparation;

		left    = -b * mNearClippingDistance/mConvergence;
		right   =  c * mNearClippingDistance/mConvergence;

		// Set the Projection Matrix
		*proyection=glm::mat4();
		*proyection=glm::frustum(left,right,bottom,top,mNearClippingDistance,mFarClippingDistance);
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();   
		//glFrustum(left, right, bottom, top,mNearClippingDistance, mFarClippingDistance);


		// Displace the world to right
		*modelview=glm::mat4();
		glm::vec3 x(mEyeSeparation,0.f,zmodel);
		*modelview=glm::translate(*modelview,x);
		//glMatrixMode(GL_MODELVIEW);                     
		//glLoadIdentity();   
		//glTranslatef(mEyeSeparation/2, 0.0f, zmodel);
	}

	void ApplyRightFrustum(glm::mat4* proyection, glm::mat4* modelview)
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
		/*glMatrixMode(GL_PROJECTION);                        
		glLoadIdentity();   
		glFrustum(left, right, bottom, top, 
		mNearClippingDistance, mFarClippingDistance);*/
		*proyection=glm::mat4();
		*proyection=glm::frustum(left,right,bottom,top,mNearClippingDistance,mFarClippingDistance);

		// Displace the world to left
		/*glMatrixMode(GL_MODELVIEW);                     
		glLoadIdentity();   
		glTranslatef(-mEyeSeparation/2, 0.0f, z);*/
		*modelview=glm::mat4();
		glm::vec3 x(-mEyeSeparation/2.f,0.f,zmodel);
		*modelview=glm::translate(*modelview,x);
	}

	glm::mat4 proyection;
	glm::mat4 modelview;

private:
	float mConvergence;
	float mEyeSeparation;
	float mAspectRatio;
	float mFOV;
	float mNearClippingDistance;
	float mFarClippingDistance;
	float zmodel;
};
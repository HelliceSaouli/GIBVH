/*
 * vCamera.cpp
 *
 *  Created on: Aug 9, 2015
 *      Author: hakosoft
 */

#include "vCamera.h"

vCamera::vCamera(const glm::vec3& eye,const glm::vec3& target, float fav,float aspect,float zNear,float zFar,const glm::mat4&  model)
{
	this->_camPosition = eye;
	this->_target      = target;

	glm::vec3 UP(0.0,1.0,0.0);	 					                    /* this is the up vector */
	this->_p           = glm::perspective(fav,aspect,zNear,zFar);           /* this is projection matrix */
	this->_v           = glm::lookAt(this->_camPosition,this->_target, UP);
	this->inverseViewMatrix = new float[16];
	int indx = 0;
	glm::mat4 inv = glm::inverse(this->_v);
	for(int i = 0 ; i < 4 ; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			this->inverseViewMatrix[indx] = inv[i][j];
			indx++;
		}
	}

	this->fav          = fav;
	this->far          = zFar;
	this->near         = zNear;
	this->_m  = model;
	this->_change      = false;  /* at the begining there is no changes */
}

vCamera::~vCamera()
{
	// TODO Auto-generated destructor stub
}


void vCamera::setPosition(const glm::vec3& eye)
{
	this->_camPosition = eye;

	if(this->_change == false)
		this->_change = true;			/* so can update works fine*/
}

void vCamera::setTarget(const glm::vec3& target)
{
	this->_target = target;

	if(this->_change == false)
		this->_change = true;
}

void vCamera::setUpvector(const glm::vec3& UP)
{
	this->_up = UP;
}
void vCamera::setFav(float Angle)  /* probably am not gonna use it for now*/
{
	this->fav = Angle;
}

void vCamera::setModelMatrix(const glm::mat4& model)
{
	this->_m = model;
}

/**************************************************************************************/

glm::vec3 vCamera::getPosition()const
{
	return this->_camPosition;
}
glm::mat4 vCamera::getViewMatrix()const
{
	return this->_v;
}

float* vCamera::getInverseViewMatrix()const
{
	return this->inverseViewMatrix;
}

glm::mat4 vCamera::getProjectionMatrix()const
{
	return this->_p;
}

glm::mat4 vCamera::getModelViewProjectionMatrix()const
{
	return this->_mvp;
}

/**************************************************************************************/

void vCamera::buildMVP()
{
	this->_mvp = this->_p * this->_v * this->_m;
}

/*************************************************************************************/
void vCamera::update()
{
	if(this->_change == true) /* this mean there is some changes in our camera position and rotation*/
	{
		this->_change = false;
		this->_v           = glm::lookAt(this->_camPosition,this->_target, this->_up);
		int indx = 0;
		glm::mat4 inv = glm::inverse(this->_v);
		for(int i = 0 ; i < 4 ; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				this->inverseViewMatrix[indx] = inv[i][j];
				indx++;
			}
		}
	}

	this->buildMVP();

}

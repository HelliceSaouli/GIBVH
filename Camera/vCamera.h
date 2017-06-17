/*********************************************************************
 * vCamera.h                                                         *
 *                                                                   *
 *  Created on: Aug 9, 2015                                          *
 *      Author: hakosoft saouli                                      *
 *      this a virtual camera 2.0 it will use  GLM data structure    *
 *********************************************************************/

#ifndef VCAMERA_H_
#define VCAMERA_H_

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

#include "util.h"


class vCamera
{
	private:
		glm::mat4 _v;	  // the view matrix
		glm::mat4 _p;    //  the projection matrix
		glm::mat4 _m;	//   the model matrix
		glm::mat4 _mvp;//    the model view projection matrix
		float *inverseViewMatrix;

		glm::vec3 _camPosition;		 /* the position of the camera in world space or the eye*/
		glm::vec3 _target;           /* the focus point where the camera is looking Camera - target = the vector direction*/
		glm::vec3 _up;				 /* the up vector for camera*/

		float near;		             /* the near plane of Frustrum*/
		float far;                   /* the Far plane of  Frustrum*/
		float fav;

		bool _change;				/*  this will help me to detect if there is any changes in camera view matrix so i can update correctly*/
		void buildMVP(); /* this will only multiplay the 3 matrices togther*/

	public:
		vCamera(const glm::vec3&,const glm::vec3&, float ,float ,float,float,const glm::mat4&  model = glm::mat4(1.0)); /* one constractor that do every thing*/
		virtual ~vCamera();

		/****************************************************************
		 *                                                              *
		 *  this is bunch of methodes setter and getter and other stuff *
		 *                                                              *
		 ****************************************************************/
		void setPosition(const glm::vec3&);
		void setTarget(const glm::vec3&);
		void setUpvector(const glm::vec3&);

		void setFav(float);

		void setModelMatrix(const glm::mat4&); /* this to feed the model matrix to the system manual */

		/****************************************************************/
		glm::vec3 getPosition()const;
		glm::mat4 getViewMatrix()const;
		glm::mat4 getProjectionMatrix()const;
		glm::mat4 getModelViewProjectionMatrix()const;
		float* getInverseViewMatrix()const;
		/****************************************************************/
		void update();              /*  this update function will recalculate everything if the camera changed position or where it looks at*/
		/***************************************************************/

		/*****************************************************************************
		 *                                                                           *
		 * this all the function that needed to do transformation betewen coordinats *
		 *                                                                           *
		 *****************************************************************************/
		//glm::vec3 World2Camra(const  glm::vec3&)const;			/* this will take a 3D point in the world and put it in Camera cordinate system*/
		//glm::vec3 Camera2World(const glm::vec3&)const;			/* this do what it said XD*/

		/**************************************************************/

};

#endif /* VCAMERA_H_ */

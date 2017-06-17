/**********************************************************************
 * transform.h                                                        *
 *                                                                    *
 *  Created on: Aug 10, 2015                                          *
 *      Author: hakosoft saouli                                       *
 *      this classs will take care to create for us the model matrix  *
 *      wisch will move the 3D model around. this class is updated    *
 *      simpler version of my old Transform3D class, here we are      *
 *      using GLM to do stuff for us.                                 *
 **********************************************************************/

#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

class transform
{

	private:
		glm::mat4 _m;				/* this is the model 4X4 matrix    */

		glm::mat4 _T; 				/* this is the translation matrix */
		glm::mat4 _R;				/* this is the rotation matrix   */
		glm::mat4 _S;				/* this is the scale matrix     */

	public:
		transform();				/* no need to use a fancy constractor */
		virtual ~transform();

		/********************************************************************
		 *  We only need one getter, set method are usless since we need to *
		 *  calculate the model matrix not just set it                      *
		 ********************************************************************/
		glm::mat4 getModelMatrix();


		/************************************************
		 *                                              *
		 *  1- BASIC TRANSFORMATIONS                    *
		 *                                              *
		 ************************************************/

		//1-1 the translation : A change from one location to another is represented by a translation matrix, T.
		void translate(float,float,float);
		void translate(const glm::vec3&);

		//1-2 The rotation : A rotation transform rotates a vector (position or direction) by a given angle around a given axis passing through the origin.
		void rotate(float,float,float);
		void rotate(const glm::vec3&);

		//1-3 The Scale : scales an entity with factors sx, sy,and sz along the x-, y-, and z- directions respectively.
		void scale(float,float,float);
		void scale(const glm::vec3&);

		/***********************************************************************/
		void update();				/* here we gonna calcluate the model matrix */
};

#endif /* TRANSFORM_H_ */

/*
 * transform.cpp
 *
 *  Created on: Aug 10, 2015
 *      Author: hakosoft
 */

#include "transform.h"

transform::transform()
{
	this->_m = glm::mat4(1.0);
	this->_T = glm::mat4(1.0);
	this->_R = glm::mat4(1.0);
	this->_S = glm::mat4(1.0);
}

transform::~transform()
{
	// TODO Auto-generated destructor stub
}

/*******************************************************************/
void transform::translate(float x,float y, float z)
{
	glm::vec3 val(x,y,z);

	this->_T = glm::translate(val);
}

void transform::translate(const glm::vec3& val)
{
	this->_T = glm::translate(val);
}

/*******************************************************************/

void transform::rotate(float x, float y, float z)
{
	glm::vec3 val(x,y,z);

	glm::mat4 rotXmatrix  = glm::rotate(val.x,glm::vec3(1.0,0.0,0.0));
	glm::mat4 rotYmatrix  = glm::rotate(val.y,glm::vec3(0.0,1.0,0.0));
	glm::mat4 rotZmatrix  = glm::rotate(val.z,glm::vec3(0.0,0.0,1.0));

	this->_R = rotZmatrix * rotYmatrix * rotXmatrix;
}

void transform::rotate(const glm::vec3& val)
{
	glm::mat4 rotXmatrix  = glm::rotate(val.x,glm::vec3(1.0,0.0,0.0));
	glm::mat4 rotYmatrix  = glm::rotate(val.y,glm::vec3(0.0,1.0,0.0));
	glm::mat4 rotZmatrix  = glm::rotate(val.z,glm::vec3(0.0,0.0,1.0));

	this->_R = rotZmatrix * rotYmatrix * rotXmatrix;
}
/*******************************************************************/

void transform::scale(float x,float y,float z)
{
	glm::vec3 val(x,y,z);
	this->_S = glm::scale(val);
}

void transform::scale(const glm::vec3& val)
{
	this->_S = glm::scale(val);
}

/********************************************************************/
void transform::update()
{
	this->_m = this->_T * this->_S * this->_R;
}


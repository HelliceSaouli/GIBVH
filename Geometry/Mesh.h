/*
 * Mesh.h
 *
 *  Created on: Jun 13, 2015
 *      Author: hakosoft
 */

#ifndef MESH_H_
#define MESH_H_

#include "glm/glm.hpp" // i could change it with my other classes that i created before
#include "GL/glew.h"
#include <vector>

class Vertex
{
	private:
		glm::vec3 _pos; //from now on i will start to name my privates variable with _ before it
		glm::vec3 _color;
		glm::vec2 _texCoord; // for texture
	public:
		Vertex(const glm::vec3 &pos,/*const glm::vec3 &color,*/const glm::vec2 &texCoord){
			this->_pos = pos;
			//this->_color = color;
			this->_texCoord = texCoord;
		}

		glm::vec3 getVertexPos(){
			return this->_pos;
		}

		glm::vec2 getTexCoords(){
			return this->_texCoord;
		}

		glm::vec3 getColors(){
			return this->_color;
		}
};
class Mesh
{
	private:

		enum
		{
			POSITION_VB,
			TEXCOORD_VB,
			NUM_BUFFER
		};

		GLuint _vao; // vertex array object modern openGL way to refer to mesh data
		GLuint _vbo[NUM_BUFFER];
		unsigned int _drawCount;

	public:
		Mesh(Vertex*, unsigned int,GLint*);
		void Draw();
		virtual ~Mesh();
};

#endif /* MESH_H_ */

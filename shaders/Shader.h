/*
 * Shader.h
 *
 *  Created on: Jun 12, 2015
 *      Author: hakosoft
 */

#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include "GL/glew.h"
#include "Camera/vCamera.h"

#define NUM_SHADER 2
#define NUM_UNIFORM 2

class Shader
{
    private:

		GLuint m_program;	// OpenGL way to keep track of where the program is. how to refer to shader
		GLuint m_shader[NUM_SHADER];
		GLuint m_uniforms[NUM_UNIFORM];
	public:
		Shader(const std::string&);
		void bind();// set the GPU in state where it  using the vertex shader and fragment shader that are part of this class
		void deleteShader(); // unbind shader
		void Update(const vCamera &);
		GLint GetAttributeLocation(const std::string&);

		virtual ~Shader();

	    std::string LoadShader(const std::string&); // loading the shader program
		void CheckShaderError(GLuint, GLuint, bool, const std::string&); // check all sort of error that could happen
		GLuint CreateShader(const std::string&,GLenum); // this will create the actual shader
};

#endif /* SHADER_H_ */

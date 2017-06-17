/*
 * Shader.cpp
 *
 *  Created on: Jun 12, 2015
 *      Author: hakosoft
 */

#include "Shader.h"
#include <cstdlib>
#include<iostream>
#include<fstream>

Shader::Shader(const std::string& filePath)
{
	this->m_program   = glCreateProgram(); // create our shader program
	this->m_shader[0] = CreateShader(LoadShader(filePath + ".vshader"),GL_VERTEX_SHADER); // create our shader
	this->m_shader[1] = CreateShader(LoadShader(filePath + ".fshader"),GL_FRAGMENT_SHADER);

	for(int i = 0 ; i < NUM_SHADER ; i++)
		glAttachShader(this->m_program,this->m_shader[i]); //attach every shader created to the program

	// link the program
	glLinkProgram(this->m_program);
	CheckShaderError(this->m_program,GL_LINK_STATUS,true,"Error : shader linking failed ..:"); // check if every thing OK

	// validating program
	glValidateProgram(this->m_program);
	CheckShaderError(this->m_program,GL_VALIDATE_STATUS,true,"Error : Invalid program ..:");

	this->m_uniforms[0] = glGetUniformLocation(this->m_program,"MVP");
	this->m_uniforms[1] = glGetUniformLocation(this->m_program,"posCam");
}

Shader::~Shader()
{
	for(int i = 0 ; i < NUM_SHADER ; i++)
	{
		glDetachShader(this->m_program,this->m_shader[i]);
		glDeleteShader(this->m_shader[i]);
	}
	glDeleteProgram(this->m_program);
}

void Shader::deleteShader()
{
	glUseProgram(0);
}

GLint Shader::GetAttributeLocation(const std::string& name)
{
	return glGetAttribLocation(this->m_program,name.c_str());
}

GLuint Shader::CreateShader(const std::string& text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);
	if(shader == 0 )
		std::cerr<<"Failed to create shader..!!!"<<std::endl;

	const GLchar* source[1];
	GLint sourceLength[1];

	sourceLength[0]   = text.length();
	source[0]         = text.c_str();

	glShaderSource(shader,1,source,sourceLength);
	glCompileShader(shader);
	CheckShaderError(shader,GL_COMPILE_STATUS,false,"Error : shader compiling failed ..:");

	return shader;
}

void Shader::bind()
{
	glUseProgram(this->m_program);
}

void Shader::Update(const vCamera &cam)
{
	glm::mat4 MVP = cam.getModelViewProjectionMatrix(); // i should call it mvp

	glm::vec3 posCam   = cam.getPosition();
	glUniformMatrix4fv(this->m_uniforms[0],1,GL_FALSE,&MVP[0][0]);
	glUniform3fv(this->m_uniforms[1],1,&posCam[0]);
}
std::string Shader::LoadShader(const std::string &filePath)
{
	std::ifstream file;
	file.open((filePath).c_str());
	std::string output;
	std::string line;

	if(!file.is_open())
	{
		std::cerr << " Enable to load shader: "<<filePath<<"...!!"<<std::endl;
		exit (EXIT_FAILURE);
	}

	while(file.good())
	{
		getline(file,line);
		output.append(line + "\n");
	}

	file.close();
	return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = {0};

	if(isProgram)
		glGetProgramiv(shader,flag,&success);
	else
		glGetShaderiv(shader,flag, &success);

	if(success == GL_FALSE)
	{
		if(isProgram)
			glGetProgramInfoLog(shader, sizeof(error),NULL,error);
		else
			glGetShaderInfoLog(shader, sizeof(error),NULL,error);

		std::cerr<<errorMessage<<" : '"<<error<<"'"<<std::endl;
	}
}

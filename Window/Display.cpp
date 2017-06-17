/*
 * Display.cpp
 *
 *  Created on: Jun 12, 2015
 *      Author: hakosoft
 */

#include "Display.h"
//include GLEW
#include "GL/glew.h"
Display::Display(int width, int height,const std::string &title)
{
	SDL_Init(SDL_INIT_EVERYTHING); // it's OK  to use it here because we are using SDL just for windowing

	//color bits
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);

	//buffers
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

	this->m_window = SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_OPENGL);
	this->m_glContext = SDL_GL_CreateContext(this->m_window);

	GLenum status = glewInit();

	if(status != GLEW_OK)
	{
		std::cerr << " GLEW Initialization FAILD ..!!" <<std::endl;
	}

	this->isColsed = false;
	glEnable(GL_DEPTH_TEST);

}

Display::~Display()
{
	SDL_GL_DeleteContext(this->m_glContext);
	SDL_DestroyWindow(this->m_window);
	SDL_Quit();	// same thing
}

void Display::Clear(float r, float g, float b, float a) // for clearing buffers and stuff
{
  glClearColor(r,g,b,a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Display::IsClosed()	// checking if the window is closed or not it helps to handle the event
{
	return this->isColsed;
}
void Display::Update()	// updating the window
{
	SDL_GL_SwapWindow(this->m_window);
}

SDL_Window* Display::getWindow()
{
	return this->m_window;
}

/*
 * Display.h
 *
 *  Created on: Jun 12, 2015
 *      Author: hakosoft
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "util.h"
#include <SDL2/SDL.h>

class Display
{
	private:
	SDL_Window* m_window; // our window
	SDL_GLContext m_glContext; // openGL context
	bool isColsed;

	public:
		Display(int,int,const std::string&);
		virtual ~Display();

		void Update();
		bool IsClosed();
		void Clear(float,float,float,float);

		SDL_Window* getWindow();
};

#endif /* DISPLAY_H_ */

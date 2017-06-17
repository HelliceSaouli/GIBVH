/*
 * Fps.h
 *
 *  Created on: Jun 25, 2015
 *      Author: hakosoft saouli
 *
 *      http://sdl.beuc.net/sdl.wiki/SDL_Average_FPS_Measurement
 */

#ifndef FPS_H_
#define FPS_H_

#include "util.h"
#include <SDL2/SDL.h>
#define FRAME_VALUES 200
class Fps
{
	private :
	// An array to store frame times:
	Uint32 frametimes[FRAME_VALUES];
	// Last calculated SDL_GetTicks
	Uint32 frametimelast;
	// total frames rendered
	Uint32 framecount;
	// the value you want
	float framespersecond;
	float _deltatime;

	public:
		Fps();
		void Start();
		void Count();

		float getFPS();
		virtual ~Fps();

		float deltatime()const;
};

#endif /* FPS_H_ */

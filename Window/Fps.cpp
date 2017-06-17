/*
 * Fps.cpp
 *
 *  Created on: Jun 25, 2015
 *      Author: hakosoft
 */

#include "Fps.h"

Fps::Fps()
{

}

Fps::~Fps()
{

}

void Fps::Start()
{
    // Set all frame times to 0ms.
    memset(this->frametimes, 0, sizeof(this->frametimes));
    this->framecount = 0;
	this->framespersecond = 0;
    this->frametimelast = SDL_GetTicks();
}


void Fps::Count()
{
    Uint32 frametimesindex;
    Uint32 getticks;
    Uint32 count;
    Uint32 i;

    // frametimesindex is the position in the array. It ranges from 0 to FRAME_VALUES.
    // This value rotates back to 0 after it hits FRAME_VALUES.
    frametimesindex = this->framecount % FRAME_VALUES;

    // store the current time
    getticks = SDL_GetTicks();

    // save the frame time value
    this->frametimes[frametimesindex] = getticks - this->frametimelast;

    // save the last frame time for the next fpsthink
    this->frametimelast = getticks;


    // increment the frame count
    this->framecount++;

    // Work out the current framerate

    // The code below could be moved into another function if you don't need the value every frame.

    // I've included a test to see if the whole array has been written to or not. This will stop
    // strange values on the first few (FRAME_VALUES) frames.
    if (this->framecount < FRAME_VALUES) {

            count = this->framecount;

    }
    else
    {

            count = FRAME_VALUES;

    }

    // add up all the values and divide to get the average frame time.
    this->framespersecond = 0;
    for (i = 0; i < count; i++) {

           this->framespersecond += this->frametimes[i];

    }

    this->framespersecond /= count;
    this->_deltatime = this->framespersecond  / 1000.0f;
    // now to make it an actual frames per second value...
    this-> framespersecond = 1000.f / this->framespersecond;
}

float Fps::getFPS()
{
	return this->framespersecond;
}

float Fps::deltatime()const
{
	return this->_deltatime;
}

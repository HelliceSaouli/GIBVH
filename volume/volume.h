/*
 * volume.h
 *
 *  Created on: Aug 12, 2015
 *      Author: hakosoft
 */

#ifndef VOLUME_H_
#define VOLUME_H_

#include "util.h"
#include "glm/glm.hpp"

typedef float4 VolumeType;
class volume
{
	private:

	/*******************************************************************
	 * am gonna put some needed variable so that we can use 3D texture *
	 * in cuda it's my first attempt to build class out of it so every *
	 * thing will probably not be  optimiszed and stuff could be done  *
	 * in better way next time                                         *
	 *******************************************************************/

	cudaChannelFormatDesc channelDesc;						/* describes the format of the value that is returned when fetching the texture
															  cudaCreateChannelDesc( 32, 32, 32, 0, cudaChannelFormatKindFloat )*/

	cudaArray*	            cuArray;						/*Allocates a CUDA array according to the cudaChannelFormatDesc structure desc and returns a handle to the new CUDA array in *array*/
	cudaTextureObject_t     deviceVolume; 					/* this is what gonna be passed to a kernel*/
	cudaMemcpy3DParms       copyParams;						/* this copy parametre so we copy our data to 3D cuda array*/
	struct cudaResourceDesc resDesc;						/* some data res description*/
	struct cudaTextureDesc  texDesc;						/* Specify texture object parameters*/

	float4*  _data;
	void *   h_volume;										/* this is what am gonna feel up in cpu*/

	int3 dimention;										    /* this is my volume dimention x,y,z*/


	public:
		volume(int,int,int);								/* here we create and initial our volume data*/
		virtual ~volume();

		void create3DArray();
		void setUp3DTexture();
		void createTextureObject();
		cudaTextureObject_t  getDeviceVolume()const;

};

#endif /* VOLUME_H_ */

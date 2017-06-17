/*
 * volume.cpp
 *
 *  Created on: Aug 12, 2015
 *      Author: hakosoft
 */

#include "volume.h"
#include <string.h>



/*******************************************************
 * to change the texture filtring mode change this define
 ********************************************************/

volume::volume(int x, int y , int z) // this will change i don't like the way it is
{
	int volumesize = x * y * z;

#ifdef DinoToy
	/************************** DinoToy********************/

	float resolution = pow(0.014928/(float)volumesize,1.0/3.0);
	this->_data = new float4[volumesize];
     int index = 0;
     for(int i = 0; i < x; i++)
     {
    	 for(int k = 0; k < z; k++)
     	{
    		 for(int j = 0; j < y; j++)
     		{
            	if(index > volumesize)
            	  break;
         		this->_data[index].x    = -0.13066 + i * resolution;
         		this->_data[index].y    = -0.13066 + j * resolution;
         		this->_data[index].z    = -0.74832 + k * resolution;
         		this->_data[index].w    = 1.0;
         		index++;
     		}
     	}
     }
#endif

#ifdef Temple47
/*****************************Temple 47 dataset*******************************/
	  	double resolution = pow(0.0048640/(float)volumesize,1.0/3.0);
	  	this->_data = new float4[volumesize];
	    	int index = 0;
	    	for(int i = 0; i < x; i ++)
	    	    {
	    	        for(int j = 0; j < y; j ++)
	    	        {
	    	            for(int k =0 ; k < z; k++)
	    	            {
	    	            	if(index > volumesize)
	    	            	  break;
	    	            	else
	    	            	{

	    	            		this->_data [index].x    =  -0.05  + i * resolution;
	    	            		this->_data [index].y    =  -0.022  + j * resolution;
	    	            		this->_data [index].z    =  -0.10  + k * resolution;
	    	            		this->_data [index].w    =  1.0;
	    	            		index++;
	    	            	}
	    	            }
	    	        }
	    	    }
#endif

#ifdef Dinorig48
/******************************Dino 48 dataset*********************************/
    double resolution = pow(0.0048640/(float)volumesize,1.0/3.0);
	this->_data = new float4[volumesize];
  	int index = 0;
  	for(int i = 0; i < x; i ++)
  	    {
  	        for(int j = 0; j < y; j ++)
  	        {
  	            for(int k =0 ; k < z; k++)
  	            {
  	            	if(index > volumesize)
  	            	  break;
  	            	else
  	            	{

  	            		this->_data [index].x    = -0.05  + i * resolution;
  	            		this->_data [index].y    = -0.022 + j * resolution;
  	            		this->_data [index].z    = -0.1  + k * resolution;
  	            		this->_data [index].w    =  1.0;
  	            		index++;
  	            	}
  	            }
  	        }
  	    }
#endif

#ifdef Dinorig361
/******************************Dino 361 dataset*********************************/ //didn't work
    double resolution = pow(0.0004650072/(float)volumesize,1.0/3.0);
	this->_data = new float4[volumesize];
  	int index = 0;
  	for(int i = 0; i < x; i ++)
  	    {
  	        for(int j = 0; j < y; j ++)
  	        {
  	            for(int k =0 ; k < z; k++)
  	            {
  	            	if(index > volumesize)
  	            	  break;
  	            	else
  	            	{

  	            		this->_data [index].x    = -0.041897  + i * resolution;
  	            		this->_data [index].y    =  0.001126 + j * resolution;
  	            		this->_data [index].z    = -0.037845  + k * resolution;
  	            		this->_data [index].w    =  1.0;
  	            		index++;
  	            	}
  	            }
  	        }
  	    }
#endif

#ifdef othere
  	/********************************otheres***************************/
	float  res = 800.0/x;
	this->_data = new float4[volumesize];
	int index = 0;

	for(int i = 0; i < x; i ++)
	    {
			for(int k =0 ; k < z; k++)
	        {
			 for(int j = 0; j < y; j ++)
	            {
	            	if(index > volumesize)
	            	  break;
	            	else
	            	{

	            		this->_data [index].x    = -100 + i * res;
	            		this->_data [index].y    = -100 + j * res;
	            		this->_data [index].z    = -100 + k * res;
	            		this->_data [index].w    =  1.0;
	            		index++;
	            	}
	            }
	        }
	    }

#endif
	  this->h_volume = this->_data;
	  this->dimention = make_int3(x,y,z);

	  this->deviceVolume = 0;
	  this->copyParams = {0};
}

volume::~volume()
{
	// TODO Auto-generated destructor stub
}

void volume::create3DArray()
{
	// create 3D array
	cudaExtent volumeSize = make_cudaExtent(this->dimention.x,this->dimention.y,this->dimention.z);
	this->channelDesc = cudaCreateChannelDesc<VolumeType>();
	CUDA_CHECK_RETURN(  cudaMalloc3DArray(&this->cuArray,&this->channelDesc,volumeSize));

	// copy data to 3D array
	this->copyParams.srcPtr   =  make_cudaPitchedPtr(this->h_volume,volumeSize.width * sizeof(float4),volumeSize.width,volumeSize.height);
	this->copyParams.dstArray =  this->cuArray;
	this->copyParams.extent   =  volumeSize;
	this->copyParams.kind     =  cudaMemcpyHostToDevice;
	CUDA_CHECK_RETURN(cudaMemcpy3D(&this->copyParams));

}

void volume::setUp3DTexture()
{
	// Specify texture
	memset(&this->resDesc,0,sizeof(this->resDesc));
	this->resDesc.resType = cudaResourceTypeArray;
	this->resDesc.res.array.array = this->cuArray;

	// Specify texture object parameters
	memset(&this->texDesc,0,sizeof(this->texDesc));
	this->texDesc.normalizedCoords = true;
	this->texDesc.addressMode[0]   = cudaAddressModeClamp;
	this->texDesc.addressMode[1]   = cudaAddressModeClamp;

#if filterModes == 0
	this->texDesc.filterMode       = cudaFilterModeLinear;
#else
	this->texDesc.filterMode       = cudaFilterModePoint;
#endif


	this->texDesc.readMode         = cudaReadModeElementType;
}

void volume::createTextureObject()
{
	cudaCreateTextureObject(&this->deviceVolume,&this->resDesc,&this->texDesc,NULL);
}

cudaTextureObject_t volume::getDeviceVolume()const
{
	return this->deviceVolume;
}

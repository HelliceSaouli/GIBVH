/*
 * PixelBuffer.h
 *
 *  Created on: Aug 5, 2015
 *      Author: hakosoft saouli
 *      this class  is responsible  for pixel buffer and it relation
 *      with GPU device
 *
 *      this code inspired from NVIDIA SDK VolumeRendering sample
 */

#ifndef PIXELBUFFER_H_
#define PIXELBUFFER_H_

#include <string>
#include <iostream>
#include "Silhouettes/ImageDataArrays.h"
#include "Camera/vCamera.h"
#include "GL/glew.h"

#include <cuda_runtime.h>
#include <cuda_gl_interop.h>  // it will help to manipulate OpenGL resources via CUDA device

class PixelBuffer
{
	private:
		int w,h;
		GLuint _pbo;     // Pixel buffer object
		GLuint _image;  // Texture Object
		struct cudaGraphicsResource *cuda_pbo_rsc; // CUDA Graphics Resource (to transfer PBO)

	public:
		PixelBuffer(int,int); // we will use standard  class contractor
		virtual ~PixelBuffer();

		void clear();
		void creatPBO();
		void registerPBOToCUDA();
		void creatTexture();
		void fromPBOToTexture();
		void bindTargetTex();
		void render(const cudaTextureObject_t &,const ImageDataArrays&,const vCamera&,bool,int w, int h);

		inline // next time put in the utility functions
		int iDivUp(int a, int b)
		{
		    return (a % b != 0) ? (a / b + 1) : (a / b);
		}
};

#endif /* PIXELBUFFER_H_ */

/*
 * PixelBuffer.cpp
 *
 *  Created on: Aug 5, 2015
 *      Author: hakosoft
 */

#include "PixelBuffer.h"

extern "C"
void CallKernelraycaster(uint *d_output,cudaTextureObject_t texObject,glm::vec3 vCamPosition, int imageW, int imageH/*,unsigned char *slihouette*/,glm::mat4 *camera,
						 uchar4* images,bool isCOlor,unsigned int imageNumber,int w, int h,dim3 Blocks,dim3 Threads );

extern "C"
void copyInvViewMatrix(const float* invViewMatrix, size_t sizeofMatrix); /* copying data from host to constane GPU memory*/
PixelBuffer::PixelBuffer(int w,int h)
{
	this->w      = w;
	this->h      = h;
	this->_image = 0;
	this->_pbo   = 0;

}

PixelBuffer::~PixelBuffer()
{
	// TODO Auto-generated destructor stub
}

void PixelBuffer::clear()
{
	cudaGraphicsUnregisterResource(this->cuda_pbo_rsc);
	glDeleteBuffers(1,&this->_pbo);
	glDeleteTextures(1,&this->_image);
	cudaDeviceReset();
}

void PixelBuffer::creatPBO()
{
	if(this->_pbo != 0)
	{
		this->clear();
	}

	glGenBuffers(1,&this->_pbo);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER,this->_pbo);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, this->w * this->h* sizeof(GLubyte) * 4, 0, GL_STREAM_DRAW); // RGBA
}

void PixelBuffer::registerPBOToCUDA()
{
	cudaGraphicsGLRegisterBuffer(&this->cuda_pbo_rsc, this->_pbo, cudaGraphicsMapFlagsWriteDiscard);
}

void PixelBuffer::creatTexture()
{
	glGenTextures(1,&this->_image);
	glBindTexture(GL_TEXTURE_2D,this->_image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,this->w,this->h,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
    glBindTexture(GL_TEXTURE_2D,0);
}

void PixelBuffer::fromPBOToTexture()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, this->_pbo);
    glBindTexture(GL_TEXTURE_2D, this->_image);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->w, this->h, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

void PixelBuffer::bindTargetTex()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,this->_image);
}
void PixelBuffer::render(const cudaTextureObject_t &texObject,const ImageDataArrays &data,const vCamera& vCam,bool isCOlor,int w, int h)
{

	size_t sizeofMatrix = sizeof(float4)*4 ;
	copyInvViewMatrix(vCam.getInverseViewMatrix(),sizeofMatrix);
	// do some matrix calculation here next time
    uint *d_output;
    cudaGraphicsMapResources(1, &this->cuda_pbo_rsc, 0);
    size_t bytes;
    cudaGraphicsResourceGetMappedPointer((void **)&d_output, &bytes,this->cuda_pbo_rsc);
    cudaMemset(d_output, 0,this->w* this->h*4);
    // call CUDA kernel, writing results to PBO
    dim3 blockSize(16, 16);
    dim3 gridSize(iDivUp(this->w, blockSize.x), iDivUp(this->h, blockSize.y));
    CallKernelraycaster(d_output,texObject,vCam.getPosition(),this->w,this->h/*,data.d_silhouette_array*/,data.d_CameraMatrices,data.d_image_array,isCOlor,data.imageCount,w,h,gridSize,blockSize);
    cudaGraphicsUnmapResources(1, &this->cuda_pbo_rsc, 0);
}


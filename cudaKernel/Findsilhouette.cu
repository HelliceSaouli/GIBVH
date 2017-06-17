/****************************************************************************
 * Author : saouli abdelhak (Hakosoft)
 * date : 11/07/2015
 *
 * this kernel is for to find a silhouette from an image its' simple task
 * we can add complicated stuff later on
 ****************************************************************************/

#include <stdio.h>
#include <cuda.h>
#include <iostream>
#include "util.h"

__device__ int ThreadID_2D_2D(unsigned int width)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	return y * width + x;
}

// this kernel is used for a specific data set (dinausre) once i start using camera i will write one kernel

__global__ void rgbaToSilhouette(uchar4* input,int CameraID,uchar4* ImageSilhouettes/*,unsigned char* output*/, unsigned int rows, unsigned int cols)
{

	int thread_1D_pos = ThreadID_2D_2D(cols);
	uchar4 rgba;
	rgba = input[thread_1D_pos];
	int imageSize = rows * cols;
	if(rgba.x>rgba.z)
	{
		rgba.w = 1.0;
		ImageSilhouettes[(CameraID*imageSize) + thread_1D_pos] = rgba;
	}
		//output[thread_1D_pos] = 255;
	else
	{
		rgba.w = 0.0;
		ImageSilhouettes[(CameraID*imageSize) + thread_1D_pos] = rgba;
	}
		//output[thread_1D_pos] = 0;
}

//use for other data sets
__global__ void rgbaToSilhouette2(uchar4* input,int CameraID,uchar4* ImageSilhouettes/*, unsigned char* output*/, unsigned int rows, unsigned int cols) //this white background
{

	int thread_1D_pos = ThreadID_2D_2D(cols);
	if(thread_1D_pos > rows * cols)
		return;
	uchar4 rgba;
	int imageSize = rows * cols;

	rgba = input[thread_1D_pos];
	float grayIn = .299f * rgba.x + .587f * rgba.y + .114f * rgba.z;
	if(grayIn >= 200.0)
	{
		//output[thread_1D_pos] = 0;
		rgba.w = 0.0;
		ImageSilhouettes[(CameraID*imageSize) + thread_1D_pos] = rgba;
	}

	else
	{
		//output[thread_1D_pos] = 255;
		rgba.w = 1.0;
		ImageSilhouettes[(CameraID*imageSize) + thread_1D_pos] = rgba;
	}

}


__global__ void rgbaToSilhouette3(uchar4* input,int CameraID,uchar4* ImageSilhouettes/*, unsigned char* output*/, unsigned int rows, unsigned int cols) //this white background
{

	int thread_1D_pos = ThreadID_2D_2D(cols);
	if(thread_1D_pos > rows * cols)
		return;
	uchar4 rgba;
	int imageSize = rows * cols;

	rgba = input[thread_1D_pos];

	float grayIn = .299f * rgba.z + .587f * rgba.y + .114f * rgba.x;
	if(grayIn > 20.0)
	{
		/*input[thread_1D_pos].x = 255;
		input[thread_1D_pos].y = 255;
		input[thread_1D_pos].z = 255;
		input[thread_1D_pos].w = 255;*/
		rgba.w = 1.0;
		ImageSilhouettes[(CameraID*imageSize) + thread_1D_pos] = rgba;
	}

	else
	{
		/*input[thread_1D_pos].x = 0.0;
		input[thread_1D_pos].y = 0.0;
		input[thread_1D_pos].z = 0.0;
		input[thread_1D_pos].w = 255;*/
		rgba.w = 0.0;
		ImageSilhouettes[(CameraID*imageSize) + thread_1D_pos] = rgba;
	}

}
extern "C"
void CallBackrgbaToSilhouetteKernel(uchar4* input,int CameraID,uchar4* ImageSilhouettes/*, unsigned char* output*/, unsigned int rows, unsigned int cols,
							        dim3 blocks,dim3 grid)
{
	rgbaToSilhouette<<<grid,blocks>>>(input,CameraID,ImageSilhouettes/*,output*/,rows,cols);
	cudaDeviceSynchronize();
	CUDA_CHECK_RETURN(cudaGetLastError());
}

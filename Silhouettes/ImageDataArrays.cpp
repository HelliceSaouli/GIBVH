/*
 * ImageDataArrays.cpp
 *
 *  Created on: Jul 11, 2015
 *      Author: hakosoft
 */

#include "ImageDataArrays.h"



ImageDataArrays::ImageDataArrays(const unsigned int imagecount,unsigned int _w,unsigned int _h)
{
	//size_t silhouettesSize = imagecount * _w * _h * sizeof(unsigned char);
	size_t imagesSize      = imagecount * _w * _h * sizeof(uchar4);

	size_t MatricesSize    = imagecount * sizeof(glm::mat4);

	// allocate device data
	//CUDA_CHECK_RETURN(cudaMalloc((void**)&this->d_silhouette_array,silhouettesSize));
	CUDA_CHECK_RETURN(cudaMalloc((void**)&this->d_image_array,imagesSize));
	CUDA_CHECK_RETURN(cudaMalloc((void**)&this->d_CameraMatrices,MatricesSize));

	this->imageCount = imagecount;

	// allocate host memory non pinned
	/*this->h_CameraMatrices   = new glm::mat4[imagecount];

	const int sizesilhouette = imagecount * _w * _h;
	this->h_silhouette_array = new unsigned char[sizesilhouette];
	this->h_image_array      = new uchar4[sizesilhouette];*/

	// let's try use pinned memory allocation here since data is to big


	CUDA_CHECK_RETURN(cudaMallocHost((void**)&this->h_CameraMatrices , MatricesSize));
	//CUDA_CHECK_RETURN(cudaMallocHost((void**)&this->h_silhouette_array,silhouettesSize));
	//CUDA_CHECK_RETURN(cudaMallocHost((void**)&this->h_image_array ,imagesSize));

	this->w = _w;
	this->h = _h;
}

ImageDataArrays::~ImageDataArrays()
{
	// TODO Auto-generated destructor stub
}

glm::mat4 ImageDataArrays::LoadMatrice(unsigned int CameraID, char* path)
{
	glm::mat4 Projection_Matrix;
	glm::mat4 TProjection_Matrix;
	std::ifstream filep(path,std::ifstream::in);

	        //check if file opened
	        if(!filep)
	        {
	           std::cerr << " unable to open file!!!"<<std::endl;
	            exit(EXIT_FAILURE);
	        }
	        for(int i = 0; i < 3 ; i++)
	        {

	            for(int j = 0; j < 4; j++)
	            {
	                filep>>Projection_Matrix[i][j];
	            }
	        }
	        filep.close();

	        Projection_Matrix[3][0] =  0.0;
	        Projection_Matrix[3][1] =  0.0;
	        Projection_Matrix[3][2] =  0.0;
	        Projection_Matrix[3][3] =  1.0;

	        TProjection_Matrix = glm::transpose(Projection_Matrix);
	return TProjection_Matrix;
}

void ImageDataArrays::MergeMatricesData()
{

	for(unsigned int i = 0; i < this->imageCount; i++)
	{
		char* fileName = new char[255];
#ifdef TempleDirectory2
		sprintf(fileName,TempleDirectory2,i);
#endif

#ifdef DinoDirectory2
		sprintf(fileName,DinoDirectory2,i);
#endif

#ifdef DinoDirectory2_361
		sprintf(fileName,DinoDirectory2_361,i);
#endif
#ifdef ToyDirectory2
		sprintf(fileName,ToyDirectory2,i);
#endif

#ifdef MorpheuseDirectory2
		sprintf(fileName,MorpheuseDirectory2,i);
#endif
		this->h_CameraMatrices[i] = this->LoadMatrice(i,fileName);

		//upload to GPU why we are here and it's done  only one time this block was not here just to remmeber in case i want to back to my old version
		size_t MatricesSize    = this->imageCount * sizeof(glm::mat4);
		CUDA_CHECK_RETURN(cudaMemcpy(this->d_CameraMatrices,this->h_CameraMatrices,MatricesSize,cudaMemcpyHostToDevice));
		/****************************************/
		delete[]fileName;
	}
}
/****************************************************************************
 *  i'am trying to merge the data directly in the GPU to make stuff faster
 *  i got ride of silhouettes so no need for merge silhouette all this work
 *  to minimise the data transfer betewen cpu and gpu if i get around it
 *  it will be nice
 **************************************************************************/
void ImageDataArrays::MergeSilhouetteData(unsigned char* frameSilhouette,unsigned int CameraID,unsigned int imageSize)
{
	for(unsigned int j = 0; j < imageSize; j++)
	{
		this->h_silhouette_array[(CameraID*imageSize) + j] = frameSilhouette[j];
	}
}

void ImageDataArrays::MergeImageData(uchar4* frameImage,unsigned int CameraID,unsigned int imageSize)
{
	for(unsigned int j = 0; j < imageSize; j++)
	{
		this->h_image_array[(CameraID*imageSize) + j] = frameImage[j];
	}
}
/*******************************************************************************************************************/
void ImageDataArrays::UploadToGPU()
{
	/**********************************
	 * am trying to get rid of this
	 * function so we can optimize
	 *********************************/

	//size_t silhouettesSize = this->imageCount * this->w * this->h * sizeof(unsigned char);
	//size_t imagesSize      = this->imageCount * this->w * this->h * sizeof(uchar4);
	//size_t MatricesSize    = this->imageCount * sizeof(glm::mat4);

	//copying data from CPU to GPU
	//CUDA_CHECK_RETURN(cudaMemcpy(this->d_silhouette_array,this->h_silhouette_array,silhouettesSize,cudaMemcpyHostToDevice));
	//CUDA_CHECK_RETURN(cudaMemcpy(this->d_image_array,this->h_image_array,imagesSize,cudaMemcpyHostToDevice));
	//CUDA_CHECK_RETURN(cudaMemcpy(this->d_CameraMatrices,this->h_CameraMatrices,MatricesSize,cudaMemcpyHostToDevice));

	//free the  host data when it's pinned we can't use this delete
	/*delete[] this->h_CameraMatrices;
	delete[] this->h_image_array;
	delete[] this->h_silhouette_array;*/
}

void ImageDataArrays::FreeResource() // clean up
{
	cudaFree(this->d_image_array);
}


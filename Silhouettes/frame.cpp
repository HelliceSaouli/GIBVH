/*
 * frame.cpp
 *
 *  Created on: Jul 10, 2015
 *      Author: hakosoft
 */

#include "frame.h"




// kernel
extern "C"
void CallBackrgbaToSilhouetteKernel(uchar4* input,int CameraID,uchar4* ImageSilhouettes/*, unsigned char* output*/, unsigned int rows, unsigned int cols,
									dim3 blocks,dim3 grid);

frame::frame(unsigned int ID)
{
	char* fileName = new char[255];
#ifdef TempleDirectory
	sprintf(fileName,TempleDirectory,ID);
#endif

#ifdef DinoDirectory
	sprintf(fileName,DinoDirectory,ID);
#endif

#ifdef DinoDirectory361
	sprintf(fileName,DinoDirectory361,ID);
#endif

#ifdef ToyDirectory
	sprintf(fileName,ToyDirectory,ID);
#endif

#ifdef MorpheuseDirectory
	sprintf(fileName,MorpheuseDirectory,ID);
#endif

	std::string buffer(fileName);
	delete[] fileName;

	cv::Mat img;
	img = cv::imread(buffer,CV_LOAD_IMAGE_COLOR);    // read the image
    if(! img.data )                              // Check for invalid input
    {
       std::cerr <<  "Could not open or find the image...!!" << std::endl ;
       exit(0);
    }
	int row = img.rows;
	int col = img.cols;

	this->h_imageInput = new uchar4[row * col]; // page Malloc on host
	//cudaMallocHost((void**)&this->h_imageInput,row * col * sizeof(uchar4)); // pinned malloc

	// this what make take so long and make gabs betewen kernel calls
	for(int x = 0; x < row; x++ )
	{
		for(int y = 0; y < col; y++)
		{
		   cv::Vec3b intensity = img.at<cv::Vec3b>(x,y);
		   this->h_imageInput[x * col + y].z = intensity.val[0];
		   this->h_imageInput[x * col + y].y = intensity.val[1];
		   this->h_imageInput[x * col + y].x = intensity.val[2];
		   this->h_imageInput[x * col + y].w = intensity.val[3];
		}
	}

	this->w = col;
	this->h = row;
	this->CameraID = ID; // in case we need it some how
	// let's allocate data to the out put image since we know how much we have
	//this->h_imageOutput = new unsigned char[row * col];
	//cudaMallocHost((void**)&this->h_imageOutput,row * col * sizeof(unsigned char));
}


frame::~frame()
{
}

void frame::cleanFrame()
{
	delete[] h_imageInput;
	delete[] h_imageOutput;
}

unsigned int frame::getWidth()const
{
	return this->w;
}

unsigned int frame::getHeigth()const
{
	return this->h;
}

unsigned int frame::getCameraID()const
{
	return this->CameraID;
}

uchar4* frame::getImageData()
{
	return this->h_imageInput;
}

unsigned char* frame::getSilhouetteData()
{
	// i shoud check if i had the data before returning it
	return this->h_imageOutput;
}

void frame::extractSilhouetteGPU(const ImageDataArrays &ImageSilhouette)
{
	//1-create data for GPU
	uchar4*        d_imageInput  = NULL;
	//unsigned char* d_imageOutput = NULL;


	size_t inputSize  = this->w * this->h * sizeof(uchar4);
	//size_t outputSize = this->w * this->h * sizeof(unsigned char);

	//2-allocate memory in the GPU
	CUDA_CHECK_RETURN(cudaMalloc((void**)&d_imageInput,  inputSize));
	//CUDA_CHECK_RETURN(cudaMalloc((void**)&d_imageOutput,outputSize));

	//3- copying data from CPU to GPU
	CUDA_CHECK_RETURN(cudaMemcpy(d_imageInput,this->h_imageInput,inputSize,cudaMemcpyHostToDevice));

	//4-Launching the carving kernel
	dim3 block(16,16);

	dim3 grid(iDivUp(this->w,block.x),iDivUp(this->h,block.y));
	CallBackrgbaToSilhouetteKernel(d_imageInput,this->CameraID,ImageSilhouette.d_image_array/*,d_imageOutput*/,this->h ,this->w,block,grid);

	//5- get the data back
	//CUDA_CHECK_RETURN(cudaMemcpy(this->h_imageOutput, d_imageOutput, outputSize,cudaMemcpyDeviceToHost));
	//CUDA_CHECK_RETURN(cudaMemcpy(this->h_imageInput, d_imageInput, inputSize,cudaMemcpyDeviceToHost));

	//output the image
	/*cv::Mat imageGrey(this->h, this->w, CV_8UC4, (void*)this->h_imageInput);
	char* fileName = new char[255];
     sprintf(fileName,"viff%03d.png",this->CameraID);
	cv::imwrite(fileName, imageGrey);*/

	cudaFree(d_imageInput);
	//cudaFree(d_imageOutput);

}

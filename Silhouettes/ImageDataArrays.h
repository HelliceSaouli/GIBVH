/*
 * ImageDataArrays.h
 *
 *  Created on: Jul 11, 2015
 *      Author: hakosoft
 *      this will hold the information  peaces that came from the GPU put
 *      them in to one array send them back
 */

#ifndef IMAGEDATAARRAYS_H_
#define IMAGEDATAARRAYS_H_


//C++ headers
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>

//OpenCV all headers
#include "opencv2/opencv.hpp"

//CUDA headers
#include <cuda_runtime.h>

//GLM headers
#include "glm/glm.hpp"

// Utility
#include <util.h> // helps  some utility and defined macro

class ImageDataArrays
{
  public:
		// i will play it risk and let it public see what i can do later on
		unsigned char* h_silhouette_array;
		unsigned char* d_silhouette_array; // this will hold all images silhouette and it will be on GPU

		uchar4*		   h_image_array;
		uchar4*		   d_image_array;     //  this will hold all images that will be used as texture and it will be on GPU

		glm::mat4*     h_CameraMatrices;
		glm::mat4*     d_CameraMatrices;

		unsigned int w,h;
		unsigned int   imageCount;  // the number of images

		/******************************************************************************/
		ImageDataArrays(const unsigned int,unsigned int,unsigned int);
		virtual ~ImageDataArrays();

		void MergeSilhouetteData(unsigned char*,unsigned int,unsigned int);
		void MergeImageData(uchar4*,unsigned int,unsigned int);
		void UploadToGPU();

		glm::mat4 LoadMatrice(unsigned int,char *path); //load Camera Matrices from file
		void MergeMatricesData(); // save all data in host array than send it to the GPU
		void FreeResource();
};

#endif /* IMAGEDATAARRAYS_H_ */

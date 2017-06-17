/*
 * frame.h
 *
 *  Created on: Jul 10, 2015
 *      Author: hakosoft saouli
 *
 *      this class will handle images
 */

#ifndef FRAME_H_
#define FRAME_H_

//C++ headers
#include <string>
#include <iostream>
#include "ImageDataArrays.h"
//OpenCV all headers
#include "opencv2/opencv.hpp"

//CUDA headers
#include <cuda_runtime.h>

// Utility
#include <util.h> // helps  some utility and defined macro

class frame
{
	private:
		unsigned int w,h,CameraID; // width and height and camera id in this case is the number of image in file
		uchar4*        h_imageInput; // that we read from file or camera stream. it's host (CPU) data
		unsigned char* h_imageOutput; // the silhouette that we will use. it's host (CPU) data

	public:
		frame(unsigned int); // create a frame with it's camera ID
		virtual ~frame();

		// getter to catch the Data
		unsigned int getWidth()const;
		unsigned int getHeigth()const;
		unsigned int getCameraID()const;


		uchar4*        getImageData();           // colored image
		unsigned char* getSilhouetteData();		 // white and black image

		void extractSilhouetteGPU(const ImageDataArrays&); // here we pass the gpu pointer of the data to kernel so it reside there
		void cleanFrame();

		inline
		int iDivUp(int a, int b)
		{
		    return (a % b != 0) ? (a / b + 1) : (a / b);
		}

};

#endif /* FRAME_H_ */

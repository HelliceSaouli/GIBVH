/*
 * TextureGL.h
 *
 *  Created on: Jun 26, 2015
 *      Author: hakosoft
 *
 *
 *      using free image to load images :
 *      http://www.malgouyres.org/freeimagehowto
 *
 *

 * A FBO (Framebuffer object) is a target where you can render images other than the default frame buffer or screen.
 *
 * A PBO (Pixel Buffer Object) allows asynchronous transfers of pixel data to and from the device. This can be helpful to improve overall performance when rendering if you have other things that can be done while waiting for the pixel transfer.
 *
 */

#ifndef TEXTUREGL_H_
#define TEXTUREGL_H_

#include <string>
#include <iostream>
#include <FreeImage.h>

#include "GL/glew.h"
class TextureGL
{
	private:
		GLuint _mTexture; // variable that store the handle of textures
	public:
		TextureGL();
		TextureGL(const std::string&); // take the image file load it
		void BindTexture(unsigned int); // you can bind more than one texture so we need id
		virtual ~TextureGL();


		inline
		unsigned char  *loadImage(const char *filename,int &w,int &h)
		{
			  FIBITMAP *dib1 = NULL;
			  FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(filename);
			  dib1 = FreeImage_Load(fif, filename, JPEG_DEFAULT);
			  if (!dib1)
				{
				   std::cerr << "Erreur ouverture d\'image" << std::endl;
				   exit (0);

				}
			  int height, width;
			  RGBQUAD rgbquad;
			  FREE_IMAGE_TYPE type;
			  BITMAPINFOHEADER *header;
			  type = FreeImage_GetImageType(dib1);

			  height = FreeImage_GetHeight(dib1);
			  width = FreeImage_GetWidth(dib1);

			  w = width;
			  h = height;

			  header = FreeImage_GetInfoHeader(dib1);
			  int scanLineWidh = ((3*width)%4 == 0) ? 3*width : ((3*width)/4)*4+4;

			  unsigned char * texels= (GLubyte*)calloc(height*scanLineWidh, sizeof(GLubyte));
			  for(int x=0 ; x<width ; x++)
			  {
				for(int y=0 ; y<height; y++)
				{
				  FreeImage_GetPixelColor(dib1,x,y,&rgbquad);
				  texels[(y*scanLineWidh+3*x)]=((GLubyte*)&rgbquad)[2];
				  texels[(y*scanLineWidh+3*x)+1]=((GLubyte*)&rgbquad)[1];
				  texels[(y*scanLineWidh+3*x)+2]=((GLubyte*)&rgbquad)[0];
				}
			  }

			return texels;
	}


};

#endif /* TEXTUREGL_H_ */

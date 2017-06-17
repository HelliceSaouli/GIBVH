/*
 * TextureGL.cpp
 *
 *  Created on: Jun 26, 2015
 *      Author: hakosoft
 */

#include "TextureGL.h"
#include <cassert>

TextureGL::TextureGL()
{
	// TODO Auto-generated constructor stub

}

TextureGL::TextureGL(const std::string& ImageName)
{
	int w,h;
	unsigned char* ImageData = loadImage(ImageName.c_str(),w,h); // load the image

	glGenTextures(1,&this->_mTexture); 						// ask openGL nicely to generate texture for us
	glBindTexture(GL_TEXTURE_2D,this->_mTexture);			// bind it

	// some texture parameters

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // send the data to opengl
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,ImageData);
    glBindTexture(GL_TEXTURE_2D,0);
    free(ImageData);
}

TextureGL::~TextureGL() // clean up
{
	glDeleteTextures(1,&this->_mTexture);
}


void TextureGL::BindTexture(unsigned int unit)
{
	assert(unit >=0 && unit<= 31);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D,this->_mTexture);

}


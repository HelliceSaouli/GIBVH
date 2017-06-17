#include<iostream>

#include "Silhouettes/frame.h"
#include "Silhouettes/ImageDataArrays.h"
#include "Camera/vCamera.h"
#include "Transforms/transform.h"
#include "Window/Display.h"
#include "Window/Fps.h"
#include "Input/input.h"
#include "Texture/TextureGL.h"
#include "shaders/Shader.h"
#include "Geometry/Mesh.h"
#include "Transforms/transform.h"
#include "volume/volume.h"
#include "PixelBuffer/PixelBuffer.h"

#include "util.h"

int main(void)
{

	//cudaSetDevice(1);
	//1- set our 3D volume and send it to GPU
	volume _volume(10,10,10);
	//volume _volume(64,64,64);
	//volume _volume(128,128,128);
	//volume _volume(256,256,256);
	_volume.create3DArray();
	_volume.setUp3DTexture();
	_volume.createTextureObject();

	std::cout << " volume created and send to GPU !!"<<std::endl;

	//2- set our images and send the to gpu
#ifdef TempleDino47
	unsigned int images = 47; // number of images
	unsigned int w = 640;
	unsigned int h = 480;
#endif

#ifdef DinoToy
	unsigned int images = 36; // number of images
	unsigned int w = 720;
	unsigned int h = 576;
#endif

#ifdef othere
	unsigned int images = 8; // number of images
	unsigned int w = 1400;
	unsigned int h = 1300;
#endif
	//create data holder for our silhouette and their matrices
	ImageDataArrays MyData(images,w,h);
	MyData.MergeMatricesData();

	//load data
	for(unsigned int camIdx = 0; camIdx < images; camIdx++)// if we go real time this will change to while (capture 1,2,3...K != non-stop)
	{

		frame Image(camIdx);
		Image.extractSilhouetteGPU(MyData);
		//unsigned char* silhouette = Image.getSilhouetteData();
		//uchar4*        texture    = Image.getImageData();

		//unsigned int imageSize = Image.getWidth() * Image.getHeigth();
		//MyData.MergeSilhouetteData(silhouette,camIdx,imageSize);
		//MyData.MergeImageData(texture,camIdx,imageSize);
		//clean data
		//Image.cleanFrame();

	}
	//upload Data to GPU
	//MyData.UploadToGPU();

	std::cout<<" Silhouette exctracted all image data are in GPU!"<<std::endl;

	std::cout<<"Rendring.......!!"<<std::endl;
	//3- reconstruc and render
	Fps FramePerSc;
	input in;
	in.HideMouse(false);
	Display _display(800,600,"Silhouette Carving : carve using Ray csting on GPU");
	Shader _shader("shader programs/Simple");

	// uglly as shit XD
	Vertex quard[] = { Vertex(glm::vec3(-1.0,-1.0,0.0),glm::vec2(0.0,0.0)),
					   Vertex(glm::vec3( 1.0,-1.0,0.0),glm::vec2(1.0,0.0)),
			           Vertex(glm::vec3(-1.0,1.0,0.0), glm::vec2(0.0,1.0)),
			           Vertex(glm::vec3(-1.0,1.0,0.0), glm::vec2(0.0,1.0)),
			           Vertex(glm::vec3( 1.0,-1.0,0.0),glm::vec2(1.0,0.0)),
			           Vertex(glm::vec3( 1.0,1.0, 0.0),glm::vec2(1.0,1.0)),
	};

	GLint attribut[2];

	attribut[0] = _shader.GetAttributeLocation("position");
	attribut[1] = _shader.GetAttributeLocation("texCoord");

	Mesh cube(quard,sizeof(quard) / sizeof(quard[0]),attribut);
	vCamera   camera(glm::vec3(0.0,0.0,3.5),glm::vec3(0.0,0.0,0.0),60.0f,800/600,0.01f,1000.0f);
	bool isCOlor = false;
	float phi   = 0.0;
	float theta = 0.0;
	float mouseSpeed = 2.0;
	float walkspeed  = 0.5;
	glm::vec3 direction;
	glm::vec3 right;

	PixelBuffer _PBO(800,600);
	_PBO.creatPBO();
	_PBO.registerPBOToCUDA();
	_PBO.creatTexture();
	FramePerSc.Start(); // start counting fps
	while(!in.QUIT())
	{
		in.updateEvents();

		if(in.mouseMotion() && in.getMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			// calculate the new angle across the vertical and horizantal vectors
			phi   += in.getMouseY() * mouseSpeed * FramePerSc.deltatime();
			//if(phi > 90.0)
				//phi -= 90.0;

			theta += in.getMouseX() * mouseSpeed * FramePerSc.deltatime();
			//if(theta > 90.0)
			//	theta -= 90.0;

			float phiRadian   = DegreeToRad(phi);
			float thetaRadian = DegreeToRad(theta);

			 direction = glm::vec3(
			    cos(phiRadian) * sin(thetaRadian),
			    sin(phiRadian),
			    cos(phiRadian) * cos(thetaRadian)
			);

			right = glm::vec3(sin(thetaRadian - PI/2.0f), 0,cos(thetaRadian - PI/2.0f));
			glm::vec3 up = glm::cross( right, direction );
			//camera.setUpvector(glm::vec3(0.0,1.0,0.0)); // bahe nssahle 3la ro7i chwi
			camera.setUpvector(up);
			glm::vec3 target = camera.getPosition() - direction; // - is correct not +
			camera.setTarget(target);
		}

		if(in.getKey(SDL_SCANCODE_UP) == KEY_DOWN)
		{
			camera.setPosition(camera.getPosition() - direction * walkspeed * FramePerSc.deltatime());
		}

		if(in.getKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
		{
			camera.setPosition(camera.getPosition() + direction * walkspeed * FramePerSc.deltatime());
		}

		/*if(in.getKey(SDL_SCANCODE_C) == KEY_DOWN)
		{
			isCOlor = true;
		}
		if(in.getKey(SDL_SCANCODE_B) == KEY_DOWN)
		{
			isCOlor = false;
		}*/
		if(in.getKey(SDL_SCANCODE_Q) == KEY_DOWN)
		{
			 in.exit();
		}

		/*if(in.getKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
		{
			//TODO.. in case we press R we rest to initial postion
		}*/


		 camera.update();
		_PBO.render(_volume.getDeviceVolume(),MyData,camera,isCOlor,w,h);

		_PBO.fromPBOToTexture();

		_shader.bind();
			 _PBO.bindTargetTex();
			 _display.Clear(1.0f,0.0,0.0f,0.0f);
			  cube.Draw();
		_shader.deleteShader();

		FramePerSc.Count();// count
		_display.Update();
		std::cout<<"FPS :"<<(int)FramePerSc.getFPS()<<std::endl;
	}
	return 0;
}

#include "util.h"
#include "helper_math.h"
#include <cuda.h>
#include "glm/glm.hpp"


/************************************************** ray casting functions *********************************************/
typedef struct	/* a data sturctur to hold 4x4 view matrix*/
{
    float4 m[4];
} mat4;

__constant__ mat4 d_invViewMatrix;  // inverse view matrix


struct Ray					/* data sturctur to hold our ray that will be shooting*/
{
    float3 origine;   // origin
    float3 direction;   // direction
};


// intersect ray with a box  every body using this
// http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm

__device__ int intersectBox(Ray r, float3 boxmin, float3 boxmax, float *tnear, float *tfar)
{
    // compute intersection of ray with all six bbox planes
    float3 invR = make_float3(1.0f) / r.direction;
    float3 tbot = invR * (boxmin - r.origine);
    float3 ttop = invR * (boxmax - r.origine);

    // re-order intersections to find smallest and largest on each axis
    float3 tmin = fminf(ttop, tbot);
    float3 tmax = fmaxf(ttop, tbot);

    // find the largest tmin and the smallest tmax
    float largest_tmin = fmaxf(fmaxf(tmin.x, tmin.y), fmaxf(tmin.x, tmin.z));
    float smallest_tmax = fminf(fminf(tmax.x, tmax.y), fminf(tmax.x, tmax.z));

    *tnear = largest_tmin;
    *tfar = smallest_tmax;

    return smallest_tmax > largest_tmin;
}

__device__ float3 mul(const mat4 &M, const float3 &v)	/* this multiplacation will be used in raycasting calculation  since all calculation are using float3 and stuff and i'am to tired to change every thing to glm::vec and mats*/
{
    float3 r;
    r.x = dot(v, make_float3(M.m[0]));
    r.y = dot(v, make_float3(M.m[1]));
    r.z = dot(v, make_float3(M.m[2]));
    return r;
}

// transform vector by matrix with translation
__device__
float4 mul(const mat4 &M, const float4 &v)
{
    float4 r;
    r.x = dot(v, M.m[0]);
    r.y = dot(v, M.m[1]);
    r.z = dot(v, M.m[2]);
    r.w = 1.0f;
    return r;
}
__device__ uint rgbaFloatToInt(float4 rgba)	/* covert float to int */
{
    rgba.x = __saturatef(rgba.x);   // clamp to [0.0, 1.0]
    rgba.y = __saturatef(rgba.y);
    rgba.z = __saturatef(rgba.z);
    rgba.w = __saturatef(rgba.w);
    return (uint(rgba.w*255)<<24) | (uint(rgba.z*255)<<16) | (uint(rgba.y*255)<<8) | uint(rgba.x*255);
}

/**************************************** carving functions**************************************************************/
__device__ float4 MultiplicationMatrixVector(glm::mat4 Mat, glm::vec4 Vec) /* this will be used for projection in silhouette becoz all data are using glm*/
{

	glm::vec4 w(0.0);

	w = Mat * Vec;
	float4 reslut = make_float4(w.x,w.y,w.z,w.w);
	return reslut;
}
__device__ float4 Projection(glm::mat4 Mat,float x, float y, float z)
{
	float4 screen;
	glm::vec4 Vec(x,y,z,1.0);
	screen =  MultiplicationMatrixVector(Mat,Vec);

	screen.x = screen.x / screen.z;
	screen.y = screen.y / screen.z;
	//screen.z = screen.z / screen.z;
	//screen.w = screen.w / screen.z;

	return screen;
}
__device__ int Check(uchar4 *Slihouette,int i, int w,int h,int imgnbr,int u , int v)
{
	if((u>= 0  && u < w) && (v >=0 && v< h) )
	{
		int j =(v * w )+ u;
		int imgsize = w * h;
		if(Slihouette[(i*imgsize) + j].w == 1.0)
		{
			return 1;
		}
	}
	return -1;
}

__device__ float4 blendingColor(float depth1,float depth2,float4 c1,float4 c2) // this fucntion simpley blende 2 color but instead of using alpha we use depth
{
	// this function didn't work well probalby need to be tooned in and try it with someting else
	float alpha = 1- (1-depth1) * (1-depth2);

	float r =  c1.x * depth1 / alpha + c2.x * depth2 * (1 - depth1) / alpha;
	float g =  c1.y * depth1 / alpha + c2.y * depth2 * (1 - depth1) / alpha;
	float b =  c1.z * depth1 / alpha + c2.z * depth2 * (1 - depth1) / alpha;
	float a = 1.0;

	return make_float4(r,g,b,a);
}
__device__ float4 lerping(float t, float4 c1,float4 c2)// c(t) = (1 − t)c 1 + tc 2 ,t ∈ [0, 1] linear interpolation
{
	float invt = 1-t;

	float r =  invt  * c1.x + t * c2.x;
	float g =  invt  * c1.y + t * c2.y;
	float b =  invt  * c1.z + t * c2.z;
	float a =  invt  * c1.w + t * c2.w;

	return make_float4(r,g,b,a);
}

// i will use the availbal code from Nvidia samples next time when i will try to optimise i will change stuff
__global__ void raycaster(uint *d_output,cudaTextureObject_t texObject,glm::vec3 vCamPosition, int imageW, int imageH/*,unsigned char *slihouette*/,
						  glm::mat4 *camera, uchar4* images,bool isCOlor,unsigned int imageNumber, int w, int h)
{

    const int maxSteps = 1000;
    const float tstep = 0.01f;
    const float3 boxMin = make_float3(-1.0f, -1.0f, -0.55f); // lazem nahder 3liha fi papier achffa 3laahe ykrojo double  genre ranna ndorro dakle cycle par example l'intersection (0.13,0.5,0.5) ta3ti nafsse texel li tamdo l'intersection (1.0,2.0,1.5) XD
    const float3 boxMax = make_float3(1.0f, 1.0f, 0.55f);
   /* const float3 lightPosition = make_float3(150.0f,150.0f,0.0f); // this should be declared out of here  it shoud be passed as render parameter
    const float4 lightCOlor    = make_float4(1.0f,1.0f,1.0f,1.0f);*/
    float depth = 10000.0f;
  //  float3 gradient;
   // float3 lightDirection;

    int x = blockIdx.x*blockDim.x + threadIdx.x;
    int y = blockIdx.y*blockDim.y + threadIdx.y;

    if ((x >= imageW) || (y >= imageH)) return;
    d_output[y*imageW + x]  = rgbaFloatToInt(make_float4(0.1,0.1,0.75,1.0));


    float u = (x / (float) imageW)*2.0f-1.0f;
    float v = (y / (float) imageH)*2.0f-1.0f;

    // calculate eye ray in world space
    Ray eyeRay;
    eyeRay.origine = make_float3(mul(d_invViewMatrix, make_float4(vCamPosition.x,vCamPosition.y,vCamPosition.z, 1.0f)));  // use this to move the camera not the object make_float3( vCamPosition.x,vCamPosition.y,vCamPosition.z);
    eyeRay.direction = normalize(make_float3(u, v, -2.0f));
    eyeRay.direction = mul(d_invViewMatrix, eyeRay.direction);

    // find intersection with box
    float tnear, tfar;
    int hit = intersectBox(eyeRay, boxMin, boxMax, &tnear, &tfar);

    if (!hit) return;

    if (tnear < 0.0f) tnear = 0.0f;     // clamp to near plane

    // march along ray from front to back, accumulating color
    float t = tnear;
    float3 pos = eyeRay.origine + eyeRay.direction *tnear;
    float3 step = eyeRay.direction * tstep;

    float4 rgbaFinale = make_float4(0.0f,0.0f,0.0f,1.0f);
    for (int i=0; i<maxSteps; i++)
    {
        // read from 3D texture
        // remap position to [0, 1] coordinates
        float4 texel = tex3D<float4>(texObject,pos.x*0.5f+0.5f, pos.y*0.5f+0.5f, pos.z*0.5f+0.5f);
        // now that we have the texel wich represent the position of volume we wanna carve let's do the silhouette consistence test

        bool InAll;
		for(int j = 0; j< imageNumber; j++)
		{

			float4 screen  = Projection(camera[j],texel.x,  texel.y, texel.z);
			int u = (int)screen.x;
			int v = (int)screen.y;
			int check = Check(images,j,w,h,imageNumber, u , v);
			// calulate color whever  it's correct or not

			if(check == 1)
			{
				InAll = true;

				int indexer =(v * w)+ u;
				if(screen.z < depth)
					{
						float4 col = make_float4(  (float)images[(j*w*h) + indexer].x / 255.0
												  ,(float)images[(j*w*h) + indexer].y / 255.0
												  ,(float)images[(j*w*h) + indexer].z / 255.0
												 ,1.0);
							rgbaFinale = lerping(0.5,rgbaFinale,col); //lerp hurt the correct and almost correct the wrong
							depth = screen.z;
					}
			}
			else
			{
				depth = 10000.0f;
				InAll = false;
				break;
			}
		}
		if(InAll == true)
		{
			/*if(isCOlor == false)
			{
				// just give one color
				float4 rgba =  1.5 *  make_float4(0.5f,0.5f,0.5f,1.0f);
				d_output[y*imageW + x]  = rgbaFloatToInt(rgba);
				return;
			}*/
			//else
			//{

				rgbaFinale = 1.5 * (rgbaFinale); // divding the lerped color by image number gived somthing corret
				d_output[y*imageW + x]  = rgbaFloatToInt(rgbaFinale );
				return;
			//}
		 }
		/*else
		{
		    float4 rgba = make_float4(0.5,0.5,0.5,0.3);
		    d_output[y*imageW + x]  = rgbaFloatToInt(rgba);
		}*/
        t += tstep;
        /*
         * TO DO add an early termination
         */
        if (t > tfar)
        	return;
        pos += step;
    }

}


extern "C"
void CallKernelraycaster(uint *d_output,cudaTextureObject_t texObject,glm::vec3 vCamPosition, int imageW, int imageH/*,unsigned char *slihouette*/,glm::mat4 *camera, uchar4* images,bool isCOlor,unsigned int imageNumber,int w, int h,dim3 Blocks,dim3 Threads )
{
	cudaError_t err ;
	raycaster<<<Blocks,Threads>>>(d_output,texObject,vCamPosition, imageW, imageH/*,slihouette*/,camera,images,isCOlor,imageNumber,w,h);
	err = cudaGetLastError();
	if ( cudaSuccess != err)
		fprintf(stderr, "Failed (error code %s)!\n", cudaGetErrorString(err));
}

extern "C"
void copyInvViewMatrix(const float* invViewMatrix, size_t sizeofMatrix)
{
	CUDA_CHECK_RETURN(cudaMemcpyToSymbol(d_invViewMatrix, invViewMatrix, sizeofMatrix));
}


#ifndef UTIL_H_
#define UTIL_H_


#include <stdlib.h>
#include <iostream>
#include <cmath>


/****************************************************
 * uncomment one of the defines images don't forget to do the
 * sme thing in frame classe and in image array
 * and int the volume classe
 ***************************************************/

//#define TempleDino47   // this has the same size image with dino rig
#define DinoToy
//#define othere

/****************************************************
 * uncomment one of the defines to change the volume
 ***************************************************/
//#define Dinorig48
//#define Dinorig361
//#define Temple47
#define DinoToy
//#define othere

/***********************************************************************************
 * uncomment one to change the directory
 ***********************************************************************************/
//#define TempleDirectory  "resources/Data sets/Temple 47 image/visualize/%02d.png"
//#define DinoDirectory    "resources/Data sets/Dino rig/visualize/%02d.png"
//#define DinoDirectory361    "resources/Data sets/Dino rig 361/visualize/%03d.png"
#define ToyDirectory     "resources/Data sets/Toy Dinosaur/visualize/%02d.jpeg"
//#define MorpheuseDirectory "resources/Data sets/Action Figure Morpheus/visualize/%02d.jpg"


//#define TempleDirectory2  "resources/Data sets/Temple 47 image/projection/%d.txt"
//#define DinoDirectory2    "resources/Data sets/Dino rig/projection/%d.txt"
//#define DinoDirectory2_361    "resources/Data sets/Dino rig 361/projection/%d.txt"
#define ToyDirectory2     "resources/Data sets/Toy Dinosaur/projection/%d.txt"
//#define MorpheuseDirectory2 "resources/Data sets/Action Figure Morpheus/projection/%d.txt"

#define filterModes 0 // 0 mean cudaFilterModeLinear 1 cudaFilterModePoint
/*******************************************************************************
 * Some CUDA helpers

 *****************************************************************************/
#include <cuda_runtime.h>

#ifndef PI
# define PI 3.1415926535897932384626433832795                          //The number PI used in math
#endif

#ifndef LOG2
# define LOG2 0.69314718055994530941723212145818                       // The log
#endif


/* Define some helpful MACRO */
#ifndef SWAP
# define SWAP(a,b,c) ((c) = (a), (a) = (b),  (b) = (c))                  // Swapping between a and b
#endif

#ifndef MIN
#  define MIN(a,b)  ((a) > (b) ? (b) : (a))								 // the min between a and b
#endif

#ifndef MAX
#  define MAX(a,b)  ((a) < (b) ? (b) : (a))								// the max between a and b
#endif

#ifndef CLAMP
# define CLAMP(x,min,max) ( (x) < (min) ? (min) : ((x > max) ?  (max) : (x)) ) // this will help a lot for example clamping color  when add them togthere
#endif

static void CheckCudaErrorAux (const char *, unsigned, const char *, cudaError_t);
#define CUDA_CHECK_RETURN(value) CheckCudaErrorAux(__FILE__,__LINE__, #value, value)

/*************************************************************************************
 *
 *  Include  some usful inline fucntion
 *
 ************************************************************************************/
inline
static void CheckCudaErrorAux (const char *file, unsigned line, const char *statement, cudaError_t err)
{
	if (err == cudaSuccess)
		return;
	std::cerr << statement<<" returned " << cudaGetErrorString(err) << "("<<err<< ") at "<<file<<":"<<line << std::endl;
	exit (1);
}

inline
float DegreeToRad(float Angle)
{
	return (Angle  * ( PI / 180.00));
}

inline
float cotangent(float Angle)
{
	return (1/tan(Angle));
}
#endif /* UTIL_H_ */

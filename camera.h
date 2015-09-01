/*
 * cvideo.h
 *
 *  Created on: Mar 21, 2012
 *      Author: root
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <pthread.h>
#include <math.h>
#include <vector>
#include "omp.h"
#include "utility.h"
#include "cvideo.h"
#include "load_image.h"
#include "viewer.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
//#include "libfreenect_sync.h"
//#include "libfreenect.h"
#include "/root/workspace/include-files/image_processing.h"
#include "/root/workspace/include-files/utility.h"


#ifndef CAMERA_H_
#define CAMERA_H_
#define PI 3.14159265

using namespace std;

class Camera{

public:
	IplImage* rgbImage;
	IplImage* depthImage;
	IplImage* grayImage;
	IplImage* intermediateImage;
	int cameraIndex;
	unsigned int width;
	unsigned int height;
	char* rawrgbImage;
	short* rawdepthImage;
	short* dataWithDepth;

	char* backgroundImage;
	char* rawCharDepthImage;
	char* rawgrayImage;
	int** realDepthVal;
	int* realDepthVal1D;
	int autoViewVal;

//	freenect_context *f_ctx;
//	freenect_device *f_dev;
//	freenect_video_format requested_format;
//	freenect_video_format current_format;
//	freenect_resolution requested_resolution;
//	freenect_resolution current_resolution;
	pthread_mutex_t depth_mutex;
	pthread_mutex_t video_mutex;

	Camera();
	Camera(int index);
	void cleanImageMemory();
	void diffDepthTest(int* decodedBlock);
	void writeDispValToFile(Camera* data);
	void storeDispVal_block_method_2(int* decodedBlock);
	void getRealDepthValue();
	char* grabDepthImageChar();
	void grabKinectGrayImage();
	void initKinect();
//	void depth_cb(freenect_device *dev, void *v_depth, uint32_t timestamp);

	void grabKinectRGBImage();
	void grabKinectVisibleImages();
	void grabKinectDepthImage();
	void grabKinectBackgroundGrayImage();

	char* getRawRGBImage();
	short* getRawDepthImage();
	char* getRawGrayImage();

	~Camera();

private:

};


#endif /* CAMERA_H_ */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <math.h>
#include <vector>
#include "omp.h"
#include "utility.h"
#include "cvideo.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdint.h>
#include "../inc/image_processing.h"
#include "../inc/utility.h"

#define PI 3.14159265

using namespace std;

class Camera{
public:
  IplImage* rgbImage;
  IplImage* depthImage;
  IplImage* grayImage;
  IplImage* intermediateImage;
  int cameraIndex;
  uint32_t width;
  uint32_t height;
  char* rawrgbImage;
  short* rawdepthImage;
  short* dataWithDepth;

  char* backgroundImage;
  char* rawCharDepthImage;
  char* rawgrayImage;
  int** realDepthVal;
  int* realDepthVal1D;
  int autoViewVal;

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

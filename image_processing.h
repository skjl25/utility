#ifndef IP_H_
#define IP_H_

//
//extern "C" {
//  #include<jpeglib.h>	//Link with: libjpeg.lib
//  #include<jmorecfg.h>
//}

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>
//#include <tiffio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class ImageProcessing {
public:
  void saveSequenceOfIplImages(char* filelocation, int sequenceNumber, IplImage* foregroundImg);

  void convertToGrayScale(char* origImg, char* grayG2ImgChar, int width, int height);
  //IplImage* convertCharGrayArrayToIplImage(char* orgArray, int height, int width);
  IplImage* convertColorArrayToIplImage(char* orgArray, int height, int width);
  IplImage* convertColorArrayToIplImage_ext(char* orgArray, int height, int width);
  void convertIplImageToCharArray(char* returnArray, IplImage* orgImage, int height, int width);
  void convertColorIplImageToCharArray(char* returnArray, IplImage* orgImage, int height, int width);

  int getEuclideanDistance(int x1, int x2, int y1, int y2);

  IplImage* getSynthesizedImage_gray(IplImage* leftIm, IplImage* rightIm, int* dispMapMat, double alpha);
  IplImage* getSynthesizedImage_rgb(IplImage* leftIm, IplImage* rightIm, int** dispMapMat, double alpha);

  //void save_pgm(string, char*, unsigned int, unsigned int);
  char* load_pgm(string, unsigned int&, unsigned int&);
  void save_ppm(string, char*, unsigned int, unsigned int);
  char* load_ppm(string, unsigned int&, unsigned int&);
  void save_ppm_unsigned(string filename, unsigned char* data, unsigned int width, unsigned int height);
  void extractVideoSequenceToImg(char* movieFileLocation, char* saveFileLocation, char* saveFileType);

  template<class T> IplImage* convertGrayArrayToIplImage(T* orgArray, int width, int height) {
    IplImage* returnImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
    if (sizeof(T) == sizeof(int) || sizeof(T) == sizeof(short))
    {
      int index = 0;
      for (int i = 0; i < height; i++)
      {
        for (int j = 0; j < width; j++)
        {
          int pixel = orgArray[index];
          ((uchar*)(returnImg->imageData + i*returnImg->widthStep))[j] = pixel;
          index++;
        }
      }
    }
    if (sizeof(T) == sizeof(char))
    {
      int index = 0;
      for (int i = 0; i < height; i++)
      {
        for (int j = 0; j < width; j++)
        {
          char pixel = orgArray[index];
          ((uchar*)(returnImg->imageData + i*returnImg->widthStep))[j] = pixel;
          index++;
        }
      }
    }
    return returnImg;
  }

  int findAvg(IplImage* data);
  double findSdv(IplImage* data);
};

#endif

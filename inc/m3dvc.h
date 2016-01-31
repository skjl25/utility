#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <vector>
#include "omp.h"
#include "utility.h"

#ifndef M3DVC_H_
#define M3DVC_H_
#define PI 3.14159265

using namespace std;

class m3dvc {
public:

  int* encoded_depth_data;
  int* encoded_visible_data;
  int* encoded_reduced_visible_data;
  int* encoded_merge_visible_depth_data;

  int* decoded_reduced_visible_data;
  int* decoded_visible_data;
  int* reconstructed_visible_data;

  //Need to be more specific whether depth of visible
  int* decodedBlock;
  int autoViewVal;
  int* decodedData;

  m3dvc(int cameraWidth, int cameraHeight);

  //////////////////////////////////////////////////////

  //DWT implementation
  void M3DVC_visible_encode(double* dwtOrgImage);
  void M3DVC_visible_encode(int* dwtOrgImage);
  void M3DVC_visible_decode();

  int* getLowFreqArrayExt(double* dwtOrgImage, int Width, int Height);

  template<class T> int* getLowFreqArray(T* dwtOrgImage, int Width, int Height) {
    int* lowPassFilter = new int[(Width*Height) / 2];
    //creating low freq
    int j = 0;
    for (int i = 0; i < Width*Height; i = i + 2)
    {
      lowPassFilter[j] = ((int)dwtOrgImage[i] + (int)dwtOrgImage[i + 1]) / 2;
      j++;
    }
    return lowPassFilter;
  }

  template<class T> int* getHighFreqArray(T* dwtOrgImage, int* lowPassFilter, int Width, int Height) {
    int* highPassFilter = new int[(Width*Height) / 2];
    //creating low freq
    //creating high freq
    int j = 0;
    for (int i = 0; i < Width*Height; i = i + 2)
    {
      highPassFilter[j] = (int)dwtOrgImage[i] - (int)lowPassFilter[j];
      j++;
    }
    return highPassFilter;
  }

  int* getEncodedDWTExt(double* dwtOrgImage, int Width, int Height);
  void getReplicateBlocks(int compressedIndicator);
  int* getDWTBlock(int* oneMergeFilter, int Width, int Height, int compressedIndicator);

  void M3DVC_depth_decode_2();
  void M3DVC_depth_encode_2(int* realDepthVal1D);

  void mergeVisibleDepthData();
  void extractVisibleDepthData(char* receivedData);

  inline void setImageHeight(int cameraHeight) { height = cameraHeight; };
  inline void setImageWidth(int cameraWidth) { width = cameraWidth; };
  inline void setEncodedVisibleData() { encoded_visible_data = new int[width*height]; };
  inline void setReducedVisibleData() { encoded_reduced_visible_data = new int[width*height]; };
  inline void setEncodedDepthData() { encoded_depth_data = new int[(width*height) / 4]; };
  inline void setDecodedBlock() { decodedBlock = new int[height*width]; };
  inline void setEncodedMergeVisibleDepthData() { encoded_merge_visible_depth_data = new int[(width*height) / 2]; };
  inline void setDecodedVisibleData() { decoded_visible_data = new int[width*height]; };
  inline void setReconstructedVisibleData() { reconstructed_visible_data = new int[width*height]; };
  inline void setDecodedReducedVisibleData() { decoded_reduced_visible_data = new int[width*height]; };
  inline int getImageHeight() { return height; };
  inline int getImageWidth() { return width; };

  //////////////////////////////////////////////////////

  virtual ~m3dvc();

private:
  unsigned int width;
  unsigned int height;
};

#endif /* CVIDEO_H_ */

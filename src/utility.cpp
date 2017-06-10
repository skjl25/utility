/*
utiltity for utility

The MIT License (MIT)

Copyright (c) 2014-2017 Suk Kyu Lee <skjl25@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "../inc/utility.h"
#include "stdint.h"
Utility::Utility() {}
#if LINUX
void utility::startTimer() {
  gettimeofday(&tv, NULL);
  startTime = tv.tv_sec + (tv.tv_usec / 1000000.0);
}

void utility::getElapsedTime() {
  gettimeofday(&tv, NULL);
  end_time = tv.tv_sec + (tv.tv_usec / 1000000.0);
  elapsedTime = 1000.0*(end_time - startTime);
  printf("Elapsed time is %.0lf ms\n", elapsedTime);
}
void utility::getElapsedTime(char* debugMsg) {
  gettimeofday(&tv, NULL);
  end_time = tv.tv_sec + (tv.tv_usec / 1000000.0);
  elapsedTime = 1000.0*(end_time - startTime);
  printf("%s Elapsed time is %.0lf ms\n", debugMsg, elapsedTime);
}
#endif

#if WINDOWS
void Utility::startTimer() {
  start_time = clock();
}

void Utility::getElapsedTime() {
  end_time = clock();
  elapsed_time = (double)(end_time - start_time);
  printf("Elapsed time is %f ms\n", elapsed_time);
}
void Utility::getElapsedTime(char* msg_debug) {
  clock_t end_time = clock();
  elapsed_time = (double)(end_time - start_time);
  printf("%s Elapsed time is %f ms\n", msg_debug, elapsed_time);
}
#endif

double Utility::getMean(double** src, int width, int height) {
  double mean = 0;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      mean = mean + src[i][j];
    }
  }
  return mean / (width*height);
}

double Utility::getMean(double* src, int width, int height) {
  double mean = 0;
  for (int i = 0; i < width*height; i++) {
    mean = mean + src[i];
  }
  return mean / (width*height);
}

double Utility::getStd(double** src, int width, int height) {
  double mean = 0;
  double sum = 0;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      mean = mean + src[i][j];
      sum = sum + (src[i][j])*(src[i][j]);
    }
  }

  mean = mean / (width*height);
  sum = (sum / (width*height)) - mean*mean;
  sum = sqrt(sum);
  return sum;
}

double Utility::getStd(double* src, int width, int height) {
  double mean = 0;
  double sum = 0;
  for (int i = 0; i < width*height; i++) {
    mean = mean + src[i];
    sum = sum + (src[i])*(src[i]);
  }

  mean = mean / (width*height);
  sum = (sum / (width*height)) - mean*mean;
  sum = sqrt(sum);
  return sum;
}

double** Utility::convertChar1DToDouble2D(char* src, int width, int height) {
  double** dst = memset2DArray<double>(width, height);

  int index = 0;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      uint8_t val_result = (uint8_t)src[j*width + i];
      dst[i][j] = val_result*1.0;
      index++;
    }
  }
  return dst;
}

char* Utility::convertDoubleToUintChar1D(double* src, int src_length) {
  int index = 0;
  char* dst = new char[src_length*sizeof(short)];

  for (int i = 0; i < src_length; i++) {
    short val_result = 0;
    //    		printf("%f\n",src[i][j]);
    if (src[i] > 255.0)
      val_result = (short)255;
    else if (src[i] < 0.0)
      val_result = (short)0;
    else
      val_result = (short)src[i];
    memcpy(&dst[index], &val_result, sizeof(short));

    index++;
  }
  return dst;
}

int* Utility::convertCharToUintInt1D(char* src, int src_length) {
  //	int* dst=new int[src_length];
  int* dst = (int*)malloc(sizeof(int)*src_length);
  for (int i = 0; i < src_length; i++) {
    uint8_t val_result = (uint8_t)src[i];
    dst[i] = val_result;
  }
  return dst;
}

void Utility::convertCharToUintInt1DExt(char* src, int* dst,
                                        int src_length) {
  //	int* dst=new int[src_length];
  //	int* dst=(int*)malloc(sizeof(int)*src_length);
  for (int i = 0; i < src_length; i++) {
    uint8_t val_result = (uint8_t)src[i];
    dst[i] = val_result;
  }
  //	return dst;
}

char* Utility::convertDoubleToUintChar1D(double** src, int width, int height) {
  int index = 0;
  char* dst = new char[width*height*sizeof(short)];

  for (int j = 0; j < height; j++) {
    for (int i = 0; i<width; i++) {
      short val_result = 0;
      //    		printf("%f\n",src[i][j]);
      if (src[i][j]>255.0)
        val_result = (short)255;
      else if (src[i][j] < 0.0)
        val_result = (short)0;
      else
        val_result = (short)src[i][j];
      memcpy(&dst[index], &val_result, sizeof(short));
      index++;
    }
  }
  return dst;
}

void Utility::changeFileNameInOrderedSequence(char* dst_file_name, int img_seq,
                                              char* loc_folder,
                                              char* name_file_header,
                                              char* type_ext) {
  char* file_name = new char[100];
  if (img_seq < 10) {
    sprintf(file_name, "%s_0000%d", name_file_header, img_seq);
  }
  if (img_seq >= 10 && img_seq < 100) {
    sprintf(file_name, "%s_000%d", name_file_header, img_seq);
  }
  if (img_seq >= 100 && img_seq < 1000) {
    sprintf(file_name, "%s_00%d", name_file_header, img_seq);
  }
  if (img_seq >= 1000 && img_seq < 10000) {
    sprintf(file_name, "%s_0%d", name_file_header, img_seq);
  }
  if (img_seq >= 10000 && img_seq < 100000) {
    sprintf(file_name, "%s_%d", name_file_header, img_seq);
  }
  sprintf(dst_file_name, "%s%s.%s", loc_folder, file_name, type_ext);
  delete[] file_name;
}

void Utility::initWritingFile(char* loc_file) {
  //		pFile=fopen(loc_file,"w");
  myfile.open(loc_file);
}

void Utility::closeFile() {
  //		pFile=fopen(loc_file,"w");
  myfile.close();
}

void Utility::saveDatatoDatFile(char* src, int src_length, char* loc_file) {
  char* file_name = new char[100];
  sprintf(file_name, "%s.dat", loc_file);
  initWritingFile(file_name);
  for (int i = 0; i < src_length; i++) {
    //		writeToFile(src[i]);
    fprintf(pFile, "%c", src[i]);
  }
  delete[] file_name;
}

Utility::~Utility() {}
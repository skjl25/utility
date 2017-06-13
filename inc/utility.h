/*
utility header for the cosa_enc

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

#ifndef UTILITY_H_
#define UTILITY_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <fstream>
#include <vector>
#include <stdint.h>
#define WINDOWS 1
#define LINUX 0

#if WINDOWS
#include "time.h"
#endif

using namespace std;

class Utility {
public:
  ofstream myfile;
  FILE* pFile;
#if LINUX
  double start_time, end_time, elapsed_time;
#endif

#if WINDOWS
  clock_t start_time, end_time;
  double elapsed_time;
#endif

  void startTimer();
  void getElapsedTime();
  void getElapsedTime(char* msg_debug);

  //double array->Uint character array //255.0->255 double val is within the
  //unsigned char range
  char* convertDoubleToUintChar1D(double** src, int width, int height);
  char* convertDoubleToUintChar1D(double* src, int src_length);

  //char array->Double array
  double** convertChar1DToDouble2D(char* src, int width, int height);

  int* convertCharToUintInt1D(char* src, int src_length);
  void convertCharToUintInt1DExt(char* src, int* dst, int src_length);

  //Getting the avg and standard deviation of set of arrays in various forms
  double getMean(double** src, int width, int height);
  double getMean(double* src, int width, int height);
  double getStd(double** src, int width, int height);
  double getStd(double* src, int width, int height);

  //Writing File functions
  void changeFileNameInOrderedSequence(char* dst_file_name, int img_seq,
                                       char* loc_folder, char* name_file_header,
                                       char* type_ext);
  void initWritingFile(char* fileLocation);
  void closeFile();
  void saveDatatoDatFile(char* src, int src_length, char* loc_file);

  //C++ function template sample
  template <class T> T Add(T a, T b) {
    return a + b;
  }

  template<class T> void writeToFile(T data) {
    myfile << data << endl;
  }

  template<class T> T** memset2DArray(int width, int height) {
    T** dst = new T*[width];
    for (int i = 0; i < width; i++) {
      dst[i] = new T[height];
    }
    return dst;
  }

  template<class T> double* convertArrayToDouble1D(T* src, int width, int height) {
    double* dst = new double[width*height];

    if (sizeof(T) == sizeof(char)) {
      int index = 0;
      for (int i = 0; i < width*height; i++) {
        uint8_t src_val = (uint8_t)src[i];
        dst[i] = src_val*1.0;
        index++;
      }
    }
    if (sizeof(T) == sizeof(short)) {
      int index = 0;
      for (int i = 0; i < width*height; i++) {
        int src_val = src[i];
        dst[i] = src_val*1.0;
        index++;
      }
    }
    return dst;
  }

  template<class T> char* convertVectorToChar1D(vector<T> src, int src_length) {
    if (sizeof(T) == sizeof(int)) {
      int index = 0;
      char* dst = new char[src_length*sizeof(int)];

      for (int i = 0; i < src_length; i = i++) {
        memcpy(&dst[index], &src[i], sizeof(int));
        index = index + sizeof(int);
      }
      return dst;
    }
    if (sizeof(T) == sizeof(double)) {
      int index = 0;
      char* dst = new char[src_length*sizeof(double)];

      for (int i = 0; i < src_length; i = i++) {
        memcpy(&dst[index], &src[i], sizeof(double));
        index = index + sizeof(double);
      }
      return dst;
    }
  }

  template<class T> char* convertArrayToChar1D(T* src, int src_length) {
    if (sizeof(T) == sizeof(int)) {
      char* dst = new char[src_length];
      for (int i = 0; i < src_length; i++) {
        dst[i] = (uint8_t)src[i];
      }
      return dst;
    }
    if (sizeof(T) == sizeof(double)) {
      int index = 0;
      char* dst = new char[src_length*sizeof(double)];

      for (int i = 0; i < src_length; i = i++) {
        memcpy(&dst[index], &src[i], sizeof(double));
        index = index + sizeof(double);
      }
      return dst;
    }
  }

  Utility();
  virtual ~Utility();
};
#endif /* UTILITY_H_ */

#include "../include/utility.h"
#include "stdint.h"
utility::utility() {
}
#if LINUX
void utility::startTimer(){
  gettimeofday(&tv,NULL);
  startTime=tv.tv_sec+(tv.tv_usec/1000000.0);
}

void utility::getElapsedTime(){
  gettimeofday(&tv,NULL);
  endTime=tv.tv_sec+(tv.tv_usec/1000000.0);
  elapsedTime=1000.0*(endTime-startTime);
  printf("Elapsed time is %.0lf ms\n",elapsedTime);
}
void utility::getElapsedTime(char* debugMsg){
  gettimeofday(&tv,NULL);
  endTime=tv.tv_sec+(tv.tv_usec/1000000.0);
  elapsedTime=1000.0*(endTime-startTime);
  printf("%s Elapsed time is %.0lf ms\n",debugMsg,elapsedTime);
}
#endif

#if WINDOWS
void utility::startTimer() {
  startTime = clock();
}

void utility::getElapsedTime() {
  endTime = clock();
  elapsedTime = (double)(endTime - startTime);
  printf("Elapsed time is %f ms\n", elapsedTime);
}
void utility::getElapsedTime(char* debugMsg) {
  clock_t endTime = clock();
  elapsedTime = (double)(endTime - startTime);
  printf("%s Elapsed time is %f ms\n", debugMsg, elapsedTime);
}
#endif

double utility::getMean(double** doubleOrgImage, int Width, int Height) {
  double mean = 0;
  for (int i = 0; i < Width; i++)
  {
    for (int j = 0; j < Height; j++)
    {
      mean = mean + doubleOrgImage[i][j];
    }
  }
  return mean / (Width*Height);
}

double utility::getMean(double* doubleOrgImage, int Width, int Height) {
  double mean = 0;
  for (int i = 0; i < Width*Height; i++)
  {
    mean = mean + doubleOrgImage[i];
  }
  return mean / (Width*Height);
}

double utility::getStd(double** doubleOrgImage, int Width, int Height) {
  double mean = 0;
  double sum = 0;
  for (int i = 0; i < Width; i++)
  {
    for (int j = 0; j < Height; j++)
    {
      mean = mean + doubleOrgImage[i][j];
      sum = sum + (doubleOrgImage[i][j])*(doubleOrgImage[i][j]);
    }
  }

  mean = mean / (Width*Height);
  sum = (sum / (Width*Height)) - mean*mean;
  sum = sqrt(sum);
  return sum;
}

double utility::getStd(double* doubleOrgImage, int Width, int Height) {
  double mean = 0;
  double sum = 0;
  for (int i = 0; i < Width*Height; i++)
  {
    mean = mean + doubleOrgImage[i];
    sum = sum + (doubleOrgImage[i])*(doubleOrgImage[i]);
  }

  mean = mean / (Width*Height);
  sum = (sum / (Width*Height)) - mean*mean;
  sum = sqrt(sum);
  return sum;
}

double** utility::convertChar1DToDouble2D(char* orgArray, int width, int height) {
  double** resultArray = memset2DArray<double>(width, height);

  int index = 0;
  for (int i = 0; i < width; i++)
  {
    for (int j = 0; j < height; j++)
    {
      uint8_t temp = (uint8_t)orgArray[j*width + i];
      resultArray[i][j] = temp*1.0;
      index++;
    }
  }
  return resultArray;
}

char* utility::convertDoubleToUintChar1D(double* returnArray2, int arrayLength) {
  int index = 0;
  char* tempArray = new char[arrayLength*sizeof(short)];

  for (int i = 0; i < arrayLength; i++)
  {
    short tempVal = 0;
    //    		printf("%f\n",returnArray2[i][j]);
    if (returnArray2[i] > 255.0)
      tempVal = (short)255;
    else if (returnArray2[i] < 0.0)
      tempVal = (short)0;
    else
      tempVal = (short)returnArray2[i];
    memcpy(&tempArray[index], &tempVal, sizeof(short));

    index++;
  }
  return tempArray;
}

int* utility::convertCharToUintInt1D(char* orgArray, int arrayLength) {
  //	int* returnArray=new int[arrayLength];
  int* returnArray = (int*)malloc(sizeof(int)*arrayLength);
  for (int i = 0; i < arrayLength; i++)
  {
    uint8_t temp = (uint8_t)orgArray[i];
    returnArray[i] = temp;
  }
  return returnArray;
}

void utility::convertCharToUintInt1DExt(char* orgArray, int* convertArray, int arrayLength) {
  //	int* returnArray=new int[arrayLength];
  //	int* returnArray=(int*)malloc(sizeof(int)*arrayLength);
  for (int i = 0; i < arrayLength; i++)
  {
    uint8_t temp = (uint8_t)orgArray[i];
    convertArray[i] = temp;
  }
  //	return returnArray;
}

char* utility::convertDoubleToUintChar1D(double** returnArray2, int width1, int height1) {
  int index = 0;
  int Width = width1;
  int Height = height1;
  char* tempArray = new char[Width*Height*sizeof(short)];

  for (int j = 0; j < Height; j++)
  {
    for (int i = 0; i<Width; i++)
    {
      short tempVal = 0;
      //    		printf("%f\n",returnArray2[i][j]);
      if (returnArray2[i][j]>255.0)
        tempVal = (short)255;
      else if (returnArray2[i][j] < 0.0)
        tempVal = (short)0;
      else
        tempVal = (short)returnArray2[i][j];
      memcpy(&tempArray[index], &tempVal, sizeof(short));
      index++;
    }
  }
  return tempArray;
}

void utility::changeFileNameInOrderedSequence(char* returnFileName, int imageSequence, char* FolderLocation, char* FileHeaderName, char* extensionType) {
  char* FileName = new char[100];
  if (imageSequence < 10)
  {
    sprintf(FileName, "%s_0000%d", FileHeaderName, imageSequence);
  }
  if (imageSequence >= 10 && imageSequence < 100)
  {
    sprintf(FileName, "%s_000%d", FileHeaderName, imageSequence);
  }
  if (imageSequence >= 100 && imageSequence < 1000)
  {
    sprintf(FileName, "%s_00%d", FileHeaderName, imageSequence);
  }
  if (imageSequence >= 1000 && imageSequence < 10000)
  {
    sprintf(FileName, "%s_0%d", FileHeaderName, imageSequence);
  }
  if (imageSequence >= 10000 && imageSequence < 100000)
  {
    sprintf(FileName, "%s_%d", FileHeaderName, imageSequence);
  }
  sprintf(returnFileName, "%s%s.%s", FolderLocation, FileName, extensionType);
  delete[] FileName;
}
void utility::initWritingFile(char* fileLocation) {
  //		pFile=fopen(fileLocation,"w");
  myfile.open(fileLocation);
}

void utility::closeFile() {
  //		pFile=fopen(fileLocation,"w");
  myfile.close();
}

void utility::saveDatatoDatFile(char* integratedArray, int size, char* filelocation) {
  char* fileName = new char[100];
  sprintf(fileName, "%s.dat", filelocation);
  initWritingFile(fileName);
  for (int i = 0; i < size; i++)
  {
    //		writeToFile(integratedArray[i]);
    fprintf(pFile, "%c", integratedArray[i]);
  }
  delete[] fileName;
}

utility::~utility() {
}
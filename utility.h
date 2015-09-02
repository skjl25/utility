/*
 * utility.h
 *
 *  Created on: Mar 22, 2012
 *      Author: Suk Kyu Lee
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <math.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <vector>
#include "omp.h"


#ifndef UTILITY_H_
#define UTILITY_H_

using namespace std;


class utility {
public:
	ofstream myfile;
	FILE* pFile;
	struct timeval tv;
	double startTime,endTime,elapsedTime;

	void startTimer();
	void getElapsedTime();
	void getElapsedTime(char* debugMsg);
	//	double** memset2DDoubleArray(int width, int height);
	//	int** memset2DIntArray(int width, int height);

	//double array->Uint character array //255.0->255 double val is within the
	//unsigned char range
	char* convertDoubleToUintChar1D(double** returnArray2, int width1, int height1);
	char* convertDoubleToUintChar1D(double* returnArray2, int arrayLength);



	//char array->Double array
	double** convertChar1DToDouble2D(char* orgArray, int width,int height);
	//	double* convertChar1DToDouble1D(char* orgArray, int width,int height);
	//	double* convertShort1DToDouble1D(short* orgArray, int width,int height);

	int* convertCharToUintInt1D(char* orgArray, int arrayLength);
	void convertCharToUintInt1DExt(char* orgArray, int* convertArray, int arrayLength);




	//Getting the avg and standard deviation of set of arrays in various forms
	double getMean(double** doubleOrgImage,int Width, int Height);
	double getMean(double* doubleOrgImage,int Width, int Height);
	double getStd(double** doubleOrgImage,int Width, int Height);
	double getStd(double* doubleOrgImage,int Width, int Height);

	void changeFileNameInOrderedSequence(char* returnFileName, int imageSequence, char* FolderLocation, char* FileHeaderName, char* extensionType);

	//Writing File functions
	void initWritingFile(char* fileLocation);
	void closeFile();

	template<class T> void writeToFile(T data)
	{
		myfile << data << endl;
	}

	template<class T> T** memset2DArray(int width, int height)
	{
		T** resultArray=new T*[width];
		for(int i=0;i<width;i++)
		{
			resultArray[i]=new T[height];
		}
		return resultArray;
	}

	template<class T> double* convertArrayToDouble1D(T* orgArray, int width, int height)
	{
		double* resultArray=new double[width*height];

		if(sizeof(T)==sizeof(char))
		{
			int index=0;
			for(int i=0;i<width*height;i++)
			{
				uint8_t temp=(uint8_t)orgArray[i];
				resultArray[i]=temp*1.0;
				index++;
			}
		}
		if(sizeof(T)==sizeof(short))
		{
			int index=0;
			for(int i=0;i<width*height;i++)
			{
				int temp=orgArray[i];
				resultArray[i]=temp*1.0;
				index++;
			}
		}
		return resultArray;
	}


	template<class T> char* convertVectorToChar1D(vector<T> returnArray2, int arrayLength)
	{
		if(sizeof(T)==sizeof(int))
		{
			int index=0;
			char* dctResultImage=new char[arrayLength*sizeof(int)];

			//	for(int i=0;i<arrayLength;i=i+sizeof(double))
			//	{
			//		memcpy(&dctResultImage[i],&returnArray2[index],sizeof(double));
			//		index++;
			//	}

			for(int i=0;i<arrayLength;i=i++)
			{
				memcpy(&dctResultImage[index],&returnArray2[i],sizeof(int));
				index=index+sizeof(int);
			}
			return dctResultImage;
		}
		if(sizeof(T)==sizeof(double))
		{
			int index=0;
			char* dctResultImage=new char[arrayLength*sizeof(double)];

			//	for(int i=0;i<arrayLength;i=i+sizeof(double))
			//	{
			//		memcpy(&dctResultImage[i],&returnArray2[index],sizeof(double));
			//		index++;
			//	}
			for(int i=0;i<arrayLength;i=i++)
			{
				memcpy(&dctResultImage[index],&returnArray2[i],sizeof(double));
				index=index+sizeof(double);
			}
			return dctResultImage;
		}
	}

	template<class T> char* convertArrayToChar1D(T* orgArray, int arrayLength)
	{
		if(sizeof(T)==sizeof(int))
		{
			char* returnArray=new char[arrayLength];
			for(int i=0;i<arrayLength;i++)
			{
				returnArray[i]=(uint8_t)orgArray[i];
			}
			return returnArray;

		}
		if(sizeof(T)==sizeof(double))
		{
			int index=0;
			char* dctResultImage=new char[arrayLength*sizeof(double)];

			//	for(int i=0;i<arrayLength;i=i+sizeof(double))
			//	{
			//		memcpy(&dctResultImage[i],&returnArray2[index],sizeof(double));
			//		index++;
			//	}

			for(int i=0;i<arrayLength;i=i++)
			{
				memcpy(&dctResultImage[index],&orgArray[i],sizeof(double));
				index=index+sizeof(double);
			}
			return dctResultImage;
		}
	}
	void saveDatatoDatFile(char* integratedArray, int size, char* filelocation);

	utility();
	virtual ~utility();

};

#endif /* UTILITY_H_ */

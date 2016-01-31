#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include<stdint.h>
#include "../inc/common.h"
#include "../inc/define.h"
#include "../inc/cvideo.h"
#include "../inc/utility.h"
#include "../inc/image_processing.h"
#pragma warning(disable:4819)
using namespace std;
//1=DCT
//2=DWT
//2=50%
//4=25%
cvideo encoder;
unsigned int Width = 640;
unsigned int Height = 480;
double nonROIDeliver = 80.0 / 100.0;
double ROIDeliver = 100.0 / 100.0;
int imageSequence = 0;
int globalCounter = 130;
double** zigzaggedArrays = 0;

//Defines the size of the macroblock
int macroBlockSize = 4;

IplImage* colorCVImage;
IplImage* depthCVImage;
IplImage* grayCVImage;
IplImage* foregroundImg;
vector< vector<double> > vec;
//IplImage* depthCVImg;
char* fileLocationVisible = new char[100];
char* sendSequenceName = new char[100];
char* fileLocationDepth = new char[100];
cv::Mat colorImage;
cv::Mat flipped(480, 640, CV_8UC3);
cv::Mat colorArr[3];
utility util;

double Ct[mb_size][mb_size];
double C[mb_size][mb_size];
int Quantum[mb_size][mb_size];
int InputRunLength;
int OutputRunLength;

void init() {
  //4*4*number of row *number of height=number of Pixels
  //number of row *number of height=number of macro blocks
  //number of row=image height/macro block height
  //number of column=image width/macro block width
  // create macro blocks
  encoder.setImageHeight(Height);
  encoder.setImageWidth(Width);
  encoder.setMacroBlockHeight(macroBlockSize);
  encoder.setMacroBlockWidth(macroBlockSize);
  encoder.setDeliveryRate(nonROIDeliver);
  encoder.setROIDeliveryRate(ROIDeliver);
  encoder.setNumberOfColumn();
  encoder.setNumberOfRow();
  encoder.setNumberOfMacroBlocks();
  encoder.setLengthOfOneMacroString();
  encoder.setLengthOfOneMacroString_withROI();
  encoder.setMacroBHolder();
}


//static void idct4x4dc( int16_t d[16] )
//{
//	int16_t tmp[16];
//
//    for( int i = 0; i < 4; i++ )
//    {
//        int s01 = d[i*4+0] + d[i*4+1];
//        int d01 = d[i*4+0] - d[i*4+1];
//        int s23 = d[i*4+2] + d[i*4+3];
//        int d23 = d[i*4+2] - d[i*4+3];
//
//        tmp[0*4+i] = s01 + s23;
//        tmp[1*4+i] = s01 - s23;
//        tmp[2*4+i] = d01 - d23;
//        tmp[3*4+i] = d01 + d23;
//    }
//
//    for( int i = 0; i < 4; i++ )
//    {
//        int s01 = tmp[i*4+0] + tmp[i*4+1];
//        int d01 = tmp[i*4+0] - tmp[i*4+1];
//        int s23 = tmp[i*4+2] + tmp[i*4+3];
//        int d23 = tmp[i*4+2] - tmp[i*4+3];
//
//        d[i*4+0] = s01 + s23;
//        d[i*4+1] = s01 - s23;
//        d[i*4+2] = d01 - d23;
//        d[i*4+3] = d01 + d23;
//    }
//}
//
//
//static void dct4x4dc( int16_t d[16] )
//{
//	int16_t tmp[16];
//
//    for( int i = 0; i < 4; i++ )
//    {
//        int s01 = d[i*4+0] + d[i*4+1];
//        int d01 = d[i*4+0] - d[i*4+1];
//        int s23 = d[i*4+2] + d[i*4+3];
//        int d23 = d[i*4+2] - d[i*4+3];
//
//        tmp[0*4+i] = s01 + s23;
//        tmp[1*4+i] = s01 - s23;
//        tmp[2*4+i] = d01 - d23;
//        tmp[3*4+i] = d01 + d23;
//    }
//
//    for( int i = 0; i < 4; i++ )
//    {
//        int s01 = tmp[i*4+0] + tmp[i*4+1];
//        int d01 = tmp[i*4+0] - tmp[i*4+1];
//        int s23 = tmp[i*4+2] + tmp[i*4+3];
//        int d23 = tmp[i*4+2] - tmp[i*4+3];
//
//        d[i*4+0] = ( s01 + s23 + 1 ) >> 1;
//        d[i*4+1] = ( s01 - s23 + 1 ) >> 1;
//        d[i*4+2] = ( d01 - d23 + 1 ) >> 1;
//        d[i*4+3] = ( d01 + d23 + 1 ) >> 1;
//    }
////}
//#undef ZIG
//#define ZIG(i,y,x) level[i] = dct[x*4+y];
//#define ZIGDC(i,y,x) ZIG(i,y,x)
//#define ZIGZAG4_FRAME\
//    ZIGDC( 0,0,0) ZIG( 1,0,1) ZIG( 2,1,0) ZIG( 3,2,0)\
//    ZIG( 4,1,1) ZIG( 5,0,2) ZIG( 6,0,3) ZIG( 7,1,2)\
//    ZIG( 8,2,1) ZIG( 9,3,0) ZIG(10,3,1) ZIG(11,2,2)\
//    ZIG(12,1,3) ZIG(13,2,3) ZIG(14,3,2) ZIG(15,3,3)
//
//static void zigzag_scan_4x4_frame(int16_t level[16], int16_t dct[16]) {
//  ZIGZAG4_FRAME
//}

void ForwardDCT(int **input, int **output) {
  int N = mb_size;
  double temp[mb_size][mb_size];
  double temp1;
  int i;
  int j;
  int k;

  /*  MatrixMultiply( temp, input, Ct ); */
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      temp[i][j] = 0.0;
      for (k = 0; k < N; k++)
        temp[i][j] += ((int)input[i][k] - 128) *
        Ct[k][j];
    }
  }

  /*  MatrixMultiply( output, C, temp ); */
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      temp1 = 0.0;
      for (k = 0; k < N; k++)
        temp1 += C[i][k] * temp[k][j];
      output[i][j] = ROUND(temp1);
    }
  }
}
struct zigzag_8x8 {
  int row;
  int col;
} zigzag_8x8[8 * 8] =
{
  { 0, 0 },
  { 0, 1 }, { 1, 0 },
  { 2, 0 }, { 1, 1 }, { 0, 2 },
  { 0, 3 }, { 1, 2 }, { 2, 1 }, { 3, 0 },
  { 4, 0 }, { 3, 1 }, { 2, 2 }, { 1, 3 }, { 0, 4 },
  { 0, 5 }, { 1, 4 }, { 2, 3 }, { 3, 2 }, { 4, 1 }, { 5, 0 },
  { 6, 0 }, { 5, 1 }, { 4, 2 }, { 3, 3 }, { 2, 4 }, { 1, 5 }, { 0, 6 },
  { 0, 7 }, { 1, 6 }, { 2, 5 }, { 3, 4 }, { 4, 3 }, { 5, 2 }, { 6, 1 }, { 7, 0 },
  { 7, 1 }, { 6, 2 }, { 5, 3 }, { 4, 4 }, { 3, 5 }, { 2, 6 }, { 1, 7 },
  { 2, 7 }, { 3, 6 }, { 4, 5 }, { 5, 4 }, { 6, 3 }, { 7, 2 },
  { 7, 3 }, { 6, 4 }, { 5, 5 }, { 4, 6 }, { 3, 7 },
  { 4, 7 }, { 5, 6 }, { 6, 5 }, { 7, 4 },
  { 7, 5 }, { 6, 6 }, { 5, 7 },
  { 6, 7 }, { 7, 6 },
  { 7, 7 }
};

struct zigzag_4x4 {
  int row;
  int col;
} zigzag_4x4[4 * 4] = {
  { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 },
  { 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 },
  { 2, 0 }, { 2, 1 }, { 2, 2 }, { 2, 3 },
  { 3, 0 }, { 3, 1 }, { 3, 2 }, { 3, 3 }
};
void Initialize(int quality) {
  int i;
  int j;
  int N = mb_size;
  double pi = atan(1.0) * 4.0;

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      Quantum[i][j] = 1 + ((1 + i + j)  * quality);
  OutputRunLength = 0;
  InputRunLength = 0;
  for (j = 0; j < N; j++) {
    C[0][j] = 1.0 / sqrt((double)N);
    Ct[j][0] = C[0][j];
  }
  for (i = 1; i < N; i++) {
    for (j = 0; j < N; j++) {
      C[i][j] = sqrt(2.0 / N) *
        cos(pi * (2 * j + 1) * i / (2.0 * N));
      Ct[j][i] = C[i][j];
    }
  }
}
void InverseDCT(int **input, int **output) {
  int N = mb_size;
  double temp[mb_size][mb_size];
  double temp1;
  int i;
  int j;
  int k;

  /*  MatrixMultiply( temp, input, C ); */
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      temp[i][j] = 0.0;
      for (k = 0; k < N; k++)
        temp[i][j] += input[i][k] * C[k][j];
    }
  }

  /*  MatrixMultiply( output, Ct, temp ); */
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      temp1 = 0.0;
      for (k = 0; k < N; k++)
        temp1 += Ct[i][k] * temp[k][j];
      temp1 += 128.0;
      if (temp1 < 0)
        output[i][j] = 0;
      else if (temp1 > 255)
        output[i][j] = 255;
      else
        output[i][j] = (unsigned char)ROUND(temp1);
    }
  }
}
void ReadDCTData(int** input_data, int* raw_input_data) {
  int i;
  int row;
  int col;
  int N = mb_size;

  for (i = 0; i < (N * N); i++) {
    row = zigzag_4x4[i].row;
    col = zigzag_4x4[i].col;
    //row = zigzag_8x8[i].row;
    //col = zigzag_8x8[i].col;
    input_data[row][col] = raw_input_data[i] *
      Quantum[row][col];
  }
}

void WriteDCTData(int** output_data, int* result_out_put_data) {
  int i;
  int row;
  int col;
  double result;
  int N = mb_size;

  for (i = 0; i < (N * N); i++) {
    row = zigzag_4x4[i].row;
    col = zigzag_4x4[i].col;
    //row = zigzag_8x8[i].row;
    //col = zigzag_8x8[i].col;
    result = output_data[row][col] / Quantum[row][col];
    result_out_put_data[i] = ROUND(result);
  }
}
int main(int argc, char **argv) {
  init();

  ////////////////////////
  int n = mb_size;
  int** input;
  input = (int**)malloc(sizeof(int*)*n);
  for (int i = 0; i < n; i++) {
    input[i] = (int*)malloc(sizeof(int)*n);
  }
  int** output;
  output = (int**)malloc(sizeof(int*)*n);
  for (int i = 0; i < n; i++) {
    output[i] = (int*)malloc(sizeof(int)*n);
  }
  int** final_output;
  final_output = (int**)malloc(sizeof(int*)*n);
  for (int i = 0; i < n; i++) {
    final_output[i] = (int*)malloc(sizeof(int)*n);
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      input[i][j] = rand() % 255;
    }
  }
  //	for(int i=0;i<n;i++) {
  //		for(int j=0;j<n;j++) {
  //			printf("%d\n",input[i][j]);
  //		}
  //	}
  printf("-----\n");
  Initialize(0);
  int* temp_result = (int*)malloc(sizeof(int)*n*n);

  util.startTimer();
  for (int i = 0; i < 129600; i++) {
    ForwardDCT(input, output);
    WriteDCTData(output, temp_result);
  }
  util.getElapsedTime();

  ReadDCTData(output, temp_result);
  InverseDCT(output, final_output);
  printf("-----\n");
  //	for(int i=0;i<n;i++) {
  //		for(int j=0;j<n;j++) {
  //			printf("%d\n",final_output[i][j]);
  //		}
  //	}

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (input[i][j] != final_output[i][j]) {
        printf("different data! %d %d\n", input[i][j], final_output[i][j]);
        //getchar();
      } else {
        printf("%d %d\n", input[i][j], final_output[i][j]);
      }
    }
  }
  //getchar();
  ///////////////
  int threshold = encoder.getMacroBlockWidth()*encoder.getMacroBlockHeight()*encoder.getDeliveryRate();
  zigzaggedArrays = util.memset2DArray<double>(encoder.getNumberOfMacroBlocks(), threshold);
  grayCVImage = cvCreateImage(cvSize(Width, Height), IPL_DEPTH_8U, 1);

  while (1) {
    if (globalCounter == 45) {
      exit(1);
    }
    char* fileName2 = new char[100];
    sprintf(fileName2, "C:/test_set/foreground_mask_%d.pgm", globalCounter);
    foregroundImg = cvLoadImage(fileName2, 1);

    char* fileName = new char[100];
    sprintf(fileName, "C:/test_set/original_%d.pgm", globalCounter);
    colorCVImage = cvLoadImage(fileName, 1);

    cvCvtColor(colorCVImage, grayCVImage, CV_RGB2GRAY);

    //cvShowImage("depthShow", foregroundImg);
    //cvShowImage("colorCVImg", grayCVImage);
    //	cvWaitKey(1);

    /////////////////////////-for saving the file////--start
    //	globalCounter++;
    //	printf("Saved %d images\n",globalCounter);
    /////////////////////////-for saving the file////--end

    vector<int> ROIIndex = FindImportantRegion(foregroundImg, 4, 4);

    //for converting color to gray image
    //	save_pgm("/home/sklee25/Desktop/test.pgm",orgImage,Width,Height);

    encoder.setMacroBPixel_withROI_ext(grayCVImage, ROIIndex);

    encoder.getDCTArray();

    std::vector<double> dataVector;

    for (int i = 0; i < encoder.getNumberOfMacroBlocks(); i++) {
      encoder.getZigZagArray_with_ROI(zigzaggedArrays[i], i);
      if (encoder.macroBHolder[i].ROIFlag == 0) {
        int threshold = (int)(encoder.getMacroBlockWidth() * encoder.getMacroBlockHeight() * encoder.getDeliveryRate());
        for (int j = 0; j < threshold; j++) {
          dataVector.push_back(zigzaggedArrays[i][j]);
        }
      } else {
        int threshold = (int)(encoder.getMacroBlockWidth() * encoder.getMacroBlockHeight() * encoder.getROIDeliveryRate());
        for (int j = 0; j < threshold; j++) {
          dataVector.push_back(zigzaggedArrays[i][j]);
        }
      }
    }

    //for testing reducing the data size
    ///////////////////////////////////////////////////////////////////
    double min = *min_element(dataVector.begin(), dataVector.end());
    double max = *max_element(dataVector.begin(), dataVector.end());
    double reduceRatio = (abs(min) + max) / 255;
    double intermediateVal = abs(min) / reduceRatio;

    //	cout << "Min val is " << min << endl;
    //	cout << "Max val is " <<  max << endl;
    //	cout << "Reduce ratio val is " <<  reduceRatio << endl;
    //	cout << "Intermediate val is " <<  intermediateVal << endl;
    //	cout << "Min new Val " <<  min/reduceRatio+intermediateVal << endl;
    //	cout << "Max new Val " <<  max/reduceRatio+intermediateVal << endl;

    //	getchar();
    std::vector<int> quantized_data;

    //Creating the reduced data -start
    for (int i = 0; i < dataVector.size(); i++) {
      quantized_data.push_back(int(dataVector[i] / reduceRatio + intermediateVal));
    }
    //	double newmin=*min_element(testVector.begin(),testVector.end());
    //	double newmax=*max_element(testVector.begin(),testVector.end());
    //	cout << "Min new Val " <<  newmin << endl;
    //	cout << "Max new Val " <<  newmax << endl;
    //	getchar();
    //Creating the reduced data -end

    //decompress the reduced data -start
    vector<double>dataVectorTest;
    for (int i = 0; i < quantized_data.size(); i++) {
      double restoredVal = (quantized_data[i] - intermediateVal)*reduceRatio;
      dataVectorTest.push_back(restoredVal);
    }

    ////////////////////////////////////////////////////////

    cvideo decoder;
    decoder.setImageHeight(Height);
    decoder.setImageWidth(Width);
    decoder.setMacroBlockHeight(macroBlockSize);
    decoder.setMacroBlockWidth(macroBlockSize);
    decoder.setDeliveryRate(nonROIDeliver);
    decoder.setROIDeliveryRate(ROIDeliver);

    decoder.setNumberOfColumn();
    decoder.setNumberOfRow();
    decoder.setNumberOfMacroBlocks();

    decoder.setLengthOfOneMacroString();
    decoder.setLengthOfOneMacroString_withROI();
    decoder.setMacroBHolder();

    int receivedVectorSize = ROIIndex.size() + dataVectorTest.size();

    double** zigzaggedArraysReceiver = new double*[decoder.getNumberOfMacroBlocks()];

    for (int i = 0; i < decoder.getNumberOfMacroBlocks(); i++) {
      if (std::find(ROIIndex.begin(), ROIIndex.end(), i) != ROIIndex.end()) {
        zigzaggedArraysReceiver[i] = new double[(int)(decoder.getMacroBlockWidth() * decoder.getMacroBlockHeight())];
      } else {
        int size = (int)(decoder.getMacroBlockWidth() * decoder.getMacroBlockHeight()*nonROIDeliver);
        zigzaggedArraysReceiver[i] = new double[size];
      }
    }
    int indexVectorReceiver = 0;
    for (int i = 0; i < decoder.getNumberOfMacroBlocks(); i++) {
      if (std::find(ROIIndex.begin(), ROIIndex.end(), i) != ROIIndex.end()) {
        vector<double> row;
        for (int j = 0; j < (decoder.getMacroBlockWidth() * decoder.getMacroBlockHeight()*decoder.getROIDeliveryRate()); j++) {
          //					zigzaggedArraysReceiver[i][j]=dataVector[indexVectorReceiver];
          row.push_back(dataVector[indexVectorReceiver]);
          indexVectorReceiver++;
        }
        vec.push_back(row);
      } else {
        int threshold = (int)(decoder.getMacroBlockWidth() * decoder.getMacroBlockHeight()*nonROIDeliver);
        vector<double> row;
        for (int j = 0; j < threshold; j++) {
          double vectorVal = dataVector[indexVectorReceiver];
          //					zigzaggedArraysReceiver[i][j]=vectorVal;
          row.push_back(vectorVal);
          indexVectorReceiver++;
        }
        vec.push_back(row);
      }
    }

    decoder.getInverseZigZagArray_withROI(vec, ROIIndex);
    //	decoder.getInverseZigZagArray_withROI(zigzaggedArrays,ROIIndex);
    //decoder.getInverseZigZagArray(macroStrings);
    //	decoder.getInverseZigZagArray_withROI(macroStrings,ROIIndex);

    decoder.getIDCTArray();

    double** test = decoder.setMacroBlocksToOriginalImage();
    char* finalImage = 0;

    finalImage = util.convertDoubleToUintChar1D(test, decoder.getImageWidth(), decoder.getImageHeight());

    char* file_name_output=new char[100];
    sprintf(file_name_output,"C:/test_set/output/result_%f_%d.pgm",nonROIDeliver,globalCounter);
    ImageProcessing ip;
    IplImage* ipl_final=ip.convertGrayArrayToIplImage(finalImage,640,480);
    cvSaveImage(file_name_output,ipl_final);
    cout << "Done with DCT-Cut" << endl;
    globalCounter++;
    delete[] finalImage;
    vector< vector<double> >().swap(vec);
    vector<double>().swap(dataVector);
    vector<double>().swap(dataVector);
    vector<int>().swap(ROIIndex);
  }
  cvWaitKey(2);
}
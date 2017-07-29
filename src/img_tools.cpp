/*
image tools for utility library

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

#include "../inc/img_tools.h"
void ImageTools::saveSequenceOfIplImages(char* filelocation, int num_sequence,
                                           IplImage* foregroundImg) {
  char* fileName2 = new char[100];
  sprintf(fileName2, "%s_%d.pgm", filelocation, num_sequence);
  cvSaveImage(fileName2, foregroundImg);
  delete[] fileName2;
}

void ImageTools::convertToGrayScale(char* origImg, char* grayG2ImgChar,
                                      uint32_t width, uint32_t height) {
  for (uint32_t i = 0; i < width*height; i++) {
    uint32_t location = 3 * i + 1;
    grayG2ImgChar[i] = origImg[location];
  }
}

IplImage* ImageTools::convertColorArrayToIplImage(char* src, uint32_t height,
                                                  uint32_t width) {
  IplImage* returnImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
  int index = height*width;
  uint32_t offset = returnImg->widthStep;
  uint32_t num_channels = returnImg->nChannels;
  char* image_data = returnImg->imageData;
  for (uint32_t i = 0; i < height; i++) {
    for (uint32_t j = 0; j < width; j++) {
      int rVal = src[index + 0];
      int gVal = src[index + 1];
      int bVal = src[index + 2];

      ((uchar *)(image_data + i*offset))[j*num_channels + 0] = bVal; // B
      ((uchar *)(image_data + i*offset))[j*num_channels + 1] = gVal; // G
      ((uchar *)(image_data + i*offset))[j*num_channels + 2] = rVal; // R

      index = index + 3;
    }
  }
  return returnImg;
}

IplImage* ImageTools::convertColorArrayToIplImage_ext(char* src, uint32_t height,
                                                      uint32_t width) {
  IplImage* returnImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
  int idx = 0;
  uint32_t offset = returnImg->widthStep;
  uint32_t num_channels = returnImg->nChannels;
  char* image_data = returnImg->imageData;
  for (uint32_t i = height - 1; i = 0; i--) {
    for (uint32_t j = 0; j < width; j++) {
      int rVal = src[idx + 0];
      int gVal = src[idx + 1];
      int bVal = src[idx + 2];

      ((uchar *)(image_data + i*offset))[j*num_channels + 0] = bVal; // B
      ((uchar *)(image_data + i*offset))[j*num_channels + 1] = gVal; // G
      ((uchar *)(image_data + i*offset))[j*num_channels + 2] = rVal; // R

      idx = idx + 3;
    }
  }
  return returnImg;
}

void ImageTools::convertIplImageToCharArray(char* dst, IplImage* src,
                                              uint32_t height, uint32_t width) {
  uint32_t offset = src->widthStep;
  for (uint32_t i = 0; i < height; i++) {
    for (uint32_t j = 0; j < width; j++) {
      dst[i*width + j] = ((uchar*)(src->imageData + i*offset))[j];
    }
  }
  //	return dst;
}

void ImageTools::convertColorIplImageToCharArray(char* dst, IplImage* src,
                                                   uint32_t height,
                                                   uint32_t width) {
  int index = 0;
  char* image_data = src->imageData;
  uint32_t offset = src->widthStep;
  uint32_t num_channels = src->nChannels;
  for (uint32_t i = 0; i < height; i++) {
    for (uint32_t j = 0; j < width; j++) {
      int rVal = ((uchar *)(image_data + i*offset))[j*num_channels + 2];
      int gVal = ((uchar *)(image_data + i*offset))[j*num_channels + 1];
      int bVal = ((uchar *)(image_data + i*offset))[j*num_channels + 0];

      dst[index + 0] = rVal;
      dst[index + 1] = gVal;
      dst[index + 2] = bVal;

      index = index + 3;
    }
  }
}

double_t ImageTools::compute_ipl_img_sdv(IplImage* src) {
  double_t sum = 0;
  uint32_t offset = src->widthStep;
  for (int i = 0; i < src->height; i++) {
    for (int j = 0; j < src->width; j++) {
      sum = ((uchar*)(src->imageData + i*offset))[j] + sum;
    }
  }

  double_t threshMean = sum / (src->height*src->width);
  double_t threshSdv = 0;
  for (int i = 0; i < src->height; i++) {
    for (int j = 0; j < src->width; j++) {
      threshSdv = pow((((uchar*)(src->imageData + i*offset))[j] - threshMean),
        2) + threshSdv;
    }
  }
  threshSdv = sqrt(threshSdv / (src->height*src->width));
  return threshSdv;
}

int ImageTools::compute_ipl_img_avg(IplImage* src) {
  int sum = 0;
  for (int i = 0; i < src->height; i++) {
    for (int j = 0; j < src->width; j++) {
      sum = ((uchar*)(src->imageData + i*src->widthStep))[j] + sum;
    }
  }

  int threshMean = sum / (src->height*src->width);
  return threshMean;
}

double_t ImageTools::compute_euclidean_dist(int x1, int x2, int y1, int y2) {
  double_t euclidean_dist;

  double_t xSquare = pow(double_t(x1 - x2), 2);
  double_t ySquare = pow(double_t(y1 - y2), 2);
  euclidean_dist = sqrt(xSquare + ySquare);

  return euclidean_dist;
}

IplImage* ImageTools::getSynthesizedImage_gray(IplImage* img_left, IplImage* img_right,
                                               int* dispMapMat, double_t alpha) {
  int dispVal = 0;
  IplImage *iirImage = cvCreateImage(cvSize(img_right->width, img_right->height),
    IPL_DEPTH_8U, 1);
  IplImage *iImage = cvCreateImage(cvSize(img_right->width, img_right->height),
    IPL_DEPTH_8U, 1);

  int dispMapMatIndex = 0;
  for (int i = 0; i < img_right->height; i++) {
    for (int j = 0; j < img_right->width; j++) {
      int indexY = dispMapMatIndex / (img_right->width);
      int indexX = dispMapMatIndex % (img_right->width);

      int pixel = ((uchar*)(img_right->imageData + i*img_right->widthStep))[j];

      dispVal = dispMapMat[i*img_right->width + j];
      double_t temp_iirX = j + (1 - alpha)*dispVal;
      int iirX = (int)temp_iirX;

      ///////////////////////////////////////////
      if (iirX > img_right->width || iirX < 0) {
        if (iirX > img_right->width) {
          iirX = img_right->width - 1;
        } else {
          iirX = 0;
        }
      }
      ///////////////////////////////////////////
      int iirY = i;
      ((uchar*)(iirImage->imageData + iirY*iirImage->widthStep))[iirX] = pixel;
    }
  }

  for (int i = 0; i < img_left->height; i++) {
    for (int j = 0; j < img_left->width; j++) {
      //Method without left+dispVal*alpha
      //			int leftImgPixel=((uchar*)(leftImGray->imageData+i*leftImGray->widthStep))[j];

      //Method with left+dispVal*alpha
      int newX = j + (int)(alpha*dispMapMat[i*img_right->width + j]);

      ///////////////////////////////////////////
      if (newX > img_right->width || newX < 0) {
        if (newX > img_right->width) {
          newX = img_right->width - 1;
        } else {
          newX = 0;
        }
      }
      ///////////////////////////////////////////

      int leftImgPixel = ((uchar*)(img_left->imageData + i*img_left->widthStep))[newX];

      int rightImgPixel = ((uchar*)(iirImage->imageData + i*iirImage->widthStep))[j];

      if (rightImgPixel == 0) {
        ((uchar*)(iImage->imageData + i*iImage->widthStep))[j] = leftImgPixel;
      } else {
        ((uchar*)(iImage->imageData + i*iImage->widthStep))[j] = rightImgPixel;
      }
    }
  }
  cvReleaseImage(&iirImage);

  return iImage;
}

IplImage* ImageTools::getSynthesizedImage_rgb(IplImage* img_left, IplImage* img_right,
                                              int** dispMapMat, double_t alpha) {
  int dispVal = 0;
  IplImage *iirCImage = cvCreateImage(cvSize(img_right->width, img_right->height),
    IPL_DEPTH_8U, 3);
  IplImage *iilCImage = cvCreateImage(cvSize(img_right->width, img_right->height),
    IPL_DEPTH_8U, 3);
  IplImage *iCImage = cvCreateImage(cvSize(img_right->width, img_right->height),
    IPL_DEPTH_8U, 3);

#ifdef _GRAY
  for (int i = 0; i < rightImGray->height; i++) {
    for (int j = 0; j < rightImGray->width; j++) {
      int pixel = ((uchar*)(rightImGray->imageData + i*rightImGray->widthStep))[j];

      dispVal = dispMapMat[i][j];
      double_t temp_iirX = j + (1 - alpha)*dispVal;
      int iirX = (int)temp_iirX;

      int iirY = i;
      ((uchar*)(iirImage->imageData + iirY*iirImage->widthStep))[iirX] = pixel;
    }
  }
#endif

  //	getDispMapFromSynthesizedImage(iirImage,rightImGray);

  for (int i = 0; i < img_right->height; i++) {
    for (int j = 0; j < img_right->width; j++) {
      int pixelB = ((uchar *)(img_right->imageData + i*img_right->widthStep))[j*img_right->nChannels + 0]; // B
      int pixelG = ((uchar *)(img_right->imageData + i*img_right->widthStep))[j*img_right->nChannels + 1]; // G
      int pixelR = ((uchar *)(img_right->imageData + i*img_right->widthStep))[j*img_right->nChannels + 2]; // R

      dispVal = dispMapMat[i][j];
      double_t temp_iirX = j + (1 - alpha)*dispVal;
      int iirX = (int)temp_iirX;

      int iirY = i;
      ((uchar *)(iirCImage->imageData + iirY*iirCImage->widthStep))[iirX*iirCImage->nChannels + 0] = pixelB; // B
      ((uchar *)(iirCImage->imageData + iirY*iirCImage->widthStep))[iirX*iirCImage->nChannels + 1] = pixelG; // G
      ((uchar *)(iirCImage->imageData + iirY*iirCImage->widthStep))[iirX*iirCImage->nChannels + 2] = pixelR; // R
    }
  }

  for (int i = 0; i < img_left->height; i++) {
    for (int j = 0; j < img_left->width; j++) {
      //Method without left+dispVal*alpha
      //				int pixelLB=((uchar *)(img_left->imageData + i*img_left->widthStep))[j*img_left->nChannels + 0]; // B
      //				int pixelLG=((uchar *)(img_left->imageData + i*img_left->widthStep))[j*img_left->nChannels + 1]; // G
      //				int pixelLR=((uchar *)(img_left->imageData + i*img_left->widthStep))[j*img_left->nChannels + 2]; // R

      //Method with left+dispVal*alpha
      int newX = j + (int)(alpha*dispMapMat[i][j]);
      int pixelLB = ((uchar *)(img_left->imageData + i*img_left->widthStep))[newX*img_left->nChannels + 0]; // B
      int pixelLG = ((uchar *)(img_left->imageData + i*img_left->widthStep))[newX*img_left->nChannels + 1]; // G
      int pixelLR = ((uchar *)(img_left->imageData + i*img_left->widthStep))[newX*img_left->nChannels + 2]; // R

      int pixelRB = ((uchar *)(iirCImage->imageData + i*iirCImage->widthStep))[j*iirCImage->nChannels + 0]; // B
      int pixelRG = ((uchar *)(iirCImage->imageData + i*iirCImage->widthStep))[j*iirCImage->nChannels + 1]; // G
      int pixelRR = ((uchar *)(iirCImage->imageData + i*iirCImage->widthStep))[j*iirCImage->nChannels + 2]; // R

      if (pixelRB == 0 && pixelRG == 0 && pixelRR == 0) {
        //					((uchar*)(iImage->imageData+i*iImage->widthStep))[j]=leftImgPixel;
        ((uchar *)(iCImage->imageData + i*iCImage->widthStep))[j*iCImage->nChannels + 0] = pixelLB; // B
        ((uchar *)(iCImage->imageData + i*iCImage->widthStep))[j*iCImage->nChannels + 1] = pixelLG; // G
        ((uchar *)(iCImage->imageData + i*iCImage->widthStep))[j*iCImage->nChannels + 2] = pixelLR; // R
      } else {
        //					((uchar*)(iImage->imageData+i*iImage->widthStep))[j]=rightImgPixel;
        ((uchar *)(iCImage->imageData + i*iCImage->widthStep))[j*iCImage->nChannels + 0] = pixelRB; // B
        ((uchar *)(iCImage->imageData + i*iCImage->widthStep))[j*iCImage->nChannels + 1] = pixelRG; // G
        ((uchar *)(iCImage->imageData + i*iCImage->widthStep))[j*iCImage->nChannels + 2] = pixelRR; // R
      }
    }
  }

  return iCImage;
}

//void ImageTools::save_pgm(string filename, char* src, unsigned int width, unsigned int height)
//{
//	FILE* fid = fopen(filename.c_str(), "w");
//	fprintf(fid,"P5\n%u %u\n255\n", width, height);
//	fwrite(src, 1, width*height, fid);
//	fclose(fid);
//}

char* ImageTools::load_pgm(string filename, unsigned int& width,
                           unsigned int& height) {
  char * src;
  char tmpc;
  string type;
  int max;
  int at;

  ifstream ins;
  ins.open(filename.c_str(), ios::binary);
  ins >> type;
  ins >> width >> height;
  ins >> max;

  //cout<<endl<<"Type: "<<type;
  //cout<<endl<<"Dimensions: "<<width<<" x "<<height;
  //cout<<endl<<"Max value: "<<max;
  //cout<<endl;

  src = new char[width * height];

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      at = y*width + x;
      ins.read((char*)&tmpc, sizeof(char));
      src[at] = tmpc;
    }
  }

  ins.close();

  return src;
}

void read_yuv(yuv_data* src, char* input_file_name) {
  src->pInpVideo = fopen(input_file_name,"rb");
  if (!src->pInpVideo) {
    printf("error");
  }

  int nSize = src->width * src->height;

  for (int i = 0; i < src->num_frames; i++) {
    fread(src->pYFrame[i], sizeof(unsigned char), (nSize), src->pInpVideo);
    fread(src->pUFrame[i], sizeof(unsigned char), (nSize) / 4, src->pInpVideo);
    fread(src->pVFrame[i], sizeof(unsigned char), (nSize) / 4, src->pInpVideo);
  }
  fclose(src->pInpVideo);

}

void ImageTools::load_yuv_data(yuv_data* src, char* input_file_name, int num_frames,
                               int pic_height, int pic_width) {
  src->num_frames = num_frames;
  src->height = pic_height;
  src->width = pic_width;
  
  src->pYFrame = new unsigned char*[src->num_frames];
  for (int i = 0; i < src->num_frames; i++) {
    src->pYFrame[i] = new unsigned char[src->height*src->width];
  }

  src->pUFrame = new unsigned char*[src->num_frames];
  for (int i = 0; i < src->num_frames; i++) {
    src->pUFrame[i] = new unsigned char[src->height*src->width/4];
  }

  src->pVFrame = new unsigned char*[src->num_frames];
  for (int i = 0; i < src->num_frames; i++) {
    src->pVFrame[i] = new unsigned char[src->height*src->width/4];
  }

  read_yuv(src, input_file_name);
}

void ImageTools::write_yuv_data(yuv_data* src, char* output_file_name) {
  FILE *pTestOut;
  pTestOut = fopen(output_file_name, "w");
  int nSize = src->height* src->width;
  for (int i = 0; i < src->num_frames; i++) {
    fwrite(src->pYFrame[i], sizeof(unsigned char), (nSize), pTestOut);
    fwrite(src->pUFrame[i], sizeof(unsigned char), (nSize) / 4, pTestOut);
    fwrite(src->pVFrame[i], sizeof(unsigned char), (nSize) / 4, pTestOut);
  }
  fclose(pTestOut);
}

void ImageTools::save_ppm_unsigned(string filename, unsigned char* src,
                                     uint32_t width, uint32_t height) {
  FILE* fid = fopen(filename.c_str(), "w");
  fprintf(fid, "P6\n%u %u\n255\n", width, height);
  fwrite(src, 1, width*height * 3, fid);
  fclose(fid);
}

char* ImageTools::load_ppm(string filename, unsigned int& width,
                           unsigned int& height) {
  char * src;
  char tmpc;
  string type;
  int max;
  int at;
  string line;
  istringstream istr;
  int i;

  ifstream ins;
  ins.open(filename.c_str(), ios::binary);

  i = 0;
  while (i < 3) {
    getline(ins, line);
    if (line.at(0) != '#') {	//Ignore comments
      if (i == 0) {
        type = line;
      } else if (i == 1) {
        istr.clear();
        istr.str(line);
        istr >> width;
        istr >> height;
      } else if (i == 2) {
        max = atoi(line.c_str());
      }

      i++;
    }
  }

  //cout<<endl<<"Type: "<<type;
  //cout<<endl<<"Dimensions: "<<width<<" x "<<height;
  //cout<<endl<<"Max value: "<<max;
  //cout<<endl;

  src = new char[width * height * 3];

  for (uint32_t y = 0; y < height; y++) {
    for (uint32_t x = 0; x < width; x++) {
      at = y*width * 3 + x * 3;
      ins.read((char*)&tmpc, sizeof(char));
      src[at + 0] = tmpc;
      ins.read((char*)&tmpc, sizeof(char));
      src[at + 1] = tmpc;
      ins.read((char*)&tmpc, sizeof(char));
      src[at + 2] = tmpc;
    }
  }

  ins.close();

  return src;
}

void ImageTools::save_ppm(string filename, char* src,
                            uint32_t width, uint32_t height) {
  FILE* fid = fopen(filename.c_str(), "w");
  fprintf(fid, "P6\n%u %u\n255\n", width, height);
  fwrite(src, 1, width*height * 3, fid);
  fclose(fid);
}

void ImageTools::extractVideoSequenceToImg(char* movieFileLocation,
                                             char* saveFileLocation,
                                             char* saveFileType) {
  //char fileName[200];
  //sprintf(fileName, "%s.avi", movieFileLocation);
  printf("%s\n", movieFileLocation);

  CvCapture* capture = 0;
  IplImage* frame = 0;
  capture = cvCaptureFromAVI(movieFileLocation); // read AVI video
  if (!capture) {
    throw "Error when reading video file";
  }

  int counter = 0;
  while (true) {
    char buffer[200];
    sprintf(buffer, "%s_%d.%s", movieFileLocation, counter, saveFileType);

    /*  int cvGrabFrame (CvCapture* capture);
    IplImage* cvRetrieveFrame (CvCapture* capture)*/
    printf("%s\n", buffer);
    frame = cvQueryFrame(capture);
    if (!frame)
      break;
    cvSaveImage(buffer, frame);
    //cvNamedWindow( "w", CV_WINDOW_AUTOSIZE);
    //cvShowImage("w", frame);
    cvWaitKey(1);
    counter++;
  }
}
//Calculates image PSNR value
double_t ImageTools::get_image_psnr(uint8_t* img_recon, uint8_t* img_org,
                                    uint32_t width, uint32_t height) {
  double_t mse = 0.0;
  double_t mse_temp = 0.0;
  double_t psnr = 0.0;
  uint32_t idx;

  //Calculate MSE
  for (idx = 0; idx < width*height; idx++) {
    mse_temp = abs(img_recon[idx] - img_org[idx]);
    mse += mse_temp*mse_temp;
  }
  mse /= width*height;

  //Avoid division by zero
  if (mse == 0) {
    return 99.0;
  }

  //The PSNR
  psnr = 10 * log10(MAX_1 / mse);

  //Thats it.
  return psnr;
}

inline bool compare(const PCoordinate& first, const PCoordinate& second) {
  if (first.degree < second.degree)
    return true;
  else
    return false;
}

vector<PCoordinate> getDataOfImage(IplImage* edge_img, PCoordinate centroid_org_img) {
  vector<PCoordinate> vectorOne;

  PCoordinate pointData;
  for (int i = 0; i < edge_img->height; i++) {
    for (int j = 0; j < edge_img->width; j++) {
      double euclideanVal = 0.0;
      double p = 0.0;
      p = ((uchar*)(edge_img->imageData + i*edge_img->widthStep))[j];
      if (p == 255) {
        pointData.x = j;
        pointData.y = i;
        double degree = ToDegree(atan2(i - centroid_org_img.y, j - centroid_org_img.x));
        if (degree < 0) {
          degree += 360;
        }
        pointData.degree = degree;
        double diff1 = (centroid_org_img.x - pointData.x);
        double diff2 = (centroid_org_img.y - pointData.y);
        double pow1 = pow(diff1, 2);
        double pow2 = pow(diff2, 2);
        pointData.eculideanDistance = sqrt(pow1 + pow2);
        vectorOne.push_back(pointData);
      }
    }
  }
  sort(vectorOne.begin(), vectorOne.end(), compare);
  return vectorOne;
}

void dtw() {
  int** DTW = 0;
  int size_org_img = 0;
  int size_comp_img = 0;
  vector<PCoordinate> dataPoints;
  vector<PCoordinate> dataPointComparesd;

  DTW = new int*[size_org_img];
  for (int i = 0; i < size_org_img; i++) {
    DTW[i] = new int[size_comp_img];
  }
  int infini = INT_MAX;

  for (int i = 0; i < size_org_img; i++) {
    for (int j = 0; j < size_comp_img; j++) {
      DTW[i][j] = infini;
    }
  }

  DTW[0][0] = 0;
  for (int i = 1; i < size_org_img; i++) {
    for (int j = 1; j < size_comp_img; j++) {
      int cost = abs(dataPoints[i].eculideanDistance - dataPointComparesd[j].eculideanDistance);
      DTW[i][j] = cost + min(min(DTW[i - 1][j], DTW[i][j - 1]), DTW[i - 1][j - 1]);
    }
  }
}

#ifndef LOAD_IMAGE_H_
#define LOAD_IMAGE_H_

#include <stdio.h>

extern "C" {
  #include<jpeglib.h>	//Link with: libjpeg.lib
  #include<jmorecfg.h>
}

//#include <tiffio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void save_pgm(string, char*, unsigned int, unsigned int);
char* load_pgm(string, unsigned int&, unsigned int&);
void save_ppm(string, char*, unsigned int, unsigned int);
char* load_ppm(string, unsigned int&, unsigned int&);
void save_jpg(string, char*, unsigned int, unsigned int, int);
char* load_jpg(string, unsigned int&, unsigned int&, bool* GRAYSCALE=NULL);
char* load_tif(string, unsigned int&, unsigned int&);
void save_ppm_unsigned(string filename, unsigned char* data, unsigned int width, unsigned int height);

#endif

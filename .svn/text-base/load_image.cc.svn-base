#ifndef LOAD_IMAGE_CC_
#define LOAD_IMAGE_CC_

#include"load_image.h"

void save_pgm(string filename, char* data, unsigned int width, unsigned int height)
{
	FILE* fid = fopen(filename.c_str(), "w");
	fprintf(fid,"P5\n%u %u\n255\n", width, height);
	fwrite(data, 1, width*height, fid);
	fclose(fid);
}

void save_ppm_unsigned(string filename, unsigned char* data, unsigned int width, unsigned int height)
{
	FILE* fid = fopen(filename.c_str(), "w");
	fprintf(fid,"P6\n%u %u\n255\n", width, height);
	fwrite(data, 1, width*height*3, fid);
	fclose(fid);
}

char* load_pgm(string filename, unsigned int& width, unsigned int& height)
{
	char * data;
	char tmpc;
	string type;
	int max;
	int at;

	ifstream ins;
	ins.open(filename.c_str(),ios::binary);
	ins>>type;
	ins>>width>>height;
	ins>>max;

	//cout<<endl<<"Type: "<<type;
	//cout<<endl<<"Dimensions: "<<width<<" x "<<height;
	//cout<<endl<<"Max value: "<<max;
	//cout<<endl;

	data = new char[width * height];

	for(int y=0; y<height; y++){
		for(int x=0; x<width; x++){
			at = y*width + x;
			ins.read((char*) &tmpc, sizeof(char));
			data[at] = tmpc;
		}
	}

	ins.close();

	return data;
}

void save_ppm(string filename, char* data, unsigned int width, unsigned int height)
{
	FILE* fid = fopen(filename.c_str(), "w");
	fprintf(fid,"P6\n%u %u\n255\n", width, height);
	fwrite(data, 1, width*height*3, fid);
	fclose(fid);
}

char* load_ppm(string filename, unsigned int& width, unsigned int& height)
{
	char * data;
	char tmpc;
	string type;
	int max;
	int at;
	string line;
	istringstream istr;
	int i;

	ifstream ins;
	ins.open(filename.c_str(),ios::binary);

	i = 0;
	while(i < 3){
		getline(ins,line);
		if(line.at(0) != '#'){	//Ignore comments
			if(i == 0){
				type = line;
			}else if(i == 1){
				istr.clear();
				istr.str(line);
				istr>>width;
				istr>>height;
			}else if(i == 2){
				max = atoi(line.c_str());
			}

			i++;
		}
	}

	//cout<<endl<<"Type: "<<type;
	//cout<<endl<<"Dimensions: "<<width<<" x "<<height;
	//cout<<endl<<"Max value: "<<max;
	//cout<<endl;

	data = new char[width * height * 3];

	for(int y=0; y<height; y++){
		for(int x=0; x<width; x++){
			at = y*width*3 + x*3;
			ins.read((char*) &tmpc, sizeof(char));
			data[at+0] = tmpc;
			ins.read((char*) &tmpc, sizeof(char));
			data[at+1] = tmpc;
			ins.read((char*) &tmpc, sizeof(char));
			data[at+2] = tmpc;
		}
	}

	ins.close();

	return data;
}

void save_jpg(string filename, char* data, unsigned int width, unsigned int height, int quality)
{
	unsigned char * bits = (unsigned char*) data;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE * outfile;		//Target file
	JSAMPROW row_pointer[1];	//Pointer to JSAMPLE row[s]
	int row_stride = width * 3;	//Physical row width in image buffer

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	outfile = fopen(filename.c_str(),"wbc");
	jpeg_stdio_dest(&cinfo, outfile);

	//Image width and height, in pixels
	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE);	//Limit to baseline-JPEG values
	jpeg_start_compress(&cinfo, TRUE);

	//Write data from buffer
	while(cinfo.next_scanline < cinfo.image_height){
		//row_pointer[0] = &bits[(height-cinfo.next_scanline-1) * row_stride];
		row_pointer[0] = &bits[cinfo.next_scanline * row_stride];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	fclose(outfile);
	jpeg_destroy_compress(&cinfo);
}

char* load_jpg(string filename, unsigned int& width, unsigned int& height, bool* GRAYSCALE)
{
	int i;
	char * bits;
	struct jpeg_decompress_struct dinfo;
	struct jpeg_error_mgr jerr;
	FILE *  infile;
	JSAMPARRAY buffer;
	int row_stride;			//Physical row width in image buffer

	dinfo.err = jpeg_std_error(&jerr);
	infile = fopen(filename.c_str(), "rb");

	//Initlize data structures to read file
	jpeg_create_decompress(&dinfo);
	jpeg_stdio_src(&dinfo, infile);
	jpeg_read_header(&dinfo,TRUE);
	jpeg_start_decompress(&dinfo);

	width = dinfo.output_width;
	height = dinfo.output_height;
	row_stride = dinfo.output_width * dinfo.output_components;
	buffer = (*dinfo.mem->alloc_sarray)((j_common_ptr) &dinfo, JPOOL_IMAGE, row_stride, 1);

	//Read file
	if(dinfo.out_color_space == JCS_GRAYSCALE){
		if(GRAYSCALE != NULL) *GRAYSCALE = true;
		int at = 0;
		bits = new char[width * height];

		while(dinfo.output_scanline < dinfo.output_height){
			jpeg_read_scanlines(&dinfo, buffer, 1);
			for(i=0; i<width; i++){
				bits[at++] = buffer[0][i];
			}
		}
	}else{
		if(GRAYSCALE != NULL) *GRAYSCALE = false;
		int at = 0;
		bits = new char[width * height * 3];
		while(dinfo.output_scanline < dinfo.output_height){
			jpeg_read_scanlines(&dinfo, buffer, 1);
			for(i=0; i<width; i++){
				bits[at++] = buffer[0][i*3+0];
				bits[at++] = buffer[0][i*3+1];
				bits[at++] = buffer[0][i*3+2];
			}
		}
	}

	jpeg_finish_decompress(&dinfo);
	return bits;
}

//char* load_tif(string filename, unsigned int& width, unsigned int& height)
//				{
//	char* Irgb;
//	TIFFSetWarningHandler(NULL);
//	TIFF* tif = TIFFOpen(filename.c_str(), "r");
//
//	if(tif){
//		uint32 w, h;
//		size_t npixels;
//		uint32* raster;
//		int x, y, at, at3;
//		int r, g, b;
//
//		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
//		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
//		npixels = w * h;
//		raster = (uint32*)_TIFFmalloc(npixels*sizeof(uint32));
//
//		if(raster != NULL){
//			if(TIFFReadRGBAImage(tif, w, h, raster, 0)){
//				Irgb = new char[3*w*h];
//
//				for(x=0; x<w; x++){
//					for(y=0; y<h; y++){
//						at = y*w+x;
//						r = raster[at] & 0x000000ff;
//						g = (raster[at] >> 8) & 0x000000ff;
//						b = (raster[at] >> 16) & 0x000000ff;
//
//						at3 = 3*((h-y-1)*w+x);
//						Irgb[at3+0] = r;
//						Irgb[at3+1] = g;
//						Irgb[at3+2] = b;
//					}
//				}
//
//				width = w;
//				height = h;
//			}
//
//			_TIFFfree(raster);
//		}
//
//		TIFFClose(tif);
//	}
//
//	return Irgb;
//				}

#endif

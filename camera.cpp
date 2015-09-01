#include "camera.h"

using namespace std;
#define gwidth 640
#define gheight 480

ImageProcessing ip;

Camera::Camera()
{
	width=gwidth;
	height=gheight;
	rawgrayImage=new char[width*height];
	rgbImage=cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 3 );
	depthImage=cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	grayImage=cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	intermediateImage=cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	backgroundImage=new char[width*height];

	dataWithDepth=(short*)malloc(sizeof(short)*height*width);

	realDepthVal=new int*[height];
	for(int i=0;i<height;i++)
	{
		realDepthVal[i]=new int[width];
	}

	realDepthVal1D=new int[height*width];
}
Camera::Camera(int index)
{
	if(index==0)
	{
//		initKinect();
	}
	width=gwidth;
	height=gheight;
	rawgrayImage=new char[width*height];
	rgbImage=cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 3 );
	depthImage=cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	grayImage=cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	intermediateImage=cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	cameraIndex=index;

	backgroundImage=new char[width*height];

	dataWithDepth=(short*)malloc(sizeof(short)*height*width);

	realDepthVal=new int*[height];
	for(int i=0;i<height;i++)
	{
		realDepthVal[i]=new int[width];
	}

	realDepthVal1D=new int[height*width];

//	requested_format = FREENECT_VIDEO_RGB;
//	current_format = FREENECT_VIDEO_RGB;
//	requested_resolution = FREENECT_RESOLUTION_MEDIUM;
//	current_resolution = FREENECT_RESOLUTION_MEDIUM;
//
//	depth_mutex = PTHREAD_MUTEX_INITIALIZER;
//	video_mutex = PTHREAD_MUTEX_INITIALIZER;

}

void Camera::cleanImageMemory()
{
	cvReleaseImage(&rgbImage);
	cvReleaseImage(&depthImage);
	cvReleaseImage(&grayImage);
	cvReleaseImage(&intermediateImage);
}
//short* Camera::grabKinectDepthImage()
//{
//	short *depth = 0;
//	uint32_t ts;
//	if (freenect_sync_get_depth((void**)&depth, &ts, cameraIndex, FREENECT_DEPTH_11BIT) < 0)
//	{
//		printf("Kinect is not connected\n");
//		exit(1);
//	}
//	return depth;
//}

char* Camera::getRawRGBImage()
{
	return rawrgbImage;
}

short* Camera::getRawDepthImage()
{
	return rawdepthImage;
}

char* Camera::getRawGrayImage()
{
	return	rawgrayImage;
}


//void Camera::grabKinectDepthImage()
//{
////	short *depth = 0;
//	uint32_t ts;
//	if (freenect_sync_get_depth((void**)&rawdepthImage, &ts, cameraIndex, FREENECT_DEPTH_11BIT) < 0)
//	{
//		printf("Kinect is not connected\n");
//		exit(1);
//	}
////	return depth;
//}
//void Camera::depth_cb(freenect_device *dev, void *v_depth, uint32_t timestamp)
//{
//	//////////////////////////////////////////////
//	int got_depth = 0;
//	char t_gamma[2048];
//	char *depth_mid, *depth_front;
//	char *rgb_back, *rgb_mid, *rgb_front;
//	//////////////////////////////////////////////
//
//	int i;
//	uint16_t *depth = (uint16_t*)v_depth;
//
//	pthread_mutex_lock(&depth_mutex);
//	for (i=0; i<width*height; i++) {
//		int pval = t_gamma[depth[i]];
//		int lb = pval & 0xff;
//		switch (pval>>8) {
//		case 0:
//			depth_mid[3*i+0] = 255;
//			depth_mid[3*i+1] = 255-lb;
//			depth_mid[3*i+2] = 255-lb;
//			break;
//		case 1:
//			depth_mid[3*i+0] = 255;
//			depth_mid[3*i+1] = lb;
//			depth_mid[3*i+2] = 0;
//			break;
//		case 2:
//			depth_mid[3*i+0] = 255-lb;
//			depth_mid[3*i+1] = 255;
//			depth_mid[3*i+2] = 0;
//			break;
//		case 3:
//			depth_mid[3*i+0] = 0;
//			depth_mid[3*i+1] = 255;
//			depth_mid[3*i+2] = lb;
//			break;
//		case 4:
//			depth_mid[3*i+0] = 0;
//			depth_mid[3*i+1] = 255-lb;
//			depth_mid[3*i+2] = 255;
//			break;
//		case 5:
//			depth_mid[3*i+0] = 0;
//			depth_mid[3*i+1] = 0;
//			depth_mid[3*i+2] = 255-lb;
//			break;
//		default:
//			depth_mid[3*i+0] = 0;
//			depth_mid[3*i+1] = 0;
//			depth_mid[3*i+2] = 0;
//			break;
//		}
//	}
//	got_depth++;
//	pthread_mutex_unlock(&depth_mutex);
//}

//void Camera::grabKinectBackgroundGrayImage(){
//	uint32_t ts;
//	if(freenect_sync_get_video((void**)&rawrgbImage, &ts, cameraIndex, FREENECT_VIDEO_RGB)<0)
//	{
//		printf("Kinect is not connected\n");
//		exit(1);
//	}
//
//	ip.convertToGrayScale(rawrgbImage, backgroundImage, width,height);
//}
//
//
//
//char* Camera::grabDepthImageChar()
//{
//	char *depth = 0;
//	uint32_t ts;
//	if (freenect_sync_get_depth((void**)&depth, &ts, cameraIndex, FREENECT_DEPTH_11BIT) < 0)
//	{
//		printf("Kinect is not connected\n");
//		exit(1);
//	}
//	return depth;
//}


//char* Camera::grabKinectRGBImage(){
//	char* returnImage=0;
//	uint32_t ts;
//	if(freenect_sync_get_video((void**)&returnImage, &ts, cameraIndex, 	FREENECT_VIDEO_RGB )<0)
//	{
//		printf("Kinect is not connected\n");
//		exit(1);
//	}
//	//	save_ppm("/home/sklee25/Desktop/kinect_depth.ppm",returnImage,640,480);
//	//	getchar();
//	return returnImage;
//}

//void Camera::grabKinectRGBImage(){
//	uint32_t ts;
//	if(freenect_sync_get_video((void**)&rawrgbImage, &ts, cameraIndex, 	FREENECT_VIDEO_RGB )<0)
//	{
//		printf("Kinect is not connected\n");
//		exit(1);
//	}
//}
//
//void Camera::grabKinectGrayImage(){
//	uint32_t ts;
//	if(freenect_sync_get_video((void**)&rawrgbImage, &ts, cameraIndex, FREENECT_VIDEO_RGB)<0)
//	{
//		printf("Kinect is not connected\n");
//		exit(1);
//	}
//
//	ip.convertToGrayScale(rawrgbImage, rawgrayImage, width,height);
//	//	delete rgbImage;
//}
//
//void Camera::grabKinectVisibleImages()
//{
//	uint32_t ts;
//	if(freenect_sync_get_video((void**)&rawrgbImage, &ts, cameraIndex, FREENECT_VIDEO_RGB)<0)
//	{
//		printf("Kinect is not connected\n");
//		exit(1);
//	}
//	ip.convertToGrayScale(rawrgbImage, rawgrayImage, width,height);
//}

//void Camera::initKinect()
//{
//	if (freenect_init(&f_ctx, NULL) < 0) {
//		printf("freenect_init() failed\n");
//	}
//	freenect_set_log_level(f_ctx, FREENECT_LOG_DEBUG);
//	freenect_select_subdevices(f_ctx, (freenect_device_flags)(FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA));
//
//	int nr_devices = freenect_num_devices (f_ctx);
//	printf ("Number of devices found: %d\n", nr_devices);
//}

void Camera::diffDepthTest(int* decodedBlock)
{
	int widthIndex=0;
	int heightIndex=0;
	int diffCount=0;
	for(int k=0;k<height*width;k++)
	{
		//						printf("%f\n",0.1236 * tan((double)data[0].rawdepthImage[i]/ 2842.5 + 1.1863));
		double raw_depth=(double)realDepthVal1D[k];
		double raw_depth1=(double)decodedBlock[k];
		widthIndex=k%width;

		if (raw_depth < 2047)
		{
			//					fprintf(pFile,"%f,",(1.0 / (raw_depth * -0.0030711016 + 3.3309495161))*10);
			//			printf("%f,",(1.0 / (raw_depth * -0.0030711016 + 3.3309495161))*10);
			//
			//
			//			printf("--------------------%f\n",(1.0 / (raw_depth1 * -0.0030711016 + 3.3309495161))*10);
			int temp1=(int)((1.0 / (raw_depth * -0.0030711016 + 3.3309495161)))*10;
			int temp2=(int)((1.0 / (raw_depth1 * -0.0030711016 + 3.3309495161)))*10;

			if((temp1-temp2)!=0)
			{
				diffCount++;
			}
		}
		else
		{
			raw_depth=0;
			raw_depth1=0;
			int temp1=(int)((1.0 / (raw_depth * -0.0030711016 + 3.3309495161)))*10;
			int temp2=(int)((1.0 / (raw_depth1 * -0.0030711016 + 3.3309495161)))*10;

			if((temp1-temp2)!=0)
			{
				diffCount++;
			}
		}

		if(k%width==0 && k!=0)
		{
			//					fprintf(pFile,"\n");
			//			printf("\n");
			heightIndex++;
		}
	}
	printf("diff percentage is %f\n",(double)diffCount/(307200.00)*100);
	//		getchar();
}

void Camera::writeDispValToFile(Camera* data)
{
	FILE * pFile;
	pFile = fopen ("/home/sklee25/Desktop/dVal.txt","w");
	for(int i=0;i<data->height*data->width;i++)
	{
		//						printf("%f\n",0.1236 * tan((double)data[0].rawdepthImage[i]/ 2842.5 + 1.1863));
		double raw_depth=(double)data->rawdepthImage[i];
		if(i%data[0].width==0 && i!=0)
		{
			fprintf(pFile,"\n");
		}

		if (raw_depth < 2047)
		{
			fprintf(pFile,"%f,",(1.0 / (raw_depth * -0.0030711016 + 3.3309495161))*10);
		}
		else
		{
			raw_depth=0;
			fprintf(pFile,"%f,",(1.0 / (raw_depth * -0.0030711016 + 3.3309495161))*10);
		}
	}
	fclose (pFile);
}


void Camera::storeDispVal_block_method_2(int* decodedBlock)
{
	int widthIndex=0;
	int heightIndex=0;
	double valToSave=0;
	for(int k=0;k<height*width;k++)
	{
		//						printf("%f\n",0.1236 * tan((double)data[0].rawdepthImage[i]/ 2842.5 + 1.1863));
		double raw_depth=(double)decodedBlock[k];
		widthIndex=k%width;

		if (raw_depth < 2047)
		{
			//					fprintf(pFile,"%f,",(1.0 / (raw_depth * -0.0030711016 + 3.3309495161))*10);
			//						printf("%f,",(1.0 / (raw_depth * -0.0030711016 + 3.3309495161))*10);
			valToSave=(1.0 / (raw_depth * -0.0030711016 + 3.3309495161))*10;
			realDepthVal[heightIndex][widthIndex]=(int)valToSave;
		}
		else
		{
			raw_depth=0;
			valToSave=(1.0 / (raw_depth * -0.0030711016 + 3.3309495161))*10;
			realDepthVal[heightIndex][widthIndex]=(int)valToSave;
		}

		if(k%width==0 && k!=0)
		{
			//					fprintf(pFile,"\n");
			//						printf("\n");
			heightIndex++;
		}
	}
}

void Camera::getRealDepthValue()
{
	int widthIndex=0;
	int heightIndex=0;
	double valToSave=0;
	for(int k=0;k<height*width;k++)
	{
		//						printf("%f\n",0.1236 * tan((double)data[0].rawdepthImage[i]/ 2842.5 + 1.1863));
		double raw_depth=(double)rawdepthImage[k];

		if (raw_depth < 2047)
		{
			//					fprintf(pFile,"%f,",(1.0 / (raw_depth * -0.0030711016 + 3.3309495161))*10);
			//			printf("%f,",(1.0 / (raw_depth * -0.0030711016 + 3.3309495161))*10);
			valToSave=(1.0 / (raw_depth * -0.0030711016 + 3.3309495161))*10;
			realDepthVal1D[k]=(int)valToSave;
		}
		else
		{
			raw_depth=0;
			valToSave=(1.0 / (raw_depth * -0.0030711016 + 3.3309495161))*10;
			realDepthVal1D[k]=(int)valToSave;
		}

		if(k%width==0 && k!=0)
		{
			//					fprintf(pFile,"\n");
			//			printf("\n");
			heightIndex++;
		}
	}
}

Camera::~Camera()
{

}

//void initKinect()
//{
//
//	if (freenect_init(&f_ctx, NULL) < 0) {
//		printf("freenect_init() failed\n");
//	}
//	freenect_set_log_level(f_ctx, FREENECT_LOG_DEBUG);
//	freenect_select_subdevices(f_ctx, (freenect_device_flags)(FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA));
//
//	int nr_devices = freenect_num_devices (f_ctx);
//	printf ("Number of devices found: %d\n", nr_devices);
//}
//
//short* GrabKinectDepthImage(int deviceIndex)
//{
//	short *depth = 0;
//	uint32_t ts;
//	if (freenect_sync_get_depth((void**)&depth, &ts, deviceIndex, FREENECT_DEPTH_11BIT) < 0)
//	{
//		printf("Kinect is not connected\n");
//		exit(1);
//	}
//	return depth;
//}
//
//char* GrabKinectRGBImage(int deviceIndex)
//{
//	char* returnImage=0;
//	uint32_t ts;
//	if(freenect_sync_get_video((void**)&returnImage, &ts, deviceIndex, 	FREENECT_VIDEO_RGB )<0)
//	{
//		printf("Kinect is not connected\n");
//		exit(1);
//	}
//	//	save_ppm("/home/sklee25/Desktop/kinect_depth.ppm",returnImage,640,480);
//	//	getchar();
//	return returnImage;
//}
//
//char* GrabKinectGrayImage(int deviceIndex)
//{
//	char* rgbImage=0;
//	char* grayImage=0;
//	uint32_t ts;
//	if(freenect_sync_get_video((void**)&rgbImage, &ts, deviceIndex, FREENECT_VIDEO_RGB)<0)
//	{
//		printf("Kinect is not connected\n");
//		exit(1);
//	}
//	grayImage=convertToGrayScale(rgbImage,gwidth,gheight);
//	//	delete rgbImage;
//	return grayImage;
//}




#ifndef DEFINE_H_
#define DEFINE_H_

#define image_sequence 130
#define CompressionAlgorithm 1

#define SAMPLE_XML_PATH "/root/workspace/head-detector/src/SamplesConfig.xml"
#define POSE_TO_USE "Psi"

//#define GL_WIN_SIZE_X 720
#define GL_WIN_SIZE_X 640
#define GL_WIN_SIZE_Y 480
#define XN_CALIBRATION_FILE_NAME "UserCalibration.bin"
#define CHECK_RC(nRetVal, what)										\
		if (nRetVal != XN_STATUS_OK)									\
                        		{																\
			printf("%s failed: %s\n", what, xnGetStatusString(nRetVal));\
			return nRetVal;												\
                        		}
#define ROUND( a )      ( ( (a) < 0 ) ? (int) ( (a) - 0.5 ) : \
		(int) ( (a) + 0.5 ) )

#define mb_size 4

#endif /* DEFINE_H_ */

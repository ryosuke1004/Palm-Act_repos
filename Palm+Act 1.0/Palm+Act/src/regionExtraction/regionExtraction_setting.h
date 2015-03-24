#pragma once

#include "regionExtraction_color.h"
#include "../capture/capture.h"
#include "../interfaces/keyboardFunc.h"

namespace REGION_EXTRACTION{
	static cv::Mat_<cv::Vec3b> oriImage;
	static cv::Mat_<uchar> channel1;
	static cv::Mat_<uchar> channel2;
	static cv::Mat_<uchar> channel3;
	static int thrLow1=0;
	static int thrLow2=0;
	static int thrLow3=0;
	static int thrHigh1=255;
	static int thrHigh2=255;
	static int thrHigh3=255;
	static int morphologyType1;
	static int morphologyType2;
	static int morphologyType3;
	static int morphologyType;
	static int morphologyNum1;
	static int morphologyNum2;
	static int morphologyNum3;
	static int morphologyNum;
	
	bool settingPrm(CAPTURE::Capture* _capture, REGION_EXTRACTION::RegionExtraction_multiColor& _RegionExtraction, int _code, unsigned int _resizeRatio=100);
};
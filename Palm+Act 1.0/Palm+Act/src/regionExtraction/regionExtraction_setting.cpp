#pragma once
#include "regionExtraction_setting.h"

namespace REGION_EXTRACTION{
bool REGION_EXTRACTION::settingPrm(CAPTURE::Capture* _cap, REGION_EXTRACTION::RegionExtraction_multiColor& _regionExtraction, int _code, unsigned int _resizeRatio)
{
	thrLow1 = _regionExtraction.getRePtr(0)->getThrLow();
	thrHigh1 = _regionExtraction.getRePtr(0)->getThrHigh();
	morphologyType1 = _regionExtraction.getRePtr(0)->getMorphology().type;
	morphologyNum1 = _regionExtraction.getRePtr(0)->getMorphology().num;
	thrLow2 = _regionExtraction.getRePtr(1)->getThrLow();
	thrHigh2 = _regionExtraction.getRePtr(1)->getThrHigh();
	morphologyType2 = _regionExtraction.getRePtr(1)->getMorphology().type;
	morphologyNum2 = _regionExtraction.getRePtr(1)->getMorphology().num;
	thrLow3 = _regionExtraction.getRePtr(2)->getThrLow();
	thrHigh3 = _regionExtraction.getRePtr(2)->getThrHigh();
	morphologyType3 = _regionExtraction.getRePtr(2)->getMorphology().type;
	morphologyNum3 = _regionExtraction.getRePtr(2)->getMorphology().num;

	morphologyType = _regionExtraction.getMorphology().type;
	morphologyNum = _regionExtraction.getMorphology().num;

	char windowName[3][128];
	strcpy(windowName[0],"channel1");
	strcpy(windowName[1],"channel2");
	strcpy(windowName[2],"channel3");

	bool stopFlg = false;

	cv::Mat tImage;
	tImage=_cap->capture();
	if(tImage.empty())	return false;
	cv::cvtColor(tImage,oriImage,_code);
	cv::resize(oriImage,oriImage,cv::Size(tImage.size().width*_resizeRatio*0.01,tImage.size().height*_resizeRatio*0.01));

	cv::namedWindow(_regionExtraction.getModuleName());
	cv::namedWindow(windowName[0]);
	cv::namedWindow(windowName[1]);
	cv::namedWindow(windowName[2]);
	cv::imshow(_regionExtraction.getModuleName(),tImage);
	cv::imshow(windowName[0],tImage);
	cv::imshow(windowName[1],tImage);
	cv::imshow(windowName[2],tImage);


	while(1){
		if(KeyboardFunc::getIsCharPushed('q'))	break;
		if(KeyboardFunc::getIsCharPushed('s')){
			if(stopFlg)	stopFlg = false;
			else		stopFlg = true;
		}

		//cv::createTrackbar("1_low",_regionExtraction.getModuleName(),&thrLow1,255);
		//cv::createTrackbar("1_high",_regionExtraction.getModuleName(),&thrHigh1,255);
		//cv::createTrackbar("2_low",_regionExtraction.getModuleName(),&thrLow2,255);
		//cv::createTrackbar("2_high",_regionExtraction.getModuleName(),&thrHigh2,255);
		//cv::createTrackbar("3_low",_regionExtraction.getModuleName(),&thrLow3,255);
		//cv::createTrackbar("3_high",_regionExtraction.getModuleName(),&thrHigh3,255);
		//cv::createTrackbar("1_mTyep",_regionExtraction.getModuleName(),&morphologyType1,2);
		//cv::createTrackbar("1_mNum",_regionExtraction.getModuleName(),&morphologyNum1,20);
		//cv::createTrackbar("2_mTyep",_regionExtraction.getModuleName(),&morphologyType2,2);
		//cv::createTrackbar("2_mNum",_regionExtraction.getModuleName(),&morphologyNum2,20);
		//cv::createTrackbar("3_mTyep",_regionExtraction.getModuleName(),&morphologyType3,2);
		//cv::createTrackbar("3_mNum",_regionExtraction.getModuleName(),&morphologyNum3,20);
		//cv::createTrackbar("morphologyType",_regionExtraction.getModuleName(),&morphologyType,2);
		//cv::createTrackbar("morphologyNum",_regionExtraction.getModuleName(),&morphologyNum,20);

		cv::createTrackbar("1_low",windowName[0],&thrLow1,255);
		cv::createTrackbar("1_high",windowName[0],&thrHigh1,255);
		cv::createTrackbar("2_low",windowName[1],&thrLow2,255);
		cv::createTrackbar("2_high",windowName[1],&thrHigh2,255);
		cv::createTrackbar("3_low",windowName[2],&thrLow3,255);
		cv::createTrackbar("3_high",windowName[2],&thrHigh3,255);
		cv::createTrackbar("1_mTyep",windowName[0],&morphologyType1,2);
		cv::createTrackbar("1_mNum",windowName[0],&morphologyNum1,20);
		cv::createTrackbar("2_mTyep",windowName[1],&morphologyType2,2);
		cv::createTrackbar("2_mNum",windowName[1],&morphologyNum2,20);
		cv::createTrackbar("3_mTyep",windowName[2],&morphologyType3,2);
		cv::createTrackbar("3_mNum",windowName[2],&morphologyNum3,20);
		cv::createTrackbar("morphologyType",_regionExtraction.getModuleName(),&morphologyType,2);
		cv::createTrackbar("morphologyNum",_regionExtraction.getModuleName(),&morphologyNum,20);

		_regionExtraction.getRePtr(0)->setThrLow((unsigned int)thrLow1);
		_regionExtraction.getRePtr(0)->setThrHigh((unsigned int)thrHigh1);
		_regionExtraction.getRePtr(0)->setMorphology(MORPHOLOGY(morphologyType1), morphologyNum1);
		_regionExtraction.getRePtr(1)->setThrLow((unsigned int)thrLow2);
		_regionExtraction.getRePtr(1)->setThrHigh((unsigned int)thrHigh2);
		_regionExtraction.getRePtr(1)->setMorphology(MORPHOLOGY(morphologyType2), morphologyNum2);
		_regionExtraction.getRePtr(2)->setThrLow((unsigned int)thrLow3);
		_regionExtraction.getRePtr(2)->setThrHigh((unsigned int)thrHigh3);
		_regionExtraction.getRePtr(2)->setMorphology(MORPHOLOGY(morphologyType3), morphologyNum3);

		_regionExtraction.setMorphology(MORPHOLOGY(morphologyType),morphologyNum);

		if(!stopFlg) tImage = _cap->capture();
		if(tImage.empty())	break;
		cv::cvtColor(tImage,oriImage,_code);
		cv::resize(oriImage,oriImage,cv::Size(tImage.size().width*_resizeRatio*0.01,tImage.size().height*_resizeRatio*0.01));
		_regionExtraction.getResult(oriImage);
		cv::Mat allChannel = _regionExtraction.getResultImage();
		cv::Mat channel1 = _regionExtraction.getRePtr(0)->getResultImage();
		cv::Mat channel2 = _regionExtraction.getRePtr(1)->getResultImage();
		cv::Mat channel3 = _regionExtraction.getRePtr(2)->getResultImage();
		cv::imshow(windowName[0],channel1);
		cv::imshow(windowName[1],channel2);
		cv::imshow(windowName[2],channel3);

		cv::resize(allChannel,allChannel,cv::Size(tImage.size().width/4,tImage.size().height/4));
		cv::resize(channel1,channel1,cv::Size(tImage.size().width/4,tImage.size().height/4));
		cv::resize(channel2,channel2,cv::Size(tImage.size().width/4,tImage.size().height/4));
		cv::resize(channel3,channel3,cv::Size(tImage.size().width/4,tImage.size().height/4));

		cv::Mat hImage1;
		cv::Mat hImage2;
		cv::hconcat(channel1,channel2,hImage1);
		cv::hconcat(channel3,allChannel,hImage2);

		cv::Mat resultImage;
		cv::hconcat(hImage1,hImage2,resultImage);

		cv::imshow(_regionExtraction.getModuleName(),resultImage);
		cv::waitKey(1);
	}
	cv::destroyWindow(_regionExtraction.getModuleName());
	cv::destroyWindow(windowName[0]);
	cv::destroyWindow(windowName[1]);
	cv::destroyWindow(windowName[2]);
	printf("OK!\n\n");
	cv::waitKey(2000);
}

}
#pragma once
#include "capture_cam.h"

namespace CAPTURE_CAM{
Capture_cam::Capture_cam()
{
	this->flgStarted=false;
}
Capture_cam::~Capture_cam()
{
	this->endCapture();
}

bool Capture_cam::startCapture(unsigned int _sourceNo)
{
	this->flgStarted = false;

	this->cap = cv::VideoCapture(_sourceNo);
	if(!this->cap.isOpened()){
		puts("Error: startCapture");
		puts("- This video cannot be opened!");
		return this->flgStarted = false;
	}
	else{
		this->sourceNo = _sourceNo;
		cv::Mat frame;
		cap>>frame;

		this->info.rows = frame.size().width;
		this->info.cols = frame.size().height;
		this->info.channels = frame.channels();
		this->info.dataSize=this->info.rows*this->info.cols*this->info.channels;
		//this->info.fps = cap.get(CV_CAP_PROP_FPS);
		this->info.fps = 15;
		this->info.brightness = cap.get(CV_CAP_PROP_BRIGHTNESS);
		this->info.contrast = cap.get(CV_CAP_PROP_CONTRAST);
		this->info.saturation = cap.get(CV_CAP_PROP_SATURATION);
		this->info.hue = cap.get(CV_CAP_PROP_HUE);
		this->info.gain = cap.get(CV_CAP_PROP_GAIN);
		this->info.exposure = cap.get(CV_CAP_PROP_EXPOSURE);
		this->flgStarted = true;

		return this->initCapture();
	}
}
bool Capture_cam::initCapture()
{
	if(!this->isStarted()){
		puts("error: initCapture");
		puts("- This has not been started!");
		return false;
	}
	
	this->timer.setStartTime();
	return this->flgStarted = cap.isOpened();
}
cv::Mat Capture_cam::capture(unsigned char* _buf)
{
	if(!this->isStarted()){
		puts("error: capture");
		puts("- This has not been started!");
		return cv::Mat();
	}

	timer.setEndTime();
	while(timer.getDeltaTime() < (1.0/this->info.fps)){
		timer.setEndTime();
	}
	cv::Mat frame;
	this->cap >> frame;
	memcpy(_buf,frame.data,sizeof(unsigned char)*this->info.dataSize);
	timer.setStartTime();

	return frame;
}
cv::Mat Capture_cam::capture()
{
	if(!this->isStarted()){
		puts("error: capture");
		puts("- This has not been started!");
		return cv::Mat();
	}

	timer.setEndTime();
	while(timer.getDeltaTime() < (1.0/this->info.fps)){
		timer.setEndTime();
	}
	cv::Mat frame;
	this->cap>>frame;
	timer.setStartTime();

	return frame;
}
bool Capture_cam::showInfo()
{
	if(!this->isStarted()){
		puts("error: showInfo");
		puts("- This has not been started!");
		return false;
	}

	printf("==============================\n");
	printf(" Capture_cam\n");
	printf("------------------------------\n");
	printf(" size:\t(%d,%d)\n",this->info.rows,this->info.cols);
	printf(" channels:\t%d\n",this->info.channels);
	printf(" fps:\t%d\n",this->info.fps);
	printf(" brightness:\t%f\n",this->info.brightness);
	printf(" contrast:\t%f\n",this->info.contrast);
	printf(" saturation:\t%f\n",this->info.saturation);
	printf(" hue:\t%f\n",this->info.hue);
	printf(" gain:\t%f\n",this->info.gain);
	printf(" exposure:\t%f\n",this->info.exposure);
	printf("==============================\n");
	puts("");

	return true;
}
bool Capture_cam::endCapture()
{
	if(!this->isStarted()){
		puts("error: endCapture");
		puts("- This has not been started!");
		return false;
	}

	this->cap.release();
	this->flgStarted=false;

	return true;
}

bool Capture_cam::isStarted()
{
	return this->flgStarted;
}
void* Capture_cam::getCaptureInfo(unsigned int _CAP_INFO)
{
	if(!this->isStarted()){
		puts("Error: getCaptureInfo");
		puts("- This has not been started!");
		return 0;
	}
	switch(_CAP_INFO){
	case CAP_INFO::ROWS:
		return &this->info.rows;
	case CAP_INFO::COLS:
		return &this->info.cols;
	case CAP_INFO::CHANNELS:
		return &this->info.channels;
	case CAP_INFO::DATASIZE:
		return &this->info.dataSize;
	case CAP_INFO::FPS:
		return &this->info.fps;
	default:
		puts("Warning: getCaptureInfo");
		puts("- This pareter can not be accessed!");
		return 0;
	}
}
}
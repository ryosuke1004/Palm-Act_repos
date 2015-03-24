#pragma once

#include "capture_avi.h"

namespace CAPTURE_AVI{
Capture_avi::Capture_avi(){
	this->flgStarted=false;
}
Capture_avi::~Capture_avi(){
	this->endCapture();
}

bool Capture_avi::startCapture(unsigned int _sourceNo)
{
    //現在のディレクトリへのパーティションがあるかどうか確認
	FILE* tempFile = fopen("data/input/test.txt", "w+");
	if (tempFile == NULL)
	{
		puts("Error: startCapture");
		puts("- Failed to create file in current folder.  Please check permissions.");
		return false;
	}
	fclose(tempFile);
	remove("data/input/test.txt");
	
	this->flgStarted = false;
	char fileName[64];
	sprintf(fileName,"data/input/video(%d).avi",_sourceNo);
	this->cap = cv::VideoCapture(fileName);
	if(!this->cap.isOpened()){
		puts("Error: startCapture");
		puts("- This video cannot be opened!");
		return false;
	}
	else{
		cv::Mat frame;
		cap>>frame;
		strcpy(this->info.name,fileName);
		this->info.rows = frame.size().width;
		this->info.cols = frame.size().height;
		this->info.channels = frame.channels();
		this->info.dataSize=this->info.rows*this->info.cols*this->info.channels;
		this->info.fps = cap.get(CV_CAP_PROP_FPS);
		this->info.frames = cap.get(CV_CAP_PROP_FRAME_COUNT);
		this->flgStarted = true;

		return this->initCapture();
	}
}
bool Capture_avi::initCapture()
{
	if(!this->isStarted()){
		puts("error: initCapture");
		puts("- This has not been started!");
		return false;
	}
	
	this->flgStarted=false;
	this->cap = cv::VideoCapture(this->info.name);
	this->timer.setStartTime();
	return this->flgStarted = cap.isOpened();
}
cv::Mat Capture_avi::capture(unsigned char* _buf)
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
cv::Mat Capture_avi::capture()
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
bool Capture_avi::showInfo()
{
	if(!this->isStarted()){
		puts("error: showInfo");
		puts("- This has not been started!");
		return false;
	}

	printf("==============================\n");
	printf(" Capture_avi\n");
	printf("------------------------------\n");
	printf(" name:\t%s\n",this->info.name);
	printf(" size:\t(%d,%d)\n",this->info.rows,this->info.cols);
	printf(" channels:\t%d\n",this->info.channels);
	printf(" fps:\t%d\n",this->info.fps);
	printf(" frames:\t%d\n",this->info.frames);
	printf("==============================\n");
	puts("");

	return true;
}
bool Capture_avi::endCapture()
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

bool Capture_avi::isStarted()
{
	return this->flgStarted;
}
void* Capture_avi::getCaptureInfo(unsigned int _CAP_INFO)
{
	if(!this->isStarted()){
		puts("Error: getCameraInfo");
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
	case CAP_INFO::FRAMES:
		return &this->info.frames;
	default:
		puts("Warning: getCameraInfo");
		puts("- This pareter can not be accessed!");
		return 0;
	}
}

}
#include "regionExtraction_color.h"
#include <fstream>

namespace REGION_EXTRACTION{

void initDisplay(int val, void* ptr)
{
	cv::destroyWindow("result image");
	cv::namedWindow("result image");
}

/*=== class : RegionExtraction_color ===*/
RegionExtraction_color::RegionExtraction_color(
	REGION_EXTRACTION_METHOD _REGION_EXTRACTION_METHOD,
	int _thrLow,
	int _thrHigh,
	MORPHOLOGY _type,
	unsigned int _num)
{
	this->moduleName = std::string("newRegionExtraction_color");
	this->method = _REGION_EXTRACTION_METHOD;
	this->thrLow = _thrLow;
	this->thrHigh = _thrHigh;
	this->morphology.type = _type;
	this->morphology.num = _num;

	this->pixelsTtl = 0;
	this->center = cv::Point2i(0,0);
}
RegionExtraction_color::~RegionExtraction_color()
{
}

bool RegionExtraction_color::loadModule(std::string _moduleName)
{
	char fileName[128];
	std::sprintf(fileName,"%s.csv",_moduleName.c_str());
	std::ifstream ifs(fileName,std::ios::in);
	if(!ifs.is_open()){
		puts("Error: loadModule");
		puts("- This file cannot be opened!");
		return false;
	}

	char lineData[128];
	std::string token;
	ifs.getline(lineData,128);
	std::istringstream stream( lineData );

	this->moduleName = _moduleName;

	getline(stream,token,',');
	this->method = (REGION_EXTRACTION_METHOD)atoi(token.c_str());

	getline(stream,token,',');
	this->thrLow = atoi(token.c_str());

	getline(stream,token,',');
	this->thrHigh = atoi(token.c_str());

	getline(stream,token,',');
	if(atoi(token.c_str()) == OPENING)
		this->morphology.type = OPENING;
	else if(atoi(token.c_str()) == CLOSING)
		this->morphology.type = CLOSING;

	getline(stream,token,',');
	this->morphology.num = atoi(token.c_str());

	return true;
}
bool RegionExtraction_color::saveModule(std::string _moduleName)
{
	char fileName[128];
	std::sprintf(fileName,"%s.csv",_moduleName.c_str());
	std::ofstream ofs(fileName);
	if(!ofs.is_open()){
		puts("Error: loadModule");
		puts("- This file cannot be opened!");
		return false;
	}

	ofs <<(int)this->method<<","
		<<this->thrLow<<","
		<<this->thrHigh<<","
		<<this->morphology.type<<","
		<<this->morphology.num;

	return true;
}
cv::Mat RegionExtraction_color::getResult(cv::Mat_<uchar> _inputImage)
{
	this->resultImage = cv::Mat(_inputImage.size().height,_inputImage.size().width,CV_8UC1);

	switch(this->method){
	case THRESHOLD:
		cv::threshold(_inputImage, this->resultImage, this->thrHigh+1, 255, CV_THRESH_TOZERO_INV);
		cv::threshold(this->resultImage, this->resultImage, this->thrLow, 255, CV_THRESH_BINARY);
		break;

	case THRESHOLD_INV:
		cv::threshold(_inputImage, this->resultImage, this->thrHigh+1, 255, CV_THRESH_TOZERO_INV);
		cv::threshold(this->resultImage, this->resultImage, this->thrLow, 255, CV_THRESH_BINARY);
		cv::bitwise_not(this->resultImage,this->resultImage);
		break;

	case OTSU:
		cv::threshold(_inputImage,this->resultImage,0,255,CV_THRESH_OTSU);
		break;

	case OTSU_INV:
		cv::threshold(_inputImage,this->resultImage,0,255,CV_THRESH_OTSU);
		cv::bitwise_not(this->resultImage, this->resultImage);
		break;
	}

	if(this->morphology.type == OPENING){
		cv::dilate(this->resultImage,this->resultImage,cv::Mat(),cv::Point(),this->morphology.num);
		cv::erode(this->resultImage,this->resultImage,cv::Mat(),cv::Point(),this->morphology.num);
	}
	else if(this->morphology.type == CLOSING){
		cv::erode(this->resultImage,this->resultImage,cv::Mat(),cv::Point(),this->morphology.num);
		cv::dilate(this->resultImage,this->resultImage,cv::Mat(),cv::Point(),this->morphology.num);
	}
	cv::Moments moment = cv::moments(resultImage);
	this->pixelsTtl = (int)moment.m00;

	if(moment.m00>0){
		this->center = cv::Point2i((int)(moment.m10/moment.m00), (int)(moment.m01/moment.m00));
	}

	return this->resultImage;
}
void RegionExtraction_color::showInfo()
{
	printf("======== RegionExtraction_color ===========\n");
	printf("=== moduleName\t: %s\n",this->moduleName.c_str());
	printf("=== thr_low\t: %d\n",this->thrLow);
	printf("=== thr_high\t: %d\n",this->thrHigh);
	printf("===========================================\n\n");
}

void RegionExtraction_color::setMethod(REGION_EXTRACTION_METHOD _method){this->method = method;}
void RegionExtraction_color::setThrLow(unsigned int _thrLow){this->thrLow = _thrLow;}
void RegionExtraction_color::setThrHigh(unsigned int _thrHigh){this->thrHigh = _thrHigh;}
void RegionExtraction_color::setMorphology(MORPHOLOGY _type, unsigned int _num){this->morphology.type = _type; this->morphology.num = _num;}
unsigned int RegionExtraction_color::getThrLow(){return this->thrLow;}
unsigned int RegionExtraction_color::getThrHigh(){return this->thrHigh;}
std::string RegionExtraction_color::getModuleName(){return this->moduleName;}
Morphology RegionExtraction_color::getMorphology(){return this->morphology;}
cv::Mat_<uchar> RegionExtraction_color::getResultImage(){return this->resultImage;}
unsigned int RegionExtraction_color::getPixelsTtl(){return this->pixelsTtl;}
cv::Point2i RegionExtraction_color::getCenter(){return this->center;}


/*=== class : RegionExtraction_multiColor ===*/
RegionExtraction_multiColor::RegionExtraction_multiColor(std::string _moduleName)
{
	if(_moduleName.empty()){
		this->moduleName = std::string("newRegionExtraction_multiColor");
		this->morphology.type = MORPHOLOGY::OFF;
		this->morphology.num = 0;
	}
	else{
		if(!loadModule(_moduleName)){
			puts("Error: RegionExtraction_multiColor");
			puts("- This module cannnot be opened");
		}
	}
	this->pixelsTtl = 0;
	this->center = cv::Point2i(0,0);
}
RegionExtraction_multiColor::~RegionExtraction_multiColor()
{
}

bool RegionExtraction_multiColor::loadModule(std::string _moduleName)
{
	char fileName[128];
	this->moduleName = _moduleName;
	std::sprintf(fileName,"%s.csv",_moduleName.c_str());
	std::ifstream ifs(fileName,std::ios::in);
	if(!ifs.is_open()){
		puts("Error: loadModule");
		puts("- This File cannot be opened!");
		return false;
	}

	for(int i=0; i<3; i++){
		std::sprintf(fileName,"%s(%d)",_moduleName.c_str(),i);
		if(!this->RE[i].loadModule(fileName)){
			puts("Error: loadModule");
			printf("- %dth Module cannot be opened!",i+1);
			return false;
		}
	}

	char lineData[128];
	std::string token;
	ifs.getline(lineData,128);
	std::istringstream stream( lineData );

	getline(stream,token,',');
	if(atoi(token.c_str()) == OPENING)
		this->morphology.type = OPENING;
	else if(atoi(token.c_str()) == CLOSING)
		this->morphology.type = CLOSING;

	getline(stream,token,',');
	this->morphology.num = atoi(token.c_str());

	return true;
}
bool RegionExtraction_multiColor::saveModule(std::string _moduleName)
{
	char fileName[128];
	std::sprintf(fileName,"%s.csv",_moduleName.c_str());
	std::ofstream ofs(fileName);
	if(!ofs.is_open()){
		puts("Error: saveModule");
		puts("- This ofstream cannot be opened");
		return false;
	}

	for(int i=0; i<3; i++){
		std::sprintf(fileName,"%s(%d)",_moduleName.c_str(),i);
		if(!this->RE[i].saveModule(fileName)){
			puts("Error: saveModule");
			printf("- %dth Module cannot be saved!",i+1);
			return false;
		}
	}

	ofs<<this->morphology.type<<","<<this->morphology.num;

	return true;
}
cv::Mat RegionExtraction_multiColor::getResult(cv::Mat_<cv::Vec3b> _inputImage)
{
	this->resultImage = cv::Mat(_inputImage.size().height,_inputImage.size().width,CV_8UC1,255);
	std::vector<cv::Mat> planes;
	cv::split(_inputImage,planes);

	cv::Mat resultImages[4];	
	for(int i=0; i<3; i++){
		resultImages[i] = this->RE[i].getResult(planes[i]);
		cv::bitwise_and(this->resultImage, resultImages[i], this->resultImage);
		cv::resize(resultImages[i],resultImages[i],cv::Size(planes[i].size().width/4,planes[i].size().height/4));
	}
	if(this->morphology.type== OPENING){
		cv::dilate(this->resultImage,this->resultImage,cv::Mat(),cv::Point(),this->morphology.num);
		cv::erode(this->resultImage,this->resultImage,cv::Mat(),cv::Point(),this->morphology.num);
	}
	else if(this->morphology.type == CLOSING){
		cv::erode(this->resultImage,this->resultImage,cv::Mat(),cv::Point(),this->morphology.num);
		cv::dilate(this->resultImage,this->resultImage,cv::Mat(),cv::Point(),this->morphology.num);
	}
	cv::Moments moment = cv::moments(resultImage);
	this->pixelsTtl = (int)moment.m00;

	if(moment.m00>0){
		this->center = cv::Point2i((int)(moment.m10/moment.m00), (int)(moment.m01/moment.m00));
	}

	cv::resize(this->resultImage,resultImages[3],cv::Size(planes[0].size().width/4,planes[0].size().height/4));

	cv::Mat vImage[2];
	cv::hconcat(resultImages[0],resultImages[1],vImage[0]);
	cv::hconcat(resultImages[2],resultImages[3],vImage[1]);

	cv::Mat imageForShowing;
	cv::vconcat(vImage[0],vImage[1],imageForShowing);

	return this->resultImage;
}
void RegionExtraction_multiColor::showInfo()
{
	printf("=== RegionExtraction_multiColor ==================\n");
	printf("= moduleName\t: %s\n\n",this->moduleName.c_str());
	this->RE[0].showInfo();
	this->RE[1].showInfo();
	this->RE[2].showInfo();
	printf("==================================================\n\n");
}

void RegionExtraction_multiColor::setMorphology(MORPHOLOGY _type, unsigned int _num){this->morphology.type = _type; this->morphology.num = _num;}
std::string RegionExtraction_multiColor::getModuleName(){return this->moduleName;}
Morphology RegionExtraction_multiColor::getMorphology(){return this->morphology;}
unsigned int RegionExtraction_multiColor::getPixelsTtl(){return this->pixelsTtl;}
cv::Point2i RegionExtraction_multiColor::getCenter(){return this->center;}
RegionExtraction_color* RegionExtraction_multiColor::getRePtr(unsigned int _no)
{
	switch(_no){
	case 0:
	case 1:
	case 2:
		return &this->RE[_no];
	default:
		return NULL;
	}
}
cv::Mat RegionExtraction_multiColor::getResultImage(){return this->resultImage;}

}
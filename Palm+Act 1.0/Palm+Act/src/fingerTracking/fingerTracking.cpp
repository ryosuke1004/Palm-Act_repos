#include "fingerTracking.h"
#include <Windows.h>

FingerTracking::FingerTracking(unsigned int _samplingInterval)
{
	this->TM_ESM = new TEMPLATE_MATCHING_ESM::TemplateMatching_ESM(cv::Mat(),cv::Mat(),_samplingInterval);
	this->TM_STD = new TEMPLATE_MATCHING_STANDARD::TemplateMatching_standard(cv::Mat(),_samplingInterval);
	this->tmpImage = cv::Mat();
	this->prevImage = cv::Mat();
	this->G = cv::Mat();
	this->flgRenewTmpImage = false;
	this->renewThr = 0.997;
}
FingerTracking::~FingerTracking()
{
	delete this->TM_ESM;
	delete this->TM_STD;
}

bool FingerTracking::init(cv::Mat _initFrame)
{
	this->prevImage = cv::Mat();
	if(_initFrame.channels()==3){
		cv::cvtColor(_initFrame,this->prevImage,CV_BGR2GRAY);
		if(!TM_STD->makeTmpImage(this->prevImage)){
			puts("Error: init");
			puts("- fail to makeTmpImage");
			return false;
		}
	}
	else if(_initFrame.channels()==1){
		_initFrame.copyTo(this->prevImage);
		if(!TM_STD->makeTmpImage(_initFrame)){
			puts("Error: init");
			puts("- fail to makeTmpImage");
			return false;
		}
	}
	else{
		puts("Error: init");
		puts("- invalid channels");
		return false;
	}

	TM_STD->getTmpImage().copyTo(this->initTmpImage);
	TM_STD->getTmpImage().copyTo(this->tmpImage);
	this->rect_roi = TM_STD->getRectRoi();
	this->G = (cv::Mat_<double>(3,3)<<1,0,TM_STD->getRectRoi().x,0,1,TM_STD->getRectRoi().y,0,0,1);
	this->initTmpSize = TM_STD->getTmpSize();
	if(!TM_ESM->init(this->tmpImage,this->G)){
		puts("Error: init");
		puts("- fail to init TM_ESM");
		return false;	
	}

	return true;
}
bool FingerTracking::tracking(cv::Mat _curFrame)
{
	if(this->G.empty()){
		puts("Error: tracking");
		puts("- This Module has not been initialized");
		return false;
	}

	if(_curFrame.empty() || _curFrame.size().width<tmpImage.size().width || _curFrame.size().height<tmpImage.size().height){
		puts("Error: tracking");
		puts("- invalid frame");
		return false;
	}

	cv::Mat curImage;
	if(_curFrame.channels()==3){
		cv::cvtColor(_curFrame,curImage,CV_BGR2GRAY);
	}
	else if(_curFrame.channels()==1){
		_curFrame.copyTo(curImage);
	}
	else{
		puts("Error: tracking");
		puts("- invalid channel");
		return false;
	}
	Sleep(1);

	//初期テンプレートで標準テンプレートマッチング
	bool flg=false;
	TM_STD->init(this->initTmpImage);
	TM_STD->setRectRoi(this->rect_roi);
	TM_STD->matching(curImage);
	//類似度が低い場合は，１つ前のフレームのテンプレートで標準テンプレートマッチング
	if(TM_STD->getCosSim()<this->renewThr){
		flg=true;
		TM_STD->init(this->prevImage(this->rect_roi));
		TM_STD->setRectRoi(this->rect_roi);
		TM_STD->matching(curImage);
		cv::Point dP(TM_STD->getRectRoi().x-this->rect_roi.x,TM_STD->getRectRoi().y-this->rect_roi.y);
		if(!TM_ESM->init(this->tmpImage,(cv::Mat_<double>(3,3)<<1,0,dP.x,0,1,dP.y,0,0,1)*this->G))	return false;
	}
	else{
		TM_ESM->init(this->tmpImage,(cv::Mat_<double>(3,3)<<1,0,TM_STD->getRectRoi().x,0,1,TM_STD->getRectRoi().y,0,0,1));
		
	}
	Sleep(1);

	//ESMテンプレートマッチング
	TM_ESM->matching(curImage);
	Sleep(1);

	TM_ESM->getG().copyTo(this->G);
	this->rect_roi = TM_STD->getRectRoi();
	curImage.copyTo(prevImage);
	Sleep(1);
	if(!flg)	curImage(this->rect_roi).copyTo(this->tmpImage);
	return true;
}

cv::Mat FingerTracking::getTmpImage()
{
	return this->tmpImage;
}
cv::Mat FingerTracking::getprevImage()
{
	return this->prevImage;
}
cv::Size FingerTracking::getInitTmpSize()
{
	return this->initTmpSize;
}
cv::Mat FingerTracking::getG()
{
	return this->G;
}
void FingerTracking::setFlgRenewTmpImage(bool _flg)
{
	this->flgRenewTmpImage=_flg;
}
void FingerTracking::setRenewThr(double _renewThr)
{
	this->renewThr = _renewThr;
}
void FingerTracking::setRectRoi(cv::Rect _rectRoi)
{
	this->rect_roi = _rectRoi;
}
void FingerTracking::setG(cv::Mat _G)
{
	_G.copyTo(this->G);
}
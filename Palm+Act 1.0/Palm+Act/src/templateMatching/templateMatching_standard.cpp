#include "templateMatching_standard.h"
#include <Windows.h>

namespace TEMPLATE_MATCHING_STANDARD{
/*=== class: TemplateMatching_standard ===*/
TemplateMatching_standard::TemplateMatching_standard(cv::Mat _tmpImage, int _samplingInterval)
{
	this->samplingInterval=_samplingInterval;
	init(_tmpImage);
	//this->v2=0;
	//this->vv=0;
	this->cosSim=-1;
	this->rect_roi=cv::Rect();
	this->lost=true;
	this->resultImage=cv::Mat();
}
TemplateMatching_standard::~TemplateMatching_standard()
{
}

bool TemplateMatching_standard::init(cv::Mat _tmpImage)
{
	_tmpImage.copyTo(this->tmpImage);
	if(this->tmpImage.empty()){
		puts("Warning: init");
		puts("- empty tmpImage");
		this->tmpSize=cv::Size(0,0);
		this->tmpPixels=0;
		//this->v1=0;
		return false;
	}

	this->tmpSize=_tmpImage.size();

	cv::Point p;
	this->tmpPixels=0;
	//this->v1=0;
	for(int i=0; i<this->tmpSize.width*this->tmpSize.height; i+=(1+this->samplingInterval)){
		p.x=i%this->tmpSize.width;
		p.y=i/this->tmpSize.width;
		//this->v1+=this->tmpImage(p)*this->tmpImage(p);
		this->tmpPixels++;
	}
	this->lost=true;
	return true;
}
bool TemplateMatching_standard::makeTmpImage(cv::Mat _refImage)
{
	if(_refImage.empty()){
		puts("Error: makeTmpImage");
		puts("- empty image");
		return false;
	}

	this->tmpSize = cv::Size(_refImage.size().width/5,_refImage.size().height/5);
	int d;//テンプレート位置更新のためゲイン
	cv::Point p0(0,0);
	cv::Point p1(this->tmpSize.width,this->tmpSize.height);
	this->rect_roi=cv::Rect(p0,p1);
	cv::Rect rect_image(0,0,_refImage.size().width,_refImage.size().height);//画像領域
	cv::Mat resultImage;
	while(1){
		std::cout<<"p:"<<p0<<" size:"<<tmpSize<<std::endl;
		d = sqrt((double)tmpSize.width*tmpSize.width+tmpSize.height*tmpSize.height)/30;

		_refImage.copyTo(resultImage);
		cv::rectangle(resultImage,this->rect_roi,cv::Scalar(255));
		cv::imshow("q: finish 's'+arrow: change tmpImage size",resultImage);
		cv::waitKey(1);

		//テンプレート位置の更新*/
		int dx=0;
		int dy=0;
		int dw=0;
		int dh=0;
		if(!GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_DOWN & 0x28))	dy=d;
		if(!GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_LEFT & 0x25))	dx=-d;
		if(!GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_RIGHT & 0x27))	dx=d;
		if(!GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_UP & 0x26))		dy=-d;
		//'s'を押しながら矢印キーでサイズ変更
		if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_DOWN & 0x28))	dh=-d;
		if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_LEFT & 0x25))	dw=-d;
		if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_RIGHT & 0x27))	dw=d;
		if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_UP & 0x26))		dh=d;
		//'q'が押された場合に終了する
		if(GetAsyncKeyState('q' & 0x51))	break;

		//テンプレートがフレームサイズ内かチェックして更新
		if((p0+cv::Point(dx,dy)).inside(rect_image) && (p1+cv::Point(dx+dw,dy+dh)).inside(rect_image)){
			p0.x+=dx;
			p0.y+=dy;
			tmpSize.width+=dw;
			tmpSize.height+=dh;
		}
		p1 = p0 + cv::Point2i(tmpSize.width,tmpSize.height);
		this->rect_roi=cv::Rect(p0,p1);
	}
	cv::destroyWindow("q: finish 's'+arrow: change tmpImage size");

	this->init(_refImage(this->rect_roi));
	this->lost=false;
	return true;
}
cv::Mat TemplateMatching_standard::matching(cv::Mat _curImage)
{
	if(_curImage.empty()){
		puts("Error: matching");
		puts("- empty image");
		return cv::Mat();
	}
	if(this->tmpImage.empty()){
		puts("Error: matching");
		puts("- empty tmpImage");
		return cv::Mat();
	}

	for(int i=0; i<2; i++){
		cv::Rect rect_search;
		if(this->lost){//見失った場合は，画像領域全てを探索
			rect_search=cv::Rect(0,0,_curImage.size().width,_curImage.size().height);
			this->lost=false;
		}
		else{//追跡中は，前のフレームでのテンプレート位置の周辺，rect_search内を探索
			int searchArea=min(this->tmpSize.width, this->tmpSize.height);
			cv::Point p0(this->rect_roi.x-searchArea,this->rect_roi.y-searchArea);
			cv::Point p1(this->rect_roi.x+this->rect_roi.width+searchArea,this->rect_roi.y+this->rect_roi.height+searchArea);
			rect_search=cv::Rect(p0,p1);
		}
		if(rect_search.x<0)	rect_search.x=0;
		if(rect_search.y<0)	rect_search.y=0;
		if(_curImage.size().width-1<rect_search.x+rect_search.width)	rect_search.width=_curImage.size().width-1-rect_search.x;
		if(_curImage.size().height-1<rect_search.y+rect_search.height)	rect_search.height=_curImage.size().height-1-rect_search.y;

		cv::Point maxP;
		cv::Mat result;
		cv::matchTemplate(_curImage(rect_search),this->tmpImage,result,CV_TM_CCORR_NORMED);
		cv::minMaxLoc(result,NULL,&this->cosSim,NULL,&maxP);

		this->rect_roi.x = rect_search.x + maxP.x;
		this->rect_roi.y = rect_search.y + maxP.y;
		_curImage.copyTo(this->resultImage);
		cv::rectangle(this->resultImage,this->rect_roi,cv::Scalar(0));
		cv::rectangle(this->resultImage,rect_search,cv::Scalar(255));

		if(this->cosSim<TEMPLATE_MATCHING_STANDARD::cosSimThr)//見失った場合はもう一度画像領域全てを探索
			this->lost=true;
		else
			break;
	}

	return _curImage(this->rect_roi);
}

cv::Mat TemplateMatching_standard::getTmpImage()
{
	return this->tmpImage;
}
cv::Size TemplateMatching_standard::getTmpSize()
{
	return this->tmpSize;
}
int TemplateMatching_standard::getSamplingInterval()
{
	return this->samplingInterval;
}
int TemplateMatching_standard::getTmpPixels()
{
	return this->tmpPixels;
}
cv::Rect TemplateMatching_standard::getRectRoi()
{
	return this->rect_roi;
}
double TemplateMatching_standard::getCosSim()
{
	return this->cosSim;
}
bool TemplateMatching_standard::isLost()
{
	return this->lost;
}
cv::Mat TemplateMatching_standard::getResultImage()
{
	return this->resultImage;
}
void TemplateMatching_standard::setSamplingInterval(unsigned int _samplingInterval)
{
	this->samplingInterval=_samplingInterval;
	init(this->tmpImage);
}
void TemplateMatching_standard::setRectRoi(cv::Rect _rect_roi)
{
	this->rect_roi = _rect_roi;
	this->lost = false;
}

}
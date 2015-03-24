#pragma once

#include "templateMatching_ESM.h"
#include <Windows.h>
//#include <omp.h>

namespace TEMPLATE_MATCHING_ESM{

/*=== 関数 ===*/
cv::Point TEMPLATE_MATCHING_ESM::warpPoint(cv::Point _p, cv::Mat _G, G_TYPE _TYPE)
{
	if(_G.empty()){
		puts("Error: warpPoint");
		puts("- empty G");
		return cv::Point();
	}
	switch(_TYPE){
	case G_TYPE::HOMOGRAPHY:
		return cv::Point(
					(_G.at<double>(0,0)*_p.x+_G.at<double>(0,1)*_p.y+_G.at<double>(0,2))/(_G.at<double>(2,0)*_p.x+_G.at<double>(2,1)*_p.y+_G.at<double>(2,2))+0.5,
					(_G.at<double>(1,0)*_p.x+_G.at<double>(1,1)*_p.y+_G.at<double>(1,2))/(_G.at<double>(2,0)*_p.x+_G.at<double>(2,1)*_p.y+_G.at<double>(2,2))+0.5
					);
	default:
		puts("Error: warpPoint");
		puts("- invalid G_TYPE");
		return cv::Point();
	}
}
cv::Point TEMPLATE_MATCHING_ESM::derivation(cv::Mat _img, cv::Point _p, cv::Mat _G, DERIVATION_TYPE _TYPE)
{
	if(_img.empty()){
		puts("Warning: derivation");
		puts("- empty image");
		return cv::Point(0,0);
	}
	cv::Rect roi(0,0,_img.size().width,_img.size().height);
	cv::Point p=warpPoint(_p,_G);
	if(!p.inside(roi)){
		//puts("Warning: derivation");
		//puts("- warped point is out of image");
		return cv::Point(0,0);
	}

	cv::Point drv;
	switch(_TYPE){
	case DERIVATION_TYPE::SOBEL:
		if(0<p.x && p.x<_img.size().width-1 && 0<p.y && p.y<_img.size().height-1){
			cv::Point p1=warpPoint(_p+cv::Point(-1,-1),_G);
			cv::Point p2=warpPoint(_p+cv::Point(0,-1),_G);
			cv::Point p3=warpPoint(_p+cv::Point(1,-1),_G);

			cv::Point p4=warpPoint(_p+cv::Point(-1,0),_G);
			cv::Point p6=warpPoint(_p+cv::Point(1,0),_G);

			cv::Point p7=warpPoint(_p+cv::Point(-1,1),_G);
			cv::Point p8=warpPoint(_p+cv::Point(0,1),_G);
			cv::Point p9=warpPoint(_p+cv::Point(1,1),_G);
			if(p1.inside(roi) && p2.inside(roi) && p3.inside(roi) && p4.inside(roi) && p6.inside(roi) && p7.inside(roi) && p8.inside(roi) && p9.inside(roi)){
				drv.x = _img.at<uchar>(warpPoint(_p+cv::Point(1,-1),_G)) + 2*_img.at<uchar>(warpPoint(_p+cv::Point(1,0),_G)) + _img.at<uchar>(warpPoint(_p+cv::Point(1,1),_G))
						- _img.at<uchar>(warpPoint(_p+cv::Point(-1,-1),_G)) - 2*_img.at<uchar>(warpPoint(_p+cv::Point(-1,0),_G)) - _img.at<uchar>(warpPoint(_p+cv::Point(-1,1),_G));
				drv.y = _img.at<uchar>(warpPoint(_p+cv::Point(-1,1),_G)) + 2*_img.at<uchar>(warpPoint(_p+cv::Point(0,1),_G)) + _img.at<uchar>(warpPoint(_p+cv::Point(1,1),_G))
						- _img.at<uchar>(warpPoint(_p+cv::Point(-1,-1),_G)) - 2*_img.at<uchar>(warpPoint(_p+cv::Point(0,-1),_G)) - _img.at<uchar>(warpPoint(_p+cv::Point(1,-1),_G));
			}
			else{
				drv.x=0;
				drv.y=0;
			}
		}
		else{
			drv.x=0;
			drv.y=0;
		}
		break;
	default:
		break;
	}
	return drv;
}
cv::Mat TEMPLATE_MATCHING_ESM::getHomography(cv::Mat _x)
{
	if((_x.size().width*_x.size().height)!=8){
		puts("Error: getHomography");
		puts("- invalid x size");
		return cv::Mat();
	}

	//パラメータ_xが表すリー代数のsl(3)の要素計算
	cv::Mat_<double> A;
	A = _x.at<double>(0)*A1+
		_x.at<double>(1)*A2+
		_x.at<double>(2)*A3+
		_x.at<double>(3)*A4+
		_x.at<double>(4)*A5+
		_x.at<double>(5)*A6+
		_x.at<double>(6)*A7+
		_x.at<double>(7)*A8;
	
	cv::Mat_<double> G=(cv::Mat_<double>(3,3)<<
						1,0,0,
						0,1,0,
						0,0,1);
	
	//exponentialの計算
	double i_kaijou=1.0;
	cv::Mat_<double> A_kaijou = (cv::Mat_<double>(3,3)<<
								1,0,0,
								0,1,0,
								0,0,1);
	for(int i=0; i<100; i++){
		A_kaijou=A_kaijou*A;
		i_kaijou=i_kaijou*(i+1);
		G += A_kaijou/i_kaijou;
	}

	return G;
}

/*=== class: TemplateMatching_ESM ===*/
TemplateMatching_ESM::TemplateMatching_ESM(cv::Mat _tmpImage, cv::Mat _G, int _samplingInterval)
{

	if(_tmpImage.empty() || _G.empty()){
		puts("Warning: TemplateMatching_ESM");
		puts("- empty image or G");
		this->tmpImage = cv::Mat();
		this->tmpSize = cv::Size(0,0);
		this->samplingInterval=_samplingInterval;
		this->tmpPixels = 0;
		this->G = cv::Mat();
		this->deltaX = cv::Mat();
	}
	else{
		_tmpImage.copyTo(this->tmpImage);
		this->tmpSize = _tmpImage.size();
		this->samplingInterval = _samplingInterval;
		_G.copyTo(this->G);
		this->deltaX = cv::Mat();
		
		this->init(this->tmpImage,this->G);
	}
}
TemplateMatching_ESM::~TemplateMatching_ESM()
{
	this->J_tmpI.clear();
	this->J_wG.clear();
}

bool TemplateMatching_ESM::init(cv::Mat _tmpImage, cv::Mat _G)
{
	if(_tmpImage.empty()){
		puts("Error: init");
		puts("- empty image");
		return false;
	}

	_tmpImage.copyTo(this->tmpImage);
	this->tmpSize = _tmpImage.size();
	_G.copyTo(this->G);
	this->deltaX = cv::Mat();

	this->J_tmpI.clear();
	this->J_wG.clear();
	this->tmpPixels=0;
	this->v1=0;
	cv::Point2i p;
	this->samplingInterval=1;
#ifdef _OPENMP
#pragma omp parallel for
#endif
	for(int v=this->samplingInterval; v<this->tmpSize.height; v+=(1+this->samplingInterval)){
		for(int u=this->samplingInterval; u<this->tmpSize.width; u+=(1+this->samplingInterval)){
			p.x = u;
			p.y = v;
			this->v1+=this->tmpImage(p)*this->tmpImage(p);
			cv::Point2f drv=derivation(this->tmpImage,p,cv::Mat::eye(3,3,CV_64F));
			this->J_tmpI.push_back((cv::Mat_<double>(1,3) << drv.x, drv.y, 0));
			cv::Mat_<double> J_wi = (cv::Mat_<double>(3,9) <<
								p.x, p.y, 1, 0, 0, 0, -p.x*p.x, -p.x*p.y, -p.x,
								0, 0, 0, p.x, p.y, 1, -p.y*p.x, -p.y*p.y, -p.y,
								0, 0, 0, 0, 0, 0,    0,    0,  0);
			J_wG.push_back(J_wi*J_G);
			this->tmpPixels++;
		}
	}

	if(this->J_tmpI.size()!=this->tmpPixels)	return false;
	if(this->J_wG.size()!=this->tmpPixels)	return false;

	this->resultImage = cv::Mat_<uchar>(tmpSize);
	return true;
}
bool TemplateMatching_ESM::makeTmpImage(cv::Mat _refImage)
{
	if(_refImage.empty()){
		puts("Error: makeTmpImage");
		puts("- empty image");
		return false;
	}

	tmpSize = cv::Size(30,30);

	int d;//テンプレート位置更新のためゲイン
	cv::Rect rect_image(0,0,_refImage.size().width,_refImage.size().height);//画像領域
	cv::Point2i p0(_refImage.size().width/2, _refImage.size().height/2);//テンプレートの左上の初期位置
	cv::Point2i p1 = p0 + cv::Point2i(tmpSize.width,tmpSize.height);//テンプレートの右下の位置
	cv::Mat resultImage;
	while(1){
		d = sqrt((double)tmpSize.width*tmpSize.width+tmpSize.height*tmpSize.height)/30;

		_refImage.copyTo(resultImage);
		cv::rectangle(resultImage,p0,p1,cv::Scalar(255));
		cv::resize(resultImage,resultImage,cv::Size(640,480));
		cv::imshow("q: finish 's'+arrow: change tmpImage size",resultImage);
		cv::waitKey(1);

		//テンプレート位置の更新*/
		int dx=0;
		int dy=0;
		int dw=0;
		int dh=0;
		if(!GetAsyncKeyState('s' & 0x53) && GetAsyncKeyState(VK_DOWN & 0x28))	dy=d;
		if(!GetAsyncKeyState('s' & 0x53) && GetAsyncKeyState(VK_LEFT & 0x25))	dx=-d;
		if(!GetAsyncKeyState('s' & 0x53) && GetAsyncKeyState(VK_RIGHT & 0x27))	dx=d;
		if(!GetAsyncKeyState('s' & 0x53) && GetAsyncKeyState(VK_UP & 0x26))		dy=-d;
		//'s'を押しながら矢印キーでサイズ変更
		if(GetAsyncKeyState('s' & 0x53) && GetAsyncKeyState(VK_DOWN & 0x28))	dh=-d;
		if(GetAsyncKeyState('s' & 0x53) && GetAsyncKeyState(VK_LEFT & 0x25))	dw=-d;
		if(GetAsyncKeyState('s' & 0x53) && GetAsyncKeyState(VK_RIGHT & 0x27))	dw=d;
		if(GetAsyncKeyState('s' & 0x53) && GetAsyncKeyState(VK_UP & 0x26))		dh=d;
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
	}
	cv::destroyWindow("q: finish 's'+arrow: change tmpImage size");
	
	this->G = (cv::Mat_<double>(3,3)<<1,0,p0.x,0,1,p0.y,0,0,1);
	return this->init(_refImage(cv::Rect(p0,p1)),(cv::Mat_<double>(3,3)<<1,0,p0.x,0,1,p0.y,0,0,1));
}
cv::Mat TemplateMatching_ESM::matching(cv::Mat _curImage)
{
	if(_curImage.empty()||this->tmpImage.empty()||this->G.empty()){
		puts("Error: matching");
		if(_curImage.empty())	puts("- empty frame");
		if(this->tmpImage.empty())	puts("- empty tmpImage");
		if(this->G.empty())			puts("- empty G");
		return cv::Mat();
	}

	int counter=0;//誤差関数が連続で減少する場合にカウントアップ
	double tmpF = DBL_MAX;//誤差関数の最小値候補
	cv::Mat tmpG;//ホモグラフィの候補
	this->G.copyTo(tmpG);

	cv::Mat_<double> y0(this->tmpPixels,1);
	cv::Mat_<double> J_esm(cv::Size(8,this->tmpPixels),0);
	cv::Mat_<double> J_esm_i;
	cv::Point2i p;
	cv::Point2i warpedP;
	cv::Rect roi(0,0,_curImage.size().width,_curImage.size().height);
	cv::Point drv;
	cv::Mat_<double> invJ_esm(8,this->tmpPixels);
	cv::Mat incG;
	this->cosSim=0;

	while(1){
		/* 終了判定１ */
		if(this->cosSim>TEMPLATE_MATCHING_ESM::cosSimThr)	break;

		//可視化
		//cv::Point2i p1(0,0);
		//cv::Point2i p2(this->tmpSize.width,0);
		//cv::Point2i p3(this->tmpSize.width,this->tmpSize.height);
		//cv::Point2i p4(0,this->tmpSize.height);

		//cv::Point2i P1=TEMPLATE_MATCHING_ESM::warpPoint(p1,this->G);
		//cv::Point2i P2=TEMPLATE_MATCHING_ESM::warpPoint(p2,this->G);
		//cv::Point2i P3=TEMPLATE_MATCHING_ESM::warpPoint(p3,this->G);
		//cv::Point2i P4=TEMPLATE_MATCHING_ESM::warpPoint(p4,this->G);

		//cv::Mat test;
		//_curImage.copyTo(test);
		//cv::line(test,P1,P2,cv::Scalar(255));
		//cv::line(test,P2,P3,cv::Scalar(255));
		//cv::line(test,P3,P4,cv::Scalar(255));
		//cv::line(test,P4,P1,cv::Scalar(255));
		//cv::imshow("test",test);
		//cv::waitKey(1);

		//ヤコビアンの計算
		this->v2=0;this->vv=0;
#ifdef _OPENMP
#pragma omp parallel for
#endif
		for(int i=0; i<this->tmpPixels; i++){
			p.x = this->samplingInterval+(this->samplingInterval+1)*(i%(this->tmpSize.width/(this->samplingInterval+1)));
			p.y = this->samplingInterval+(this->samplingInterval+1)*(i/(this->tmpSize.width/(this->samplingInterval+1)));
			warpedP=warpPoint(p,this->G);
			if(warpedP.inside(roi)){
				y0(i) = _curImage.at<uchar>(warpedP)-this->tmpImage.at<uchar>(p);
				v2+=_curImage.at<uchar>(warpedP)*_curImage.at<uchar>(warpedP);
				vv+=this->tmpImage(p)*_curImage.at<uchar>(warpedP);
			}
			else{
				y0(i) = -this->tmpImage.at<uchar>(p);
				v2+=0;
				vv+=0;
			}

			drv=derivation(_curImage,p,this->G);
			J_esm_i = ((cv::Mat_<double>(1,3) << drv.x, drv.y, 0)+this->J_tmpI[i])*this->J_wG[i];

			J_esm(cv::Point(0,i))=J_esm_i.at<double>(0);
			J_esm(cv::Point(1,i))=J_esm_i.at<double>(1);
			J_esm(cv::Point(2,i))=J_esm_i.at<double>(2);
			J_esm(cv::Point(3,i))=J_esm_i.at<double>(3);
			J_esm(cv::Point(4,i))=J_esm_i.at<double>(4);
			J_esm(cv::Point(5,i))=J_esm_i.at<double>(5);
			J_esm(cv::Point(6,i))=J_esm_i.at<double>(6);
			J_esm(cv::Point(7,i))=J_esm_i.at<double>(7);
		}
		J_esm=J_esm/2.0;
		//   パラメータ更新   */
		cv::invert(J_esm,invJ_esm,1);
		this->deltaX = -invJ_esm*y0;
		incG=TEMPLATE_MATCHING_ESM::getHomography(this->deltaX);
		this->G=this->G*incG;
		this->cosSim=vv/sqrt(v1*v2);

		/*   終了判定2   */
		counter++;
		if(counter>TEMPLATE_MATCHING_ESM::countThr){
			counter=0;
			break;
		}
	}

	for(int h=0; h<this->tmpSize.height; h++){
		for(int w=0; w<this->tmpSize.width; w++){
			p.x=w;
			p.y=h;
			warpedP=warpPoint(p,this->G);
			if(warpedP.inside(roi))
				this->resultImage.at<uchar>(p) = _curImage.at<uchar>(warpedP);
		}
	}

	return this->resultImage;
}

/*--- アクセッサ ---*/
cv::Mat TemplateMatching_ESM::getTmpImage()
{
	return this->tmpImage;
}
cv::Size TemplateMatching_ESM::getTmpSize()
{
	return this->tmpSize;
}
int TemplateMatching_ESM::getSamplingInterval()
{
	return this->samplingInterval;
}
int TemplateMatching_ESM::getTmpPixels()
{
	return this->tmpPixels;
}
cv::Mat TemplateMatching_ESM::getG()
{
	return this->G;
}
double TemplateMatching_ESM::getCosSim()
{
	return this->cosSim;
}
cv::Mat TemplateMatching_ESM::getResultImage()
{
	return this->resultImage;
}
bool TemplateMatching_ESM::setSamplingInterval(int _interval)
{
	this->samplingInterval = _interval;
	return 	this->init(this->tmpImage,this->G);
}
void TemplateMatching_ESM::setG(cv::Mat _G)
{
	_G.copyTo(this->G);
}

}
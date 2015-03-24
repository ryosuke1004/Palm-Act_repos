#include "opticalFlow_farneback.h"
#undef min
#undef max
#define PI 3.141592653
namespace  OPTICAL_FLOW_FARNEBACK{
/* === class : OpticalFlow_farneback === */
OpticalFlow_farneback::OpticalFlow_farneback()
{
	this->name = std::string("new");

	this->flowPrm.pyrScale = 0.5;
	this->flowPrm.levels = 3;
	this->flowPrm.windowSize = 15;
	this->flowPrm.iterations = 3;
	this->flowPrm.polyN = 5;
	this->flowPrm.polySigma = 1.1;
	this->flowPrm.flags = 0;

	this->flowVectorsNum = 0;

	this->isCalcFlowField = false;
	this->isCalcFlowVectors = false;
}
OpticalFlow_farneback::~OpticalFlow_farneback()
{
	if(this->isCalcFlowVectors){
		delete [] this->flowVectors;
		delete [] this->refPoints;
	}
}

bool OpticalFlow_farneback::loadModule(std::string _name)
{
	char fileName[128];
	std::sprintf(fileName,"%s.csv",_name.c_str());
	std::ifstream ifs(fileName,std::ios::in);
	if(!ifs.is_open()){
		puts("Error: loadModule");
		puts("- This module cannot be opened!");
		return false;
	}

	char lineData[128];
	std::string token;
	ifs.getline(lineData,128);
	std::istringstream stream( lineData );

	getline(stream,token,',');
	if(token.compare("FARNEBACK")){
		puts("Error: loadModule");
		puts("- This file is not for FARNEBACK");
		return false;
	}

	getline(stream,token,',');
	this->flowPrm.pyrScale = atof(token.c_str());

	getline(stream,token,',');
	this->flowPrm.levels = atoi(token.c_str());

	getline(stream,token,',');
	this->flowPrm.windowSize = atoi(token.c_str());

	getline(stream,token,',');
	this->flowPrm.iterations = atoi(token.c_str());

	getline(stream,token,',');
	this->flowPrm.polyN = atoi(token.c_str());

	getline(stream,token,',');
	this->flowPrm.polySigma = atof(token.c_str());

	getline(stream,token,',');
	this->flowPrm.flags = atoi(token.c_str());

	return true;
}
bool OpticalFlow_farneback::saveModule(std::string _name)
{
	char fileName[128];
	std::sprintf(fileName,"%s.csv",_name.c_str());
	std::ofstream ofs(fileName);
	if(!ofs.is_open()){
		puts("Error: saveModule");
		puts("- ofstream cannot be opened!");
		return false;
	}

	ofs << "FARNEBACK,"
		<< this->flowPrm.pyrScale << ","
		<< this->flowPrm.levels << ","
		<< this->flowPrm.windowSize << ","
		<< this->flowPrm.iterations << ","
		<< this->flowPrm.polyN << ","
		<< this->flowPrm.polySigma << ","
		<< this->flowPrm.flags;

	return true;
}
void OpticalFlow_farneback::showInfo()
{
	printf("=== parameters of farneback ======================\n");
	printf("= pyrScale\t: %f\n",this->flowPrm.pyrScale);
	printf("= levels\t: %d\n",this->flowPrm.levels);
	printf("= windowSize\t: %d\n",this->flowPrm.windowSize);
	printf("= iterations\t: %d\n",this->flowPrm.iterations);
	printf("= polyN\t: %d\n",this->flowPrm.polyN);
	printf("= polySigma\t: %f\n",this->flowPrm.polySigma);
	printf("= flags\t: %d\n",this->flowPrm.flags);
	printf("==================================================\n\n");
}
bool OpticalFlow_farneback::calcFlowField(cv::Mat _prevFrame, cv::Mat _curFrame, cv::Mat_<cv::Point2f>& _flowField)
{
	if(_prevFrame.size().height != _curFrame.size().height ||
		_prevFrame.size().width != _curFrame.size().width){
			puts("Error: calcFlowField");
			puts("- size of prevFrame and curFrame is defferent");
			return false;
	}

	cv::calcOpticalFlowFarneback(
		_prevFrame,
		_curFrame,
		_flowField,
		this->flowPrm.pyrScale,
		this->flowPrm.levels,
		this->flowPrm.windowSize,
		this->flowPrm.iterations,
		this->flowPrm.polyN,
		this->flowPrm.polySigma,
		this->flowPrm.flags
		);

	return true;
}
bool OpticalFlow_farneback::calcFlowField(cv::Mat _prevFrame, cv::Mat _curFrame)
{
	return calcFlowField(_prevFrame,_curFrame,this->flowField);
}
bool OpticalFlow_farneback::calcFlowVectors(cv::Point2i* _refPoints, unsigned int _refPointsNum)
{
	if(this->isCalcFlowVectors){
		delete [] this->flowVectors;
		delete [] this->refPoints;
		this->isCalcFlowVectors = false;
	}
	this->flowVectorsNum = _refPointsNum;

	this->refPoints = new cv::Point2i[_refPointsNum];
	this->flowVectors = new cv::Point2f[_refPointsNum];

	for(int n=0; n<_refPointsNum; n++){
		this->refPoints[n] = _refPoints[n];
		this->flowVectors[n] = this->getFlowVectorAt(_refPoints[n]);
	}

	this->isCalcFlowVectors = true;
	return true;
}
bool OpticalFlow_farneback::renderFlowVectors(cv::Mat& _inputImage, float _maxNorm, unsigned int _thickness, cv::Mat_<uchar> _mask)
{
	//if(this->flowField.size().height != _inputImage.size().height ||
	//	flowField.size().width != _inputImage.size().width)	return false;

	//cv::Mat_<uchar> tMask;
	//_mask.copyTo(tMask);
	//if(tMask.empty())	tMask = cv::Mat_<uchar>(_inputImage.size(),255);
	//for(int n=0; n<this->flowVectorsNum; n++){
	//	if(tMask(this->refPoints[n])){
	//		cv::Scalar color=calPseudoColor(cv::norm(this->flowVectors[n])/_maxNorm);
	////		cv::Scalar color(255,255,255);
	//		CV_GRAPHICS::arrow(
	//			_inputImage,
	//			this->refPoints[n],
	//			10*this->flowVectors[n],
	//			color,
	//			_thickness);
	//	}
	//}
	return true;
}

std::string OpticalFlow_farneback::getModuleName()
{
	return this->name;
}
void* OpticalFlow_farneback::getPrmPtr(unsigned int _OPTICAL_FLOW_PRM)
{
	switch(_OPTICAL_FLOW_PRM){
	case OPTICAL_FLOW_PRM::PYR_SCALE:
	return &this->flowPrm.pyrScale;

	case OPTICAL_FLOW_PRM::LEVELS:
	return &this->flowPrm.levels;

	case OPTICAL_FLOW_PRM::WINDOW_SIZE:
	return &this->flowPrm.windowSize;

	case OPTICAL_FLOW_PRM::ITERATIONS:
	return &this->flowPrm.iterations;

	case OPTICAL_FLOW_PRM::POLY_N:
	return &this->flowPrm.polyN;

	case OPTICAL_FLOW_PRM::POLY_SIGMA:
	return &this->flowPrm.polySigma;

	case OPTICAL_FLOW_PRM::FLAGS:
	return &this->flowPrm.flags;

	default:
	return NULL;
	}
}
cv::Mat OpticalFlow_farneback::getFlowField()
{
	return this->flowField;
}
cv::Point2f OpticalFlow_farneback::getFlowVectorAt(cv::Point2i _refPoint)
{
	if(0<=_refPoint.x && _refPoint.x < this->flowField.size().width &&
		0<=_refPoint.y && _refPoint.y < this->flowField.size().height)
	{
		return this->flowField(_refPoint);
	}
	else
	{
		return cv::Point2f(0,0);
	}
}
unsigned int OpticalFlow_farneback::getFlowVectorsNum()
{
	return this->flowVectorsNum;
}
cv::Point2f* OpticalFlow_farneback::getFlowVectorsPtr()
{
	return this->flowVectors;
}
cv::Point2i* OpticalFlow_farneback::getRefPointsPtr()
{
	return this->refPoints;
}

cv::Scalar calcPseudoColor(double phase, double shift = 0.0)
{
	phase = std::max(std::min(phase,1.0), 0.0); //0Ç©ÇÁ1Ç…
	shift += PI+PI/4;     //ê¬Ç©ÇÁê‘Ç…
	return cv::Scalar(
		uchar( 255*(sin(1.5*PI*phase + shift + PI ) + 1)/2.0 ),
		uchar( 255*(sin(1.5*PI*phase + shift + PI/2 ) + 1)/2.0 ), 
		uchar( 255*(sin(1.5*PI*phase + shift  ) + 1)/2.0 )
		);
}

}
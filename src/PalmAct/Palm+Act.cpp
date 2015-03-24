#include "Palm+Act.h"
namespace PALM_ACT{

/*=== class : ForceSensor ===*/
ForceSensor::ForceSensor(char* _sensorName)
{	
	printf("==============================\n");
	printf("= ForceSensor                =\n");
	printf("==============================\n");

	this->isStarted = false;
	this->finger.isTracked=false;
	this->renderingFlg = false;
	this->timerFlg = false;
	this->lockFlg = false;
	this->initFlg=false;
	this->compensationFlg=true;

	/*=== モジュールの初期化 ===*/
	printf("open Module\n");
	/*--- 画像取得モジュールの準備 ---*/
	this->cap = new Capture_cam();
	/*--- オプティカルフローモジュールの準備 ---*/
	this->OF = new OpticalFlow_farneback();
	/*-- 肌色領域抽出モジュールの準備 ---*/
	this->RE_skin = new RegionExtraction_multiColor();
	/*--- 指先追跡モジュールの準備 ---*/
	this->FT = new FingerTracking(0);
	if(!this->cap->startCapture(0))	this->end();
	
	/*=== 設定の読み込み ===*/
	printf("loading\n");
	this->loadSensor(_sensorName);

	initSensor();
	this->isStarted=true;
	printf("------------------------------\n");
	printf("= ForceSensor: done          =\n");
	printf("==============================\n\n");
}
ForceSensor::~ForceSensor()
{
	this->end();
}

/*--- 関数 ---*/
void ForceSensor::deleteModules(){
	delete this->cap;
	delete this->OF;
	delete this->RE_skin;
	delete this->FT;
}
bool ForceSensor::capturing()
{
	while(1){
		this->originalFrame=this->cap->capture();
		if(!this->originalFrame.empty()) break;
	}
	
	if(this->prevFrame.empty() || this->curFrame.empty()){
		cv::resize(this->originalFrame,this->prevFrame,cv::Size(this->originalFrame.cols*this->resizeRatio*0.01, this->originalFrame.rows*this->resizeRatio*0.01));
		this->prevFrame.copyTo(this->initFrame);
		while(1){
			this->originalFrame=this->cap->capture();
			if(!this->originalFrame.empty()) break;
		}
	}
	else
		this->curFrame.copyTo(this->prevFrame);

	cv::resize(this->originalFrame,this->curFrame,cv::Size(this->originalFrame.cols*this->resizeRatio*0.01, this->originalFrame.rows*this->resizeRatio*0.01));

	return !this->curFrame.empty();
}
bool ForceSensor::skinExtraction()
{
	cv::Mat hsvImage;
	cv::cvtColor(this->curFrame,hsvImage,CV_BGR2HSV);
	this->maskOfSkin = this->RE_skin->getResult(hsvImage);
	return !this->maskOfSkin.empty();
}
cv::Point2i ForceSensor::calcActionPoint(cv::Mat _G)
{
	if(_G.empty())	return cv::Point2i();
	return getProjectedPoint(cv::Point(this->FT->getInitTmpSize().width/2,this->FT->getInitTmpSize().height/2),_G);
}
double ForceSensor::calcFingerThickness(cv::Mat _G)
{
	if(_G.empty())	return 0;
	return	1.2*std::max(
				cv::norm(getProjectedPoint(cv::Point(0,0),_G)-getProjectedPoint(cv::Point(this->FT->getInitTmpSize().width,0),_G)),
				cv::norm(getProjectedPoint(cv::Point(0,this->FT->getInitTmpSize().height),_G)-getProjectedPoint(cv::Point(this->FT->getInitTmpSize().width,this->FT->getInitTmpSize().height),_G))
			);
}
double ForceSensor::calcFingerAngle(cv::Mat _G)
{
	if(_G.empty())	return PI/2.0;
	cv::Point2f target=getProjectedPoint(cv::Point(this->FT->getInitTmpSize().width/2,0),_G);
	cv::Point2f source=getProjectedPoint(cv::Point(this->FT->getInitTmpSize().width/2,this->FT->getInitTmpSize().height),_G);
	return atan2((target-source).y,(target-source).x);
}
bool ForceSensor::fingerDetection()
{
	if(!this->finger.isTracked)
		return false;

	this->FT->getG().copyTo(this->finger.G);
	this->finger.actionPoint = calcActionPoint(this->finger.G);
	this->finger.thickness = (this->finger.thickness + calcFingerThickness(this->finger.G))*0.5;
	this->finger.angle = calcFingerAngle(this->finger.G);

	/*--- 指先マスクの生成 ---*/
	this->maskOfFinger = makeFingerMask(this->finger.actionPoint, this->finger.angle, this->curFrame.size(), this->finger.thickness);
	return !maskOfFinger.empty();
}
bool ForceSensor::opticalFlowCalculation()
{
	cv::Mat prev;
	cv::Mat cur;
	cv::cvtColor(this->prevFrame,prev,CV_BGR2GRAY);
	cv::cvtColor(this->curFrame,cur,CV_BGR2GRAY);
	return this->OF->calcFlowField(prev,cur);
}
bool ForceSensor::homographyCalculation()
{
	std::vector<cv::Point2f> prevPoints;
	std::vector<cv::Point2f> nextPoints;
	for(int h=0; h<this->curFrame.size().height; h++) {
		for(int w=0; w<this->curFrame.size().width; w++) {
			if(this->maskOfFinger(h,w) && this->maskOfSkin(h,w)){
				if(cv::norm(cv::Point2i(w,h)-this->finger.actionPoint) > sqrt(this->curFrame.size().width*this->curFrame.size().height*this->proportionOfLocalDisp*0.01)/2.0){
					prevPoints.push_back(cv::Point2f(w,h));
					nextPoints.push_back(cv::Point2f(w,h)+this->flowGain*this->OF->getFlowVectorAt(cv::Point2i(w,h)));
				}
			}
		}
	}
	if(prevPoints.empty()){
		return false;
	}
	//prevPointsからnextPointsへの変換行列
	this->dH = cv::findHomography(prevPoints,nextPoints,CV_RANSAC);
	return !this->dH.empty();
}
bool ForceSensor::forceEstimation()
{
	if(this->dH.empty()){
		puts("Error: forceExtimation");
		puts("- empty homography dH");
		return false;
	}
	cv::Mat_<double> invH;
	if(!cv::invert(this->H,invH)){
		puts("Error: run");
		puts("- cannot calculate invert matrix of Homography H");
		return false;
	}
	cv::Mat_<double> invdH;
	if(!cv::invert(this->dH,invdH)){
		puts("Error: run");
		puts("- cannot calculate invert matrix of Homography dH");
		return false;
	}

	cv::Rect tImageRect(0,0,this->curFrame.size().width,this->curFrame.size().height);
	this->onSkinVectorsNum = 0;
	this->dForce = cv::Vec3f(0,0,0);
	cv::Point2f tReferencePoint;
	cv::Point2f tProjectedActionPoint = getProjectedPoint((cv::Point2f)this->finger.actionPoint,invH*invdH);
	cv::Point2f tProjectedReferencePoint;
	bool _renderingFlg=this->renderingFlg;
	//描画
	if(_renderingFlg){
		this->resultImage = getProjectedImage(this->curFrame,invH*invdH);
		this->curFrame.copyTo(this->flowImage);
		this->initFrame.copyTo(this->flowImageInOriginalPlane);
		CV_GRAPHICS::cross(this->resultImage,tProjectedActionPoint,cv::Scalar(0,0,0));
		CV_GRAPHICS::cross(this->flowImage,finger.actionPoint,cv::Scalar(0,0,0));
		CV_GRAPHICS::cross(this->flowImageInOriginalPlane,tProjectedActionPoint,cv::Scalar(0,0,0));
	}
	/* 参照点が肌領域上にあるかどうか判定 */
	for(int i=0; i<this->referencePointsNum; i++)	this->isOnSkin[i]=true;
	for(int t=0; t<this->vectorsOnCircleNum; t++) {
		for(int r=1; r<=this->vectorsOnRadiusNum; r++) {
			if(isOnSkin[t*this->vectorsOnRadiusNum+r-1]){
				//正面画像において参照点を決定
				tProjectedReferencePoint = tProjectedActionPoint + this->referencePoints[t*this->vectorsOnRadiusNum+r-1];
				//もとの画像における参照点を計算
				tReferencePoint = getProjectedPoint(tProjectedReferencePoint,H);
				if(!tReferencePoint.inside(tImageRect) || !tProjectedReferencePoint.inside(tImageRect)){
					isOnSkin[t*this->vectorsOnRadiusNum+r-1] = false;
				}
				else if(!this->maskOfFinger(tReferencePoint) || !this->maskOfSkin(tReferencePoint)){
					isOnSkin[t*this->vectorsOnRadiusNum+r-1] = false;
				}
				//描画
				if(_renderingFlg){
					cv::circle(this->resultImage,tProjectedReferencePoint,1,cv::Scalar(0,0,0));
					cv::circle(this->flowImage,tReferencePoint,1,cv::Scalar(0,0,0));
					cv::circle(this->flowImageInOriginalPlane,tProjectedReferencePoint,1,cv::Scalar(0,0,0));
				}
			}
		}
	}
	/* 特徴量の抽出 */
	cv::Point2f tFlowVector;
	cv::Point2f tTipOfFlowVector;
	cv::Point2f tCompensatedFlowVector;
	cv::Point2f tProjectedFlowVector;
	double projectionValue=0;
	for(int i=0; i<this->referencePointsNum; i++){
		//正面画像において参照点を決定
		tProjectedReferencePoint = tProjectedActionPoint + this->referencePoints[i];
		//もとの画像における参照点を計算
		tReferencePoint = getProjectedPoint(tProjectedReferencePoint,H);
		if(isOnSkin[i]){
			//もとの画像において，参照点におけるフローベクトルを取得
			tFlowVector = this->flowGain*this->OF->getFlowVectorAt(tReferencePoint);
			//フレーム間の掌の動きを考慮して，フローベクトルの終点を補正
			tTipOfFlowVector = getProjectedPoint(tReferencePoint + tFlowVector,invdH);
			//補正後のフローベクトルを計算
			tCompensatedFlowVector = tTipOfFlowVector - tReferencePoint;
			//正面画像におけるフローベクトルを計算
			tProjectedFlowVector = getProjectedPoint(tTipOfFlowVector,invH) - tProjectedReferencePoint;
			//平均フローベクトルの計算
			this->dForce[0] += tProjectedFlowVector.x*(100.0/this->resizeRatio);
			this->dForce[1] += tProjectedFlowVector.y*(100.0/this->resizeRatio);
			//極軸への射影
			this->dForce[2]
			+= tProjectedFlowVector
				.dot(cv::Point2f(
						-cos(2.0*PI*(i%this->vectorsOnCircleNum)/this->vectorsOnCircleNum),
						-sin(2.0*PI*(i%this->vectorsOnCircleNum)/this->vectorsOnCircleNum)
						)
					)*(100.0/this->resizeRatio);
			this->onSkinVectorsNum++;
			//描画
			if(_renderingFlg){
				cv::Scalar color;
				color=CV_GRAPHICS::calPseudoColor(cv::norm(tProjectedFlowVector));
				CV_GRAPHICS::arrow(this->resultImage,tProjectedReferencePoint,30*tProjectedFlowVector,color,1);
				color=CV_GRAPHICS::calPseudoColor(cv::norm(tFlowVector));
				CV_GRAPHICS::arrow(this->flowImage,tReferencePoint,30*tFlowVector,color,1);
				color=CV_GRAPHICS::calPseudoColor(cv::norm(tProjectedFlowVector));
				CV_GRAPHICS::arrow(this->flowImageInOriginalPlane,tProjectedReferencePoint,30*tProjectedFlowVector,color,1);
			}
		}
	}
	if(this->onSkinVectorsNum){
		this->dForce[0] = this->dForce[0]/this->onSkinVectorsNum;
		this->dForce[1] = this->dForce[1]/this->onSkinVectorsNum;
		this->dForce[2] = this->dForce[2]/this->onSkinVectorsNum;
	}
	else{
		this->dForce[0] = 0;
		this->dForce[1] = 0;
		this->dForce[2] = 0;
	}
	return true;
}
bool ForceSensor::render()
{
	if(!this->renderingFlg)	return false;
	cv::imshow("result",this->resultImage);
	cv::imshow("flow",this->flowImage);
	cv::imshow("original plane",this->flowImageInOriginalPlane);
	cv::waitKey(1);
	return true;
}
void ForceSensor::showTimer()
{
	this->ST.showSplitTimes();
}
bool ForceSensor::keyboard()
{
	KeyboardFunc::discreteFlg = true;
	if(KeyboardFunc::getIsCharPushed('p')){
		return this->prmSetting();
	}
	if(KeyboardFunc::getIsCharPushed('s')){
		this->showInfo();
	}
	if(KeyboardFunc::getIsCharPushed('i')){
		std::cout<<"3"<<std::endl;	Sleep(1000);
		std::cout<<"2"<<std::endl;	Sleep(1000);
		std::cout<<"1"<<std::endl;	Sleep(1000);
		this->initSensor();
		return true;
	}
	if(KeyboardFunc::getIsCharPushed('r')){
		if(this->renderingFlg){
			this->renderingFlg=false;
			cv::destroyAllWindows();
		}
		else	this->renderingFlg=true;
	}
	if(KeyboardFunc::getIsCharPushed('t')){
		if(this->timerFlg){
			this->timerFlg=false;
		}
		else	this->timerFlg=true;
	}
	if(KeyboardFunc::getIsSpacePushed()){
		this->finger.isTracked=false;
		return this->initFT();
	}
	if(KeyboardFunc::getIsEscapePushed()){
		end();
		return true;
	}

	return false;
}

/*--- インタフェース ---*/
bool ForceSensor::initSensor()
{
	while(this->isLocked()){}
	this->lockFlg = true;
	printf("==============================\n");
	printf("= initSensor                 =\n");
	printf("==============================\n");

	if(this->isStarted){
		delete [] this->referencePoints;
		delete [] this->isOnSkin;
	}

	this->cap->initCapture();
	/*--- 変数 ---*/
	/* 画像 */
	this->capturing();
	cv::Size tmpSize(
		this->curFrame.size().width,
		this->curFrame.size().height
	);
	this->prevFrame=cv::Mat();
	this->curFrame=cv::Mat();
	this->initFrame=cv::Mat();
	/* マスク */
	this->maskOfSkin = cv::Mat_<uchar>(tmpSize,(uchar)0);
	this->maskOfFinger = cv::Mat_<uchar>(tmpSize,(uchar)0);
	/* 点 */
	this->referencePoints = new cv::Point2f[this->referencePointsNum];
	this->isOnSkin = new bool[this->referencePointsNum];
	int tmpDataSize=tmpSize.width*tmpSize.height;
	double tmpRadius = sqrt(tmpDataSize*this->proportionOfCircle*0.01)/2.0;
	for(int t=0; t<this->vectorsOnCircleNum; t++) {
		for(int r=1; r<=this->vectorsOnRadiusNum; r++) {
			cv::Point2f position(
				(tmpRadius*r/this->vectorsOnRadiusNum)*cos(2.0*PI*t/this->vectorsOnCircleNum) ,
				(tmpRadius*r/this->vectorsOnRadiusNum)*sin(2.0*PI*t/this->vectorsOnCircleNum)
			);
			this->referencePoints[(r-1)*this->vectorsOnCircleNum + t] = position;
		}
	}
	/* 状態変数 */
	this->H = cv::Mat::eye(3,3,CV_64FC1);
	this->dH = cv::Mat::eye(3,3,CV_64FC1);
	this->onSkinVectorsNum=0;
	this->dForce = cv::Vec3f(0,0,0);
	/*--- 出力 ---*/
	this->force = cv::Vec3f(0,0,0);

	/* 指 */
	this->capturing();
	this->capturing();
	if(this->finger.isTracked){
		this->FT->setRectRoi(cv::Rect(tmpSize.width/2,tmpSize.height/2,this->FT->getInitTmpSize().width,this->FT->getInitTmpSize().height));
		this->FT->setG(cv::Mat::eye(3,3,CV_64F));
		this->FT->tracking(curFrame);
		this->FT->getG().copyTo(this->finger.G);
		this->finger.actionPoint = calcActionPoint(this->finger.G);
		this->finger.thickness = calcFingerThickness(this->finger.G);
		this->finger.angle = calcFingerAngle(this->finger.G);
	}
	this->initFlg = false;
	this->lockFlg = false;
	printf("------------------------------\n");
	printf("= initSensor: done           =\n");
	printf("==============================\n\n");
	return true;
}
bool ForceSensor::loadSensor(std::string _sensorName)
{
	while(this->isLocked()){}
	this->lockFlg = true;
	printf("==============================\n");
	printf("= loadSensor                 =\n");
	printf("==============================\n");

	/*=== 設定ファイルの読み込み ===*/
	printf("Loading %s\n",_sensorName.c_str());
	char fileName[128];
	std::sprintf(fileName,"data/%s.csv",_sensorName.c_str());
	std::ifstream ifs(fileName,std::ios::in);
	if(!ifs.is_open()){
		puts("Warning: loadSensor");
		printf("- %s does not exist\n",_sensorName.c_str());

		/*--- 名前 ---*/
		this->sensorName = "newForceSensor";
		/*--- パラメータ ---*/
		this->resizeRatio = 50;
		this->vectorsOnCircleNum = 18;
		this->vectorsOnRadiusNum = 10;
		this->referencePointsNum = this->vectorsOnCircleNum*this->vectorsOnRadiusNum;
		this->proportionOfCircle = 30;
		this->proportionOfLocalDisp = 30;
		this->flowGain=1;
	}
	else{
		char lineData[128];
		ifs.getline(lineData,128);
		std::string token;
		std::istringstream stream( lineData );
	
		/*--- 名前 ---*/
		this->sensorName = _sensorName;
		/*--- パラメータ ---*/
		getline(stream,token,',');
		this->resizeRatio = atoi(token.c_str());
		getline(stream,token,',');
		this->vectorsOnCircleNum = atoi(token.c_str());
		getline(stream,token,',');
		this->vectorsOnRadiusNum = atoi(token.c_str());
		this->referencePointsNum = this->vectorsOnCircleNum*this->vectorsOnRadiusNum;
		getline(stream,token,',');
		this->proportionOfCircle = atoi(token.c_str());
		getline(stream,token,',');
		this->proportionOfLocalDisp = atoi(token.c_str());
		getline(stream,token,',');
		this->flowGain = std::atof(token.c_str());
	}

	/*=== モジュールのパラメータ読み込み ===*/
	printf("Loading Modules\n");
	bool flg=true;
	char tModuleName[128];
	/*--- OFの読み込み ---*/
	printf(" Loading OF\n");
	std::sprintf(tModuleName,"data/%s_OF",_sensorName.c_str());
	if(!this->OF->loadModule(tModuleName))	flg=false;
	/*--- RE_skinの読み込み ---*/
	printf(" Loading RE_skin\n");
	std::sprintf(tModuleName,"data/%s_RE_skin",_sensorName.c_str());
	if(!this->RE_skin->loadModule(tModuleName))	flg=false;
	if(!flg){
		puts("Error: loadSensor");
		puts("- Module cannot be opened");
		this->deleteModules();
		/*--- 画像取得モジュールの準備 ---*/
		this->cap = new Capture_cam();
		cap->startCapture(0);
		/*--- オプティカルフローモジュールの準備 ---*/
		this->OF = new OpticalFlow_farneback();
		/*-- 肌色領域抽出モジュールの準備 ---*/
		this->RE_skin = new RegionExtraction_multiColor();
		/*--- 指先追跡モジュールの準備 ---*/
		this->FT = new FingerTracking(0);

		this->lockFlg=false;
		this->initSensor();
		printf("==============================\n\n");
		return false;
	}
	else{
		this->lockFlg=false;
		this->initSensor();
		printf("------------------------------\n");
		printf("= loadSensor: done           =\n");
		printf("==============================\n\n");
		return true;
	}
}
bool ForceSensor::saveSensor(std::string _sensorName)
{
	while(this->isLocked()){}
	printf("==============================\n");
	printf("= saveSensor                 =\n");
	printf("==============================\n");

	if(!this->isStarted){
		puts("Error: saveSensor");
		puts("- Sensor has not been started");
		printf("==============================\n\n");
		return false;
	}

	char fileName[128];
	std::sprintf(fileName,"data/%s.csv",_sensorName.c_str());
	std::ofstream ofs(fileName);
	if(!ofs.is_open()){
		puts("Error: saveSensor");
		puts("- Ofstream cannot be opened");
		printf("==============================\n\n");
		return false;
	}

	ofs
		<<this->resizeRatio<<","
		<<this->vectorsOnCircleNum<<","
		<<this->vectorsOnRadiusNum<<","
		<<this->proportionOfCircle<<","
		<<this->proportionOfLocalDisp<<","
		<<this->flowGain;
		
	std::sprintf(fileName,"data/%s_OF",_sensorName.c_str());
	OF->saveModule(fileName);
	std::sprintf(fileName,"data/%s_RE_skin",_sensorName.c_str());
	RE_skin->saveModule(fileName);

	printf("------------------------------\n");
	printf("= saveSensor: done           =\n");
	printf("==============================\n\n");
	return true;
}
bool ForceSensor::prmSetting()
{
	while(this->isLocked()){}
	this->lockFlg=true;
	printf("==============================\n");
	printf("= prmSetting                 =\n");
	printf("==============================\n");

	if(!this->isStarted){
		puts("Error: prmSetting");
		puts("- Sensor has not been started");
		this->lockFlg=false;
		printf("==============================\n\n");
		return false;
	}

	REGION_EXTRACTION::settingPrm(this->cap,*this->RE_skin,CV_BGR2HSV,this->resizeRatio);

	this->cap->initCapture();
	cv::Mat tImage;
	printf("start setting parameters of reference points\n(quit : q)\n");

	int tVectorsOnCircleNum=this->vectorsOnCircleNum;
	int tVectorsOnRadiusNum=this->vectorsOnRadiusNum;
	int tProportionOfCircle=this->proportionOfCircle;
	int tProportionOfLocalDisp=this->proportionOfLocalDisp;

	bool stopFlg = false;
	cv::namedWindow("sensor [s:stop q:quit]");
	while(1){
		if(KeyboardFunc::getIsCharPushed('q'))	break;
		if(KeyboardFunc::getIsCharPushed('s')){
			if(stopFlg)	stopFlg = false;
			else		stopFlg = true;
		}

		cv::createTrackbar("vectorsOnCircleNum","sensor [s:stop q:quit]",&tVectorsOnCircleNum,72);
		cv::createTrackbar("vectorsOnRadiusNum","sensor [s:stop q:quit]",&tVectorsOnRadiusNum,30);
		cv::createTrackbar("proportionOfCircle","sensor [s:stop q:quit]",&tProportionOfCircle,100);
		cv::createTrackbar("proportionOfLocalDisp","sensor [s:stop q:quit]",&tProportionOfLocalDisp,100);
		
		this->vectorsOnCircleNum = tVectorsOnCircleNum;
		this->vectorsOnRadiusNum = tVectorsOnRadiusNum;
		this->proportionOfCircle = tProportionOfCircle;
		this->proportionOfLocalDisp = tProportionOfLocalDisp;

		if(!stopFlg){
			this->capturing();
			this->curFrame.copyTo(tImage);
		}
		int tmpDataSize=tImage.rows*tImage.cols;
		double tmpRadius = sqrt(tmpDataSize*this->proportionOfCircle*0.01)/2.0;
		for(int t=0; t<this->vectorsOnCircleNum; t++) {
			for(int r=1; r<=this->vectorsOnRadiusNum; r++) {
				cv::Point2f position(
					tImage.size().width/2+(tmpRadius*r/this->vectorsOnRadiusNum)*cos(2.0*PI*t/this->vectorsOnCircleNum) ,
					tImage.size().height/2+(tmpRadius*r/this->vectorsOnRadiusNum)*sin(2.0*PI*t/this->vectorsOnCircleNum)
				);
				cv::circle(tImage,position,3,cv::Scalar(255,255,255),-1);
			}
		}
		cv::circle(tImage,cv::Point2i(tImage.size().width/2,tImage.size().height/2),tmpRadius,cv::Scalar(0,0,0),1);
		tmpRadius = sqrt(tmpDataSize*this->proportionOfLocalDisp*0.01)/2.0;
		cv::circle(tImage,cv::Point2i(tImage.size().width/2,tImage.size().height/2),tmpRadius,cv::Scalar(0,0,255),1);
		cv::imshow("sensor [s:stop q:quit]",tImage);
		cv::waitKey(1);
	}
	this->referencePointsNum = this->vectorsOnCircleNum*this->vectorsOnRadiusNum;
	cv::destroyWindow("sensor [s:stop q:quit]");
	this->lockFlg=false;
	this->initSensor();
	printf("------------------------------\n");
	printf("= prmSetting: done           =\n");
	printf("==============================\n\n");
}
bool ForceSensor::initFT()
{
	while(this->isLocked()){}
	this->lockFlg=true;
	printf("==============================\n");
	printf("= initFT                     =\n");
	printf("==============================\n");

	if(!this->isStarted){
		puts("Error: initFT");
		puts("- Sensor has not been started");
		this->lockFlg=false;
		printf("==============================\n\n");
		return false;
	}

	OHNO_UTILITIES::Timer tTimer;
	tTimer.setStartTime();
	puts("ready...");
	puts("3");
	while(1){
		tTimer.setEndTime();
		if(tTimer.getDeltaTime()>1)	break;

		if(!this->capturing()){
			puts("Error: initFT");
			puts("- cannot capture");
			this->lockFlg=false;
			printf("==============================\n\n");
			return false;
		}
		cv::imshow(this->sensorName,this->curFrame);
		cv::waitKey(1);
	}
	puts("2");
	tTimer.setStartTime();
	while(1){
		tTimer.setEndTime();
		if(tTimer.getDeltaTime()>1)	break;

		if(!this->capturing()){
			puts("Error: initFT");
			puts("- cannot capture");
			this->lockFlg=false;
			printf("==============================\n\n");
			return false;
		}
		cv::imshow(this->sensorName,this->curFrame);
		cv::waitKey(1);
	}
	puts("1");
	tTimer.setStartTime();
	while(1){
		tTimer.setEndTime();
		if(tTimer.getDeltaTime()>1)	break;

		if(!this->capturing()){
			puts("Error: initFT");
			puts("- cannot capture");
			this->lockFlg=false;
			printf("==============================\n\n");
			return false;
		}
		cv::imshow(this->sensorName,this->curFrame);
		cv::waitKey(1);
	}

	if(this->FT->init(this->curFrame))
		this->finger.isTracked = true;
	else
		this->finger.isTracked = false;

	this->lockFlg = false;
	this->initSensor();
	printf("------------------------------\n");
	printf("= initFT: done               =\n");
	printf("==============================\n\n");
	return this->finger.isTracked;
}
bool ForceSensor::fingerTracking()
{
	while(this->isLocked()){}
	if(!this->finger.isTracked)	return false;

	cv::Mat frame;
	this->curFrame.copyTo(frame);

	if(!this->FT->tracking(frame))
		return false;
	return true;
}
bool ForceSensor::run()
{
	while(this->isLocked()){}

	if(!this->isStarted){
		puts("Error: calibration");
		puts("- Force Sensor has not been started yet");
		return false;
	}

	/*=== キャプチャ ===*/
	this->ST.setStartTime();//1st period
	if(!this->capturing()){
		puts("Error: run");
		puts("- capturing");
		return false;
	}
	cv::imshow(this->sensorName.c_str(),curFrame);
	cv::waitKey(1);

	/*=== キー入力 ===*/
	if(this->keyboard()){
		puts("Warning: run");
		puts("- Sensor has been recieved keyboard input");
		return false;
	}

	if(!this->finger.isTracked){
		puts("Warning: run");
		puts("- FingerTracking has not been initialized");
		return false;
	}

	/*=== 指先位置の検出 ===*/
	this->ST.setSplitTime();//2nd period
	//if(!this->fingerTracking()){
	//	puts("Error: run");
	//	puts("- fingerTracking");
	//	return false;
	//}
	if(!this->fingerDetection()){
		puts("Error: run");
		puts("- fingerDetection");
		return false;
	}
	cv::Point2i p1(0,0);
	cv::Point2i p2(this->FT->getInitTmpSize().width,0);
	cv::Point2i p3(this->FT->getInitTmpSize().width,this->FT->getInitTmpSize().height);
	cv::Point2i p4(0,this->FT->getInitTmpSize().height);
	cv::Point P1=getProjectedPoint(p1,this->finger.G);
	cv::Point P2=getProjectedPoint(p2,this->finger.G);
	cv::Point P3=getProjectedPoint(p3,this->finger.G);
	cv::Point P4=getProjectedPoint(p4,this->finger.G);
	cv::Mat tmp;
	this->curFrame.copyTo(tmp);
	for(int h=0; h<this->curFrame.size().height; h++){
		for(int w=0; w<this->curFrame.size().width; w++){
			cv::Vec3b color=tmp.at<cv::Vec3b>(h,w);
			if(this->maskOfFinger.at<uchar>(h,w)==0)
				tmp.at<cv::Vec3b>(h,w)=color/3;
		}
	}
	cv::line(tmp,P1,P2,cv::Scalar(255,0,0));
	cv::line(tmp,P2,P3,cv::Scalar(255,0,0));
	cv::line(tmp,P3,P4,cv::Scalar(255,0,0));
	cv::line(tmp,P4,P1,cv::Scalar(255,0,0));
	CV_GRAPHICS::cross(tmp,this->finger.actionPoint,cv::Scalar(0,0,255));
	cv::imshow("test",tmp);
	cv::waitKey(1);

	/*=== 肌領域の抽出 ===*/
	this->ST.setSplitTime();//3rd period
	if(!this->skinExtraction()){
		puts("Error: run");
		puts("- skinExtraction");
		return false;
	}
	//cv::imshow("test",this->maskOfSkin);
	//cv::waitKey(0);

	/*=== オプティカルフローの計算 ===*/
	this->ST.setSplitTime();//4th period
	if(!this->opticalFlowCalculation()){
		puts("Error: run");
		puts("- opticalFlowCalculation");
		return false;
	}
	//cv::Mat tmp;
	//curFrame.copyTo(tmp);
	//for(int h=0; h<this->curFrame.size().height; h+=10){
	//	for(int w=0; w<this->curFrame.size().width; w+=10){
	//		cv::Point p(w,h);
	//		if(this->maskOfSkin(p)!=0)
	//			CV_GRAPHICS::arrow(tmp,p,this->OF->getFlowVectorAt(p),cv::Scalar(255,0,0));			
	//	}
	//}
	//cv::imshow("test",tmp);
	//cv::waitKey(1);

	/*=== 射影変換の取得 ===*/
	this->ST.setSplitTime();//5th period
	if(!this->homographyCalculation()){
		puts("Error: run");
		puts("- homogprahyCalculation");
		return false;
	}
	if(!this->compensationFlg){//tmp
		this->H=cv::Mat::eye(3,3,CV_64F);
		this->dH=cv::Mat::eye(3,3,CV_64F);
	}

	/*=== フレーム間特徴量の取得 ===*/
	this->ST.setSplitTime();//6th period
	if(!this->forceEstimation()){
		puts("Error: run");
		puts("- forceExtimation");
		return false;
	}

	/*=== 出力 ===*/
	force[0] += dForce[0];
	force[1] += dForce[1];
	force[2] += dForce[2];
	//ホモグラフィ変換行列の更新
	this->H = this->dH*this->H;

	if(this->renderingFlg)	this->render();
	if(this->timerFlg)	this->ST.showSplitTimes();

	this->ST.setEndTime();
	
	return true;
}
void ForceSensor::showInfo()
{
	printf("==============================\n");
	printf("= showInfo                   =\n");
	printf("==============================\n");
	std::cout<<"--- parameters ---"<<std::endl;
	std::cout<<"resizeRatio\t\t: "<<this->resizeRatio<<std::endl;
	std::cout<<"vectorsOnCircleNum\t: "<<this->vectorsOnCircleNum<<std::endl;
	std::cout<<"vectorsOnRadiusNum\t: "<<this->vectorsOnRadiusNum<<std::endl;
	std::cout<<"referencePointsNum\t: "<<this->referencePointsNum<<std::endl;
	std::cout<<"proportionOfCircle\t: "<<this->proportionOfCircle<<std::endl;
	std::cout<<"proportionOfLocalDisp\t: "<<this->proportionOfLocalDisp<<std::endl;
	std::cout<<"flowGain\t: "<<this->flowGain<<std::endl;
	std::cout<<std::endl;
	std::cout<<"--- modules ---"<<std::endl;
	this->OF->showInfo();
	this->RE_skin->showInfo();
	printf("------------------------------\n");
	printf("= showInfo: done             =\n");
	printf("==============================\n");
}
void ForceSensor::end()
{
	while(this->isLocked()){}
	if(this->isStarted){
		delete[] this->referencePoints;
		delete[] this->isOnSkin;
	}
	this->deleteModules();
	exit(0);
}

/*--- アクセッサ ---*/
/* set:パラメータ（必ず最後にinitが実行される） */
void ForceSensor::setResizeRatio(unsigned int _value)
{
	if(this->isStarted && 0<_value &&	_value<=100){
		this->resizeRatio=_value;
		this->initSensor();
	}
}
void ForceSensor::setVectorsOnCircleNum(unsigned int _value)
{
	if(this->isStarted){
		this->vectorsOnCircleNum = _value;
		this->initSensor();
	}
}
void ForceSensor::setVectorsOnRadiusNum(unsigned int _value)
{
	if(this->isStarted){
		this->vectorsOnRadiusNum = _value;
		this->initSensor();
	}
}
void ForceSensor::setProportionOfCircle(unsigned int _value)
{
	if(this->isStarted){
		this->proportionOfCircle = _value;
		this->initSensor();
	}
}
void ForceSensor::setProportionOfLocalDisp(unsigned int _value)
{
	if(this->isStarted){
		this->proportionOfLocalDisp = _value;
		this->initSensor();
	}
}
void ForceSensor::setFlowGain(double _flowGain)
{
	if(this->isStarted){
		this->flowGain = _flowGain;
		this->initSensor();
	}
}
/* set*/
void ForceSensor::setFinger(ForceSensor::Finger _finger)
{
	this->finger = _finger;
}
void ForceSensor::setDH(cv::Mat _dH)
{
	_dH.copyTo(this->dH);
}
void ForceSensor::setH(cv::Mat _H)
{
	_H.copyTo(this->H);
}
void ForceSensor::setRendering(bool _flg)
{
	this->renderingFlg=_flg;
}
void ForceSensor::setCompensation(bool _flg)
{
	this->compensationFlg=_flg;
}
/* get */
Capture* ForceSensor::getCap()
{
	return this->cap;
}
OpticalFlow* ForceSensor::getOF()
{
	return this->OF;
}
RegionExtraction_multiColor* ForceSensor::getRE_skin()
{
	return this->RE_skin;
}
unsigned int ForceSensor::getResizeRatio()
{
	return this->resizeRatio;
}
unsigned int ForceSensor::getVectorsOnCircleNum()
{
	return this->vectorsOnCircleNum;
}
unsigned int ForceSensor::getVectorsOnRadiusNum()
{
	return this->vectorsOnRadiusNum;
}
unsigned int ForceSensor::getReferencePointsNum()
{
	return this->referencePointsNum;
}
unsigned int ForceSensor::getProportionOfCircle()
{
	return this->proportionOfCircle;
}
unsigned int ForceSensor::getProportionOfLocalDisp()
{
	return this->proportionOfLocalDisp;
}
double ForceSensor::getFlowGain()
{
	return this->flowGain;
}
cv::Vec3f ForceSensor::getForce()
{
	return this->force;
}
cv::Mat ForceSensor::getCurFrame()
{
	return this->curFrame;
}
cv::Mat ForceSensor::getResultImage()
{
	return this->resultImage;
}
cv::Mat ForceSensor::getFlowImage()
{
	return this->flowImage;
}
cv::Mat ForceSensor::getFlowImageInOriginalPlane()
{
	return this->flowImageInOriginalPlane;
}
bool ForceSensor::isLocked()
{
	return this->lockFlg;
}

/*=== 関数 ===*/
cv::Mat_<uchar> PALM_ACT::makeFingerMask(cv::Point2i _source, double _angle, cv::Size2i _maskSize, unsigned int _thickness)
{
	cv::Mat_<uchar> maskImage(_maskSize,255);
	cv::Rect maskRect(0,0,_maskSize.width,_maskSize.height);

	cv::Point2d d(cos(_angle), sin(_angle));
	cv::Point2d n(d.y, -d.x);

	cv::Point2d p(_source.x,_source.y);
	cv::Point2d q;

	bool flag=true;
	while(flag){
		flag=false;
		if(p.inside(maskRect)){
			flag=true;
			maskImage(p.y,p.x)=0;
		}
		q=cv::Point2d(p.x,p.y);
		while(cv::norm(p-q)<_thickness/2){
			if(q.inside(maskRect)){
				flag=true;
				maskImage(q.y,q.x)=0;
			}
			q = q+n;
		}
		q=cv::Point2d(p.x,p.y);
		while(cv::norm(p-q)<_thickness/2){
			if(q.inside(maskRect)){
				flag=true;
				maskImage(q.y,q.x)=0;
			}
			q = q-n;
		}
		p = p+d;
	}

	p=cv::Point2d(_source.x,_source.y);
	flag=true;
	while(flag){
		flag=false;
		if(p.inside(maskRect)){
			flag=true;
			maskImage(p.y,p.x)=0;
		}
		q=cv::Point2d(p.x,p.y);
		while(cv::norm(p-q)<_thickness/2){
			if(q.inside(maskRect)){
				flag=true;
				maskImage(q.y,q.x)=0;
			}
			q = q+n;
		}
		q=cv::Point2d(p.x,p.y);
		while(cv::norm(p-q)<_thickness/2){
			if(q.inside(maskRect)){
				flag=true;
				maskImage(q.y,q.x)=0;
			}
			q = q-n;
		}
		p = p-d;
	}

	cv::erode(maskImage,maskImage,cv::Mat(),cv::Point2i(),3);
	cv::dilate(maskImage,maskImage,cv::Mat(),cv::Point2i(),3);

	return maskImage;
}
cv::Point2f PALM_ACT::getProjectedPoint(cv::Point2f _p, cv::Mat_<double> _H){
	double X = _H(0,0)*_p.x + _H(0,1)*_p.y + _H(0,2);
	double Y = _H(1,0)*_p.x + _H(1,1)*_p.y + _H(1,2);
	double Z = _H(2,0)*_p.x + _H(2,1)*_p.y + _H(2,2);

	return cv::Point2f(X/Z, Y/Z);
}
cv::Mat PALM_ACT::getProjectedImage(cv::Mat _originalImage, cv::Mat_<double> _H){
	cv::Mat_<cv::Vec3b> resultImage(_originalImage.size(),cv::Vec3b(0,0,0));

	cv::Mat invH;
	cv::invert(_H,invH);
	for(int h=0; h<_originalImage.size().height; h++){
		for(int w=0; w<_originalImage.size().width; w++){
			cv::Point p(w,h);
			cv::Point q=getProjectedPoint(p,invH);
			if(q.inside(cv::Rect(0,0,_originalImage.size().width,_originalImage.size().height))){
				resultImage.at<cv::Vec3b>(p)[0]=_originalImage.at<cv::Vec3b>(q)[0];
				resultImage.at<cv::Vec3b>(p)[1]=_originalImage.at<cv::Vec3b>(q)[1];
				resultImage.at<cv::Vec3b>(p)[2]=_originalImage.at<cv::Vec3b>(q)[2];
			}
		}
	}
	return resultImage;
}

}
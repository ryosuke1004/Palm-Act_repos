#pragma once

#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "../utilities/cvGraphics.h"

// バージョン取得
#define CV_VERSION_STR CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)
// ビルドモード
#ifdef _DEBUG
#define CV_EXT_STR "d.lib"
#else
#define CV_EXT_STR ".lib"
#endif
#pragma comment(lib, "opencv_core"            CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, "opencv_imgproc"  CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, "opencv_video"  CV_VERSION_STR CV_EXT_STR)


namespace OPTICAL_FLOW{
	/* === class : OpticalFlow === */
	class OpticalFlow{
	public:
		//仮想デストラクタ
		virtual ~OpticalFlow(){}

		/*--- インタフェース ---*/
		//I:_nameのモジュールを読み込む
		virtual bool loadModule(std::string _name)=0;
		//I:_nameという名前でモジュールを保存
		virtual bool saveModule(std::string _name)=0;
		//I:パラメータを表示
		virtual void showInfo()=0;
		//I:_prevFrameから_curFrameへのフロー場を計算し、_flowFieldに格納
		virtual bool calcFlowField(cv::Mat _prevFrame, cv::Mat _curFrame, cv::Mat_<cv::Point2f>& _flowField)=0;
		//I:_prevFrameから_curFrameへのフロー場を計算し、flowFieldに格納
		virtual bool calcFlowField(cv::Mat _prevFrame, cv::Mat _curFrame)=0;
		//I:_refPointsNum個の参照点の配列_refPointsにおけるフローベクトルをバッファに格納
		virtual bool calcFlowVectors(cv::Point2i* _refPoints, unsigned int _refPointsNum)=0;
		////I:フローベクトルの可視化
		//virtual bool renderFlowVectors(cv::Mat& _inputImage, float _maxNorm, unsigned int _thickness, cv::Mat_<uchar> _mask = cv::Mat())=0;

		//A:モジュールの名前を返す
		virtual std::string getModuleName()=0;
		//A:_prmNoパラメータのポインタを返す
		virtual void* getPrmPtr(unsigned int _OPTICAL_FLOW_PRM)=0;
		//A:flowFieldを取得
		virtual cv::Mat getFlowField()=0;
		//A:点_refPointにおけるフローベクトルを取得
		virtual cv::Point2f getFlowVectorAt(cv::Point2i _refPoint)=0;
		//A:フローベクトルの数を取得
		virtual unsigned int getFlowVectorsNum()=0;
		//A:フローベクトルの先頭ポインタを取得
		virtual cv::Point2f* getFlowVectorsPtr()=0;
		//A:参照点の先頭ポインタを取得
		virtual cv::Point2i* getRefPointsPtr()=0;
	};
};

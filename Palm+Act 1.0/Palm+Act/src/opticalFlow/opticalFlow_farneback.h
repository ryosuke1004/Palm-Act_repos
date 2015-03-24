#pragma once
#include "opticalFlow.h"

using namespace OPTICAL_FLOW;
namespace OPTICAL_FLOW_FARNEBACK{

	enum OPTICAL_FLOW_PRM{
		PYR_SCALE,
		LEVELS,
		WINDOW_SIZE,
		ITERATIONS,
		POLY_N,
		POLY_SIGMA,
		FLAGS
	};

	/* === class : OpticalFlow_farneback === */
	class OpticalFlow_farneback : public OpticalFlow{
	private:
		/*--- モジュールの情報 ---*/
		std::string name;//モジュールの名前
		struct FlowPrm{
			//画像ピラミッドのスケール、0.5は古典的画像ピラミッド
			double pyrScale;
			//画像ピラミッドの層の数
			int levels;
			//平均化窓サイズ
			int windowSize;
			//画像ピラミッドの各レベルにおける、アルゴリズムの反復数
			int iterations;
			//ピクセル近傍領域のサイズ、大きいほど滑らかなサーフェイスで近似される
			int polyN;
			//ガウス分布の標準偏差。PolyN=5なら1.1が適当
			double polySigma;
			//
			int flags;
		}flowPrm;//パラメータ

		/*--- 変数 ---*/
		cv::Mat_<cv::Point2f> flowField;//フロー場
		unsigned int flowVectorsNum;//フローベクトルをとる点の数
		cv::Point2i* refPoints;//参照点
		cv::Point2f* flowVectors;//フローベクトル

		/*--- bool変数 ---*/
		bool isCalcFlowField;//フロー場を計算したかどうか
		bool isCalcFlowVectors;//フローベクトルをセットしたかどうか

		//コピー禁止
		OpticalFlow_farneback(const OpticalFlow_farneback& _opticalFlow);
		//コピー禁止
		OpticalFlow_farneback operator=(const OpticalFlow_farneback&);


	public:
		//コンストラクタ
		OpticalFlow_farneback();
		//デストラクタ
		~OpticalFlow_farneback();

		/*--- インタフェース ---*/
		//I:_nameのモジュールを読み込む
		bool loadModule(std::string _name);
		//I:_nameという名前でモジュールを保存する
		bool saveModule(std::string _name);
		//I:パラメータを表示
		void showInfo();
		//I:_prevFrameから_curFrameへのフロー場を計算し、_flowFieldに格納
		bool calcFlowField(cv::Mat _prevFrame, cv::Mat _curFrame, cv::Mat_<cv::Point2f>& _flowField);
		//I:_prevFrameから_curFrameへのフロー場を計算し、flowFieldに格納
		bool calcFlowField(cv::Mat _prevFrame, cv::Mat _curFrame);
		//I:_refPointsNum個の参照点の配列_refPointsにおけるフローベクトルをバッファに格納
		bool calcFlowVectors(cv::Point2i* _refPoints, unsigned int _refPointsNum);
		//I:フローベクトルの可視化
		bool renderFlowVectors(cv::Mat& _inputImage, float _maxNorm, unsigned int _thickness, cv::Mat_<uchar> _mask = cv::Mat());

		/*--- アクセッサ ---*/
		//A:モジュールの名前を返す
		std::string getModuleName();
		//A:_prmNoパラメータのポインタを返す
		void* getPrmPtr(unsigned int _OPTICAL_FLOW_PRM);
		//A:flowFieldを取得
		cv::Mat getFlowField();
		//A:点_refPointにおけるフローベクトルを取得
		cv::Point2f getFlowVectorAt(cv::Point2i _refPoint);
		//A:フローベクトルの数を取得
		unsigned int getFlowVectorsNum();
		//A:フローベクトルの先頭ポインタを取得
		cv::Point2f* getFlowVectorsPtr();
		//A:参照点の先頭ポインタを取得
		cv::Point2i* getRefPointsPtr();

	};
}

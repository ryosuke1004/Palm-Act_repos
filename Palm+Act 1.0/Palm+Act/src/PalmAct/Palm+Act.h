#pragma once

//=== opencvのインクルード ===========================================================================
//・opencvのincludeディレクトリをインクルードディレクトリに指定．
//・opencvのlibディレクトリをライブラリディレクトリに指定．
//====================================================================================================
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
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
#pragma comment(lib, "opencv_calib3d"  CV_VERSION_STR CV_EXT_STR)
//====================================================================================================

//=== .hファイルのインクルード ===================================================================
#include "../utilities/ohnoUtilities.h"
#include "../opticalFlow/opticalFlow_farneback.h"
#include "../regionExtraction/regionExtraction_color.h"
#include "../regionExtraction/regionExtraction_setting.h"
#include "../interfaces/keyboardFunc.h"
#include "../utilities/cvGraphics.h"
#include "../capture/capture_cam.h"
#include "../fingerTracking\fingerTracking.h"
//====================================================================================================

using namespace REGION_EXTRACTION;
using namespace OPTICAL_FLOW_FARNEBACK;
using namespace OHNO_UTILITIES;
using namespace CAPTURE_CAM;

namespace PALM_ACT{
	/*=== class : ForceSensor ===*/
	class ForceSensor{
	private:
		///*--- 名前 ---*/
		std::string sensorName;//ForceSensorの識別名

		/*--- モジュール ---*/
		Capture* cap;//画像取得
		FingerTracking* FT;//指先追跡
		RegionExtraction_multiColor* RE_skin;//肌色領域抽出
		OpticalFlow* OF;//オプティカルフロー計算
		OHNO_UTILITIES::SplitTimer ST;//処理時間計算

		/*--- パラメータ ---*/
		unsigned int resizeRatio;//画像縮小率(%)
		unsigned int vectorsOnCircleNum;//同心円上の点の数
		unsigned int vectorsOnRadiusNum;//動径方向の点の数
		unsigned int referencePointsNum;//点の数
		unsigned int proportionOfCircle;//画像中における円の割合(%)
		unsigned int proportionOfLocalDisp;//画像中における掌の変形による変位が及ぶ範囲の割合(%)
		double flowGain;//オプティカルフローのゲイン

		/*--- 変数 ---*/
		/* 画像 */
		cv::Mat_<cv::Vec3b> initFrame;//初期フレーム
		cv::Mat_<cv::Vec3b> prevFrame;//一つ前のフレーム
		cv::Mat originalFrame;//現在のフレーム（オリジナルサイズ）
		cv::Mat_<cv::Vec3b> curFrame;//現在のフレーム
		/* マスク */
		cv::Mat_<uchar> maskOfFinger;//指領域のマスク
		cv::Mat_<uchar> maskOfSkin;//肌領域のマスク
		/* 指 */
		struct Finger{
			cv::Point2i actionPoint;//力の作用点
			double angle;//画像平面のx軸からの指の偏角
			unsigned int thickness;//指の太さ
			cv::Mat G;//指先の変換行列
			bool isTracked;//指が追跡されている
		}finger;//指先
		/* 参照点 */
		cv::Point2f* referencePoints;//相対参照点
		bool* isOnSkin;//参照点が肌領域上にあるかどうか
		/* その他の変数 */
		cv::Mat_<double> H;//初期フレートと現在のフレーム間での掌平面の射影変換行列(3,3,CV_64FC1)
		cv::Mat_<double> dH;//一つ前のフレートと現在のフレーム間での掌平面の射影変換行列(3,3,CV_64FC1)
		unsigned int onSkinVectorsNum;//肌領域上の参照点の数
		cv::Vec3f dForce;//フレーム間の力
		/* フラグ */
		bool isStarted;//センサが正しく起動されているかどうかのフラグ
		bool renderingFlg;//処理結果描画モードかどうかフラグ
		bool timerFlg;//処理時間を計測するかどうかフラグ
		bool lockFlg;//並列処理用
		bool initFlg;//初期化命令のフラグ
		bool compensationFlg;//掌の動きを補正するかどうか（掌が固定されている場合はfalse）
		/* 出力 */
		cv::Vec3f force;//力ベクトル
		cv::Mat resultImage;//処理結果画像
		cv::Mat flowImage;//現在のフレームにおけるオプティカルフローの様子を描画
		cv::Mat flowImageInOriginalPlane;//初期フレームでの掌の位置姿勢におけるオプティカルフローの様子を描画

		/*--- 関数 ---*/
		//モジュールの削除
		void deleteModules();
		//キャプチャ
		bool capturing();
		//肌色領域抽出
		bool skinExtraction();
		//力の作用点の計算
		cv::Point2i calcActionPoint(cv::Mat _G);
		//指の太さの計算
		double calcFingerThickness(cv::Mat _G);
		//指の向きの計算
		double calcFingerAngle(cv::Mat _G);
		//指の状態計算
		bool fingerDetection();
		//オプティカルフロー計算
		bool opticalFlowCalculation();
		//ホモグラフィ変換計算
		bool homographyCalculation();
		//力推定
		bool forceEstimation();
		//処理結果の描画する
		bool render();
		//処理時間を表示する
		void showTimer();
		//キーボードインタフェース
		bool keyboard();

		//コピー禁止
		ForceSensor(const ForceSensor& _forceSensor);
		//コピー禁止
		ForceSensor operator=(const ForceSensor&);

	public:
		//コンストラクタ
		ForceSensor(char* _sensorName=NULL);
		//デストラクタ
		~ForceSensor();

		/*--- インタフェース ---*/
		//センサの設定を全て初期化する
		bool initSensor();
		//_sensorNameという名前のセンサを読み込む
		bool loadSensor(std::string _sensorName);
		//_sensorNameという名前でセンサを保存する
		bool saveSensor(std::string _sensorName);
		//_sensorNameというセンサのパラメータを設定する
		bool prmSetting();
		//指トラッキングの初期化
		bool initFT();
		//指トラッキング
		bool fingerTracking();
		//センシングする
		bool run();
		//デバイスの情報を提示する
		void showInfo();
		//終了処理
		void end();

		/*--- アクセッサ ---*/
		/* set:パラメータ（必ず最後にinitが実行される） */
		void setResizeRatio(unsigned int _value);
		void setVectorsOnCircleNum(unsigned int _value);
		void setVectorsOnRadiusNum(unsigned int _value);
		void setProportionOfCircle(unsigned int _value);
		void setProportionOfLocalDisp(unsigned int _value);
		void setFlowGain(double _flowGain);
		/* set:変数（マルチスレッド化する時に利用する）*/
		void setFinger(ForceSensor::Finger _finger);
		void setDH(cv::Mat _dH);
		void setH(cv::Mat _H);
		void setRendering(bool _flg);
		void setCompensation(bool _flg);
		/* get */
		Capture* getCap();
		OpticalFlow* getOF();
		RegionExtraction_multiColor* getRE_skin();
		unsigned int getResizeRatio();
		unsigned int getVectorsOnCircleNum();
		unsigned int getVectorsOnRadiusNum();
		unsigned int getReferencePointsNum();
		unsigned int getProportionOfCircle();
		unsigned int getProportionOfLocalDisp();
		double getFlowGain();
		cv::Vec3f getForce();
		cv::Mat getCurFrame();
		cv::Mat getResultImage();
		cv::Mat getFlowImage();
		cv::Mat getFlowImageInOriginalPlane();
		bool isLocked();
	};

	/*=== 関数 ===*/
	//_sourceから_targetへ向かうベクトル上に画像サイズ_maskSizeで_thicknessの太さの指マスクを作成
	cv::Mat_<uchar> makeFingerMask(cv::Point2i _source, double _angle, cv::Size2i _maskSize, unsigned int _thickness);
	//射影変換行列_Hによる点_pの射影変換
	cv::Point2f getProjectedPoint(cv::Point2f _p, cv::Mat_<double> _H);
	//射影変換行列_Hによる画像_imageの射影変換
	cv::Mat getProjectedImage(cv::Mat _image, cv::Mat_<double> _H);
};

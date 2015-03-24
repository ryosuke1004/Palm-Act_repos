#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/opencv.hpp>

// バージョン取得
#define CV_VERSION_STR CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)
// ビルドモード
#ifdef _DEBUG
#define CV_EXT_STR "d.lib"
#else
#define CV_EXT_STR ".lib"
#endif
// ライブラリのリンク（不要な物はコメントアウト）
#pragma comment(lib, "opencv_core"            CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, "opencv_highgui"        CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, "opencv_imgproc"  CV_VERSION_STR CV_EXT_STR)


namespace CAPTURE{
	class Capture{
	public:
		//仮想デストラクタ
		virtual ~Capture(){}

		/* インタフェース */
		//I:キャプチャを開始する
		virtual bool startCapture(unsigned int _sourceNo)=0;
		//I:キャプチャを初期化する
		virtual bool initCapture()=0;
		//I:バッファにキャプチャしたデータを格納
		virtual cv::Mat capture(unsigned char* _buf)=0;
		//I:キャプチャしたデータをMatとして返す
		virtual cv::Mat capture()=0;
		//I:取得できる画像情報を表示する
		virtual bool showInfo()=0;
		//I:キャプチャを終了する
		virtual bool endCapture()=0;

		/*アクセッサ*/
		//A:カメラの情報を返す
		virtual void* getCaptureInfo(unsigned int _CAP_INFO)=0;
		//A:キャプチャが正常にスタートしているかどうか
		virtual bool isStarted()=0;
	};
};

#pragma once

#include "capture.h"
#include "../utilities/ohnoUtilities.h"

using namespace CAPTURE;
namespace CAPTURE_AVI{
	enum CAP_INFO{
		NAME,
		ROWS,
		COLS,
		CHANNELS,
		DATASIZE,
		FPS,
		FRAMES
	};

	class Capture_avi : public Capture{
	private:
		cv::VideoCapture cap;
		struct{
			char name[256];
			unsigned int rows;
			unsigned int cols;
			unsigned int channels;
			unsigned int dataSize;
			unsigned int fps;
			unsigned int frames;
		}info;//ビデオ情報
		OHNO_UTILITIES::Timer timer;//タイマー
		bool flgStarted;//カメラと接続されているかどうか

	public:
		//コンストラクタ
		Capture_avi();
		//コピー禁止
		Capture_avi(const Capture_avi& obj);
		//デストラクタ
		~Capture_avi();

		//I:キャプチャを開始する
		bool startCapture(unsigned int _sourceNo);
		//I:キャプチャを初期化する
		bool initCapture();
		//I:バッファにキャプチャしたデータを格納
		cv::Mat capture(unsigned char* _buf);
		//I:キャプチャしたデータをMatとして返す
		cv::Mat capture();
		//I:取得できる画像情報を表示する
		bool showInfo();
		//I:キャプチャを終了する
		bool endCapture();

		/*アクセッサ*/
		//A:カメラの情報を返す
		void* getCaptureInfo(unsigned int _CAP_INFO);
		//A:キャプチャが正常にスタートしているかどうか
		bool isStarted();
	};
}
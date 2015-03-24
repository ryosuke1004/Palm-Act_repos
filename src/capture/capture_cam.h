#pragma once

#include "capture.h"
#include "../utilities/ohnoUtilities.h"

using namespace CAPTURE;
namespace CAPTURE_CAM{
	enum CAP_INFO{
		ROWS,
		COLS,
		CHANNELS,
		DATASIZE,
		FPS,
		BRIGHTNESS,
		CONTRAST,
		SATURATION,
		HUE,
		GAIN,
		EXPOSURE
	};
	class Capture_cam : public Capture{
	private:
		cv::VideoCapture cap;
		unsigned int sourceNo;//カメラNo
		struct{
			unsigned int rows;
			unsigned int cols;
			unsigned int channels;
			unsigned int dataSize;
			unsigned int fps;
			double brightness;
			double contrast;
			double saturation;
			double hue;
			double gain;
			double exposure;
		}info;//カメラ情報
		OHNO_UTILITIES::Timer timer;//タイマー
		bool flgStarted;//カメラと接続されているかどうか

	public:
		//コンストラクタ
		Capture_cam();
		//コピー禁止
		Capture_cam(const Capture_cam& obj);
		//デストラクタ
		~Capture_cam();

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

#pragma once
#include <opencv2\opencv.hpp>
#include <fstream>

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

namespace TEMPLATE_MATCHING_STANDARD{
	/*=== 定数 ===*/
	static double cosSimThr = 0.995;//追跡失敗判定の閾値
	/*=== class: TemplateMatching_standard ===*/
	class TemplateMatching_standard{
	private:
		cv::Mat_<uchar> tmpImage;//テンプレート画像
		cv::Size2i tmpSize;//テンプレートのサイズ
		unsigned int samplingInterval;//テンプレートのサンプリング間隔
		int tmpPixels;//テンプレートの画素数
		cv::Rect rect_roi;//画像中のテンプレートと一致する領域
		//double v1,v2,vv;
		double cosSim;//コサイン類似度
		bool lost;//見失ったかどうか
		cv::Mat resultImage;//結果を描画した画像
		
	public:
		//コンストラクタ
		TemplateMatching_standard(cv::Mat _tmpImage=cv::Mat(), int _samplingInterval=0);
		//デストラクタ
		~TemplateMatching_standard();

		/*--- インタフェース ---*/
		//初期化 テンプレート_tmpImageを指定
		bool init(cv::Mat _tmpImage);
		//参照画像_refImageから，テンプレートを切り出して指定する．
		bool makeTmpImage(cv::Mat _refImage);
		//画像_curImageとテンプレートマッチングの結果得られる，対応する画像領域を返す
		cv::Mat matching(cv::Mat _curImage);

		/*--- アクセッサ ---*/
		cv::Mat getTmpImage();
		cv::Size getTmpSize();
		int getSamplingInterval();
		int getTmpPixels();
		cv::Rect getRectRoi();
		double getCosSim();
		bool isLost();
		cv::Mat getResultImage();
		void setSamplingInterval(unsigned int _samplingInterval);
		void setRectRoi(cv::Rect _rect_roi);
	};
};
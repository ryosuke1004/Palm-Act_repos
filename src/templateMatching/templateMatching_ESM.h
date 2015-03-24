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

namespace TEMPLATE_MATCHING_ESM{

	/*=== 定数 ===*/
	static double cosSimThr = 0.99;//繰り返し終了判定用の閾値
	static unsigned int countThr = 40;//繰り返し回数の制限
	static cv::Mat_<double> A1=(cv::Mat_<double>(3,3)<<0,0,1,0,0,0,0,0,0);//リー代数の基底行列
	static cv::Mat_<double> A2=(cv::Mat_<double>(3,3)<<0,0,0,0,0,1,0,0,0);//リー代数の基底行列
	static cv::Mat_<double> A3=(cv::Mat_<double>(3,3)<<0,1,0,0,0,0,0,0,0);//リー代数の基底行列
	static cv::Mat_<double> A4=(cv::Mat_<double>(3,3)<<0,0,0,1,0,0,0,0,0);//リー代数の基底行列
	static cv::Mat_<double> A5=(cv::Mat_<double>(3,3)<<1,0,0,0,-1,0,0,0,0);//リー代数の基底行列
	static cv::Mat_<double> A6=(cv::Mat_<double>(3,3)<<0,0,0,0,-1,0,0,0,1);//リー代数の基底行列
	static cv::Mat_<double> A7=(cv::Mat_<double>(3,3)<<0,0,0,0,0,0,1,0,0);//リー代数の基底行列
	static cv::Mat_<double> A8=(cv::Mat_<double>(3,3)<<0,0,0,0,0,0,0,1,0);//リー代数の基底行列
	static cv::Mat_<double> J_G=(cv::Mat_<double>(9,8) <<
						A1(0),A2(0),A3(0),A4(0),A5(0),A6(0),A7(0),A8(0),
						A1(1),A2(1),A3(1),A4(1),A5(1),A6(1),A7(1),A8(1),
						A1(2),A2(2),A3(2),A4(2),A5(2),A6(2),A7(2),A8(2),
						A1(3),A2(3),A3(3),A4(3),A5(3),A6(3),A7(3),A8(3),
						A1(4),A2(4),A3(4),A4(4),A5(4),A6(4),A7(4),A8(4),
						A1(5),A2(5),A3(5),A4(5),A5(5),A6(5),A7(5),A8(5),
						A1(6),A2(6),A3(6),A4(6),A5(6),A6(6),A7(6),A8(6),
						A1(7),A2(7),A3(7),A4(7),A5(7),A6(7),A7(7),A8(7),
						A1(8),A2(8),A3(8),A4(8),A5(8),A6(8),A7(8),A8(8));//ヤコビアン
	enum G_TYPE{HOMOGRAPHY,};//変換行列のタイプ
	enum DERIVATION_TYPE{SOBEL,};//画像の勾配の計算方法

	/*=== 関数 ===*/
	//変換_Gによる点_pの変換
	cv::Point warpPoint(cv::Point _p, cv::Mat _G, G_TYPE _TYPE=HOMOGRAPHY);
	//画像_imgの点warpPoint(_p,_G)における勾配を計算
	cv::Point derivation(cv::Mat _img, cv::Point _p, cv::Mat _G=cv::Mat::eye(3,3,CV_64F), DERIVATION_TYPE _TYPE=SOBEL);
	//８次元のパラメータベクトル_xから，ホモグラフィ変換行列を計算
	cv::Mat getHomography(cv::Mat _x);

	/*=== class: TemplateMatching_ESM ===*/
	class TemplateMatching_ESM{
	private:
		cv::Mat_<uchar> tmpImage;//テンプレート画像
		cv::Size2i tmpSize;//テンプレートのサイズ
		int samplingInterval;//テンプレートのサンプリング間隔
		int tmpPixels;//テンプレートの画素数
		cv::Mat_<double> G;//近似変換行列
		cv::Mat_<double> deltaX;//更新する変換行列のパラメータ
		std::vector<cv::Mat_<double>> J_tmpI;//テンプレート画像のヤコビアン
		std::vector<cv::Mat_<double>> J_wG;//その他，必要なヤコビアン
		double v1,v2,vv;
		double cosSim;//コサイン類似度
		cv::Mat resultImage;//切り出した画像

		//コピー禁止
		TemplateMatching_ESM(const TemplateMatching_ESM& obj);

	public:
		//コンストラクタ テンプレート_tmpImageとホモグラフィの初期値_Gを指定
		TemplateMatching_ESM(cv::Mat _tmpImage=cv::Mat(), cv::Mat _G=cv::Mat(), int _samplingInterval=0);
		//デストラクタ
		~TemplateMatching_ESM();

		/*--- インタフェース ---*/
		//初期化 テンプレート_tmpImageとホモグラフィの初期値_Gを指定
		bool init(cv::Mat _tmpImage, cv::Mat _G=cv::Mat());
		//参照画像_refImageから，テンプレートを切り出して指定する．
		bool makeTmpImage(cv::Mat _refImage);
		//画像_curImageとテンプレートマッチングの結果得られる，対応する画像領域を返す
		cv::Mat matching(cv::Mat _curImage);

		/*--- アクセッサ ---*/
		cv::Mat getTmpImage();
		cv::Size getTmpSize();
		int getSamplingInterval();
		int getTmpPixels();
		cv::Mat getG();
		double getCosSim();
		cv::Mat getResultImage();
		bool setSamplingInterval(int _interval);
		void setG(cv::Mat _G);

		/*--- その他関数 ---*/
	};
};

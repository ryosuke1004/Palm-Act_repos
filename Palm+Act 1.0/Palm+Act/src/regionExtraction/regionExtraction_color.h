#pragma once

#include <cstdio>
#include <fstream>

#include <opencv2/opencv.hpp>
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

namespace REGION_EXTRACTION{
	/*=== methods ===*/
	enum REGION_EXTRACTION_METHOD{
		THRESHOLD, THRESHOLD_INV,
		OTSU, OTSU_INV,
	};

	/*=== morphology ===*/
	enum MORPHOLOGY{
		OFF=0,
		OPENING=1,
		CLOSING=2,
	};

	void initDisplay(int val=0, void* ptr=NULL);

	struct Morphology{
		MORPHOLOGY type;
		unsigned int num;
	};

	/*=== class : RegionExtraction_color ===*/
	class RegionExtraction_color{
	private:
		/*--- モジュールの設定 ---*/
		std::string moduleName;//モジュールの名前
		REGION_EXTRACTION_METHOD method;//二値化処理の方法
		unsigned int thrLow;//単純閾値による二値化の際の，下限閾値
		unsigned int thrHigh;//単純閾値による二値化の際の，上限閾値
		Morphology morphology;//モルフォロジー処理の種類，回数

		/*--- 変数 ---*/
		cv::Mat_<uchar> resultImage;//結果画像
		unsigned int pixelsTtl;//抽出した領域の総画素数
		cv::Point2i center;//抽出した領域の重心位置

		/*--- bool変数 ---*/


		//コピー禁止
		RegionExtraction_color(const RegionExtraction_color& _regionExtraction_color);
		//コピー禁止
		RegionExtraction_color operator=(const RegionExtraction_color&);


	public:
		//コンストラクタ
		RegionExtraction_color(
			REGION_EXTRACTION_METHOD _REGION_EXTRACTION_METHOD=THRESHOLD,
			int _thrLow = 0,
			int _thrHigh = 255,
			MORPHOLOGY _type=(MORPHOLOGY)0,
			unsigned int _num=0);
		//デストラクタ
		~RegionExtraction_color();

		/*--- インタフェース ---*/
		//I:_moduleNameという名前のモジュールを読み込む
		bool loadModule(std::string _moduleName);
		//I:_moduleNameという名前でモジュールを保存する
		bool saveModule(std::string _moduleName);
		//I:領域抽出を行う()
		cv::Mat getResult(cv::Mat_<uchar> _inputImage);
		//I:内部情報提示
		void showInfo();

		/*--- アクセッサ ---*/
		//A:二値化処理の方法をセット
		void setMethod(REGION_EXTRACTION_METHOD _method);
		//A:下限閾値をセット
		void setThrLow(unsigned int _thrLow);
		//A:上限閾値をセット
		void setThrHigh(unsigned int _thrHigh);
		//A:モルフォロジー処理をセット
		void setMorphology(MORPHOLOGY _type, unsigned int _num);
		//A:下限閾値を取得
		unsigned int getThrLow();
		//A:上限閾値を取得
		unsigned int getThrHigh();
		//A:モジュール名を取得
		std::string getModuleName();
		//A:モルフォロジー処理を取得
		Morphology getMorphology();
		//A:結果画像を取得
		cv::Mat_<uchar> getResultImage();
		//A:抽出した領域の総画素数を取得
		unsigned int getPixelsTtl();
		//A:抽出した領域の重心位置を取得
		cv::Point2i getCenter();
	};

	/*=== class : RegionExtraction_multiColor ===*/
	class RegionExtraction_multiColor{
	private:
		/*--- モジュールの情報 ---*/
		std::string moduleName;//モジュールの名前
		Morphology morphology;//モルフォロジー処理の種類，回数

		/*--- 変数 ---*/
		cv::Mat_<uchar> resultImage;//結果画像
		unsigned int pixelsTtl;//抽出した領域の総画素数
		cv::Point2i center;//抽出した領域の重心位置

		//コピー禁止
		RegionExtraction_multiColor(const RegionExtraction_multiColor& _regionExtraction_multiColor);
		//コピー禁止
		RegionExtraction_multiColor operator=(const RegionExtraction_multiColor&);

	public:
		REGION_EXTRACTION::RegionExtraction_color RE[3];//各チャンネルの領域抽出モジュール

		//コンストラクタ
		RegionExtraction_multiColor(std::string _moduleName=std::string());
		//デストラクタ
		~RegionExtraction_multiColor();

		/*--- インタフェース ---*/
		//_moduleNameという名前のモジュールを読み込む
		bool loadModule(std::string _moduleName);
		//_moduleNameという名前でモジュールを保存する
		bool saveModule(std::string _moduleName);
		//領域抽出を行う()
		cv::Mat getResult(cv::Mat_<cv::Vec3b> _inputImage);
		//内部情報提示
		void showInfo();

		/*--- アクセッサ ---*/
		//モルフォロジー処理をセット
		void setMorphology(MORPHOLOGY _type, unsigned int _num);
		//モジュール名を取得
		std::string getModuleName();
		//モルフォロジー処理を取得
		Morphology getMorphology();
		//抽出した領域の総画素数を取得
		unsigned int getPixelsTtl();
		//抽出した領域の重心位置を取得
		cv::Point2i getCenter();
		//各領域抽出器へのポインタを取得
		RegionExtraction_color* getRePtr(unsigned int _no);
		//結果画像を取得
		cv::Mat getResultImage();
	};
};

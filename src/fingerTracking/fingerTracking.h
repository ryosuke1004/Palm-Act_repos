#include "../templateMatching/templateMatching_ESM.h"
#include "../templateMatching/templateMatching_standard.h"

class FingerTracking{
private:
	/*--- モジュール ---*/
	TEMPLATE_MATCHING_ESM::TemplateMatching_ESM* TM_ESM;//ESMによるテンプレートマッチングモジュール
	TEMPLATE_MATCHING_STANDARD::TemplateMatching_standard* TM_STD;//標準テンプレートマッチングモジュール

	/*--- 変数 ---*/
	cv::Mat tmpImage;//テンプレートマッチング
	cv::Mat prevImage;//１つ前のフレーム
	cv::Mat initTmpImage;//初期テンプレート
	cv::Size initTmpSize;//初期テンプレートのサイズ
	cv::Rect rect_roi;//標準テンプレートマッチングで得た注目領域
	cv::Mat G;//変換行列
	bool flgRenewTmpImage;//テンプレートを毎回更新するかどうか
	double renewThr;//テンプレートを更新するかどうかの閾値(-1～1)

public:
	//コンストラクタ
	FingerTracking(unsigned int _samplingInterval=0);
	//デストラクタ
	~FingerTracking();

	/*--- インタフェース ---*/
	bool init(cv::Mat _initFrame);
	bool tracking(cv::Mat _curFrame);

	/*--- アクセッサ ---*/
	cv::Mat getTmpImage();
	cv::Mat getprevImage();
	cv::Size getInitTmpSize();
	cv::Mat getG();
	void setFlgRenewTmpImage(bool _flg);
	void setSamplingInterval(unsigned int _samplingInterval);
	void setRenewThr(double _renewThr);
	void setRectRoi(cv::Rect _rectRoi);
	void setG(cv::Mat _G);
};
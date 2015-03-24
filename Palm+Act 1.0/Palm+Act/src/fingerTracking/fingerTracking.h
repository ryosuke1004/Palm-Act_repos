#include "../templateMatching/templateMatching_ESM.h"
#include "../templateMatching/templateMatching_standard.h"

class FingerTracking{
private:
	/*--- ���W���[�� ---*/
	TEMPLATE_MATCHING_ESM::TemplateMatching_ESM* TM_ESM;//ESM�ɂ��e���v���[�g�}�b�`���O���W���[��
	TEMPLATE_MATCHING_STANDARD::TemplateMatching_standard* TM_STD;//�W���e���v���[�g�}�b�`���O���W���[��

	/*--- �ϐ� ---*/
	cv::Mat tmpImage;//�e���v���[�g�}�b�`���O
	cv::Mat prevImage;//�P�O�̃t���[��
	cv::Mat initTmpImage;//�����e���v���[�g
	cv::Size initTmpSize;//�����e���v���[�g�̃T�C�Y
	cv::Rect rect_roi;//�W���e���v���[�g�}�b�`���O�œ������ڗ̈�
	cv::Mat G;//�ϊ��s��
	bool flgRenewTmpImage;//�e���v���[�g�𖈉�X�V���邩�ǂ���
	double renewThr;//�e���v���[�g���X�V���邩�ǂ�����臒l(-1�`1)

public:
	//�R���X�g���N�^
	FingerTracking(unsigned int _samplingInterval=0);
	//�f�X�g���N�^
	~FingerTracking();

	/*--- �C���^�t�F�[�X ---*/
	bool init(cv::Mat _initFrame);
	bool tracking(cv::Mat _curFrame);

	/*--- �A�N�Z�b�T ---*/
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
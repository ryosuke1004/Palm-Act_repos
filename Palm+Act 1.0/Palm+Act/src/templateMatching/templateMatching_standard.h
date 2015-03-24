#pragma once
#include <opencv2\opencv.hpp>
#include <fstream>

// �o�[�W�����擾
#define CV_VERSION_STR CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)
// �r���h���[�h
#ifdef _DEBUG
#define CV_EXT_STR "d.lib"
#else
#define CV_EXT_STR ".lib"
#endif
// ���C�u�����̃����N�i�s�v�ȕ��̓R�����g�A�E�g�j
#pragma comment(lib, "opencv_core"            CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, "opencv_highgui"        CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, "opencv_imgproc"  CV_VERSION_STR CV_EXT_STR)

namespace TEMPLATE_MATCHING_STANDARD{
	/*=== �萔 ===*/
	static double cosSimThr = 0.995;//�ǐՎ��s�����臒l
	/*=== class: TemplateMatching_standard ===*/
	class TemplateMatching_standard{
	private:
		cv::Mat_<uchar> tmpImage;//�e���v���[�g�摜
		cv::Size2i tmpSize;//�e���v���[�g�̃T�C�Y
		unsigned int samplingInterval;//�e���v���[�g�̃T���v�����O�Ԋu
		int tmpPixels;//�e���v���[�g�̉�f��
		cv::Rect rect_roi;//�摜���̃e���v���[�g�ƈ�v����̈�
		//double v1,v2,vv;
		double cosSim;//�R�T�C���ގ��x
		bool lost;//�����������ǂ���
		cv::Mat resultImage;//���ʂ�`�悵���摜
		
	public:
		//�R���X�g���N�^
		TemplateMatching_standard(cv::Mat _tmpImage=cv::Mat(), int _samplingInterval=0);
		//�f�X�g���N�^
		~TemplateMatching_standard();

		/*--- �C���^�t�F�[�X ---*/
		//������ �e���v���[�g_tmpImage���w��
		bool init(cv::Mat _tmpImage);
		//�Q�Ɖ摜_refImage����C�e���v���[�g��؂�o���Ďw�肷��D
		bool makeTmpImage(cv::Mat _refImage);
		//�摜_curImage�ƃe���v���[�g�}�b�`���O�̌��ʓ�����C�Ή�����摜�̈��Ԃ�
		cv::Mat matching(cv::Mat _curImage);

		/*--- �A�N�Z�b�T ---*/
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
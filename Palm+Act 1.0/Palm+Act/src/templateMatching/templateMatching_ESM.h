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

namespace TEMPLATE_MATCHING_ESM{

	/*=== �萔 ===*/
	static double cosSimThr = 0.99;//�J��Ԃ��I������p��臒l
	static unsigned int countThr = 40;//�J��Ԃ��񐔂̐���
	static cv::Mat_<double> A1=(cv::Mat_<double>(3,3)<<0,0,1,0,0,0,0,0,0);//���[�㐔�̊��s��
	static cv::Mat_<double> A2=(cv::Mat_<double>(3,3)<<0,0,0,0,0,1,0,0,0);//���[�㐔�̊��s��
	static cv::Mat_<double> A3=(cv::Mat_<double>(3,3)<<0,1,0,0,0,0,0,0,0);//���[�㐔�̊��s��
	static cv::Mat_<double> A4=(cv::Mat_<double>(3,3)<<0,0,0,1,0,0,0,0,0);//���[�㐔�̊��s��
	static cv::Mat_<double> A5=(cv::Mat_<double>(3,3)<<1,0,0,0,-1,0,0,0,0);//���[�㐔�̊��s��
	static cv::Mat_<double> A6=(cv::Mat_<double>(3,3)<<0,0,0,0,-1,0,0,0,1);//���[�㐔�̊��s��
	static cv::Mat_<double> A7=(cv::Mat_<double>(3,3)<<0,0,0,0,0,0,1,0,0);//���[�㐔�̊��s��
	static cv::Mat_<double> A8=(cv::Mat_<double>(3,3)<<0,0,0,0,0,0,0,1,0);//���[�㐔�̊��s��
	static cv::Mat_<double> J_G=(cv::Mat_<double>(9,8) <<
						A1(0),A2(0),A3(0),A4(0),A5(0),A6(0),A7(0),A8(0),
						A1(1),A2(1),A3(1),A4(1),A5(1),A6(1),A7(1),A8(1),
						A1(2),A2(2),A3(2),A4(2),A5(2),A6(2),A7(2),A8(2),
						A1(3),A2(3),A3(3),A4(3),A5(3),A6(3),A7(3),A8(3),
						A1(4),A2(4),A3(4),A4(4),A5(4),A6(4),A7(4),A8(4),
						A1(5),A2(5),A3(5),A4(5),A5(5),A6(5),A7(5),A8(5),
						A1(6),A2(6),A3(6),A4(6),A5(6),A6(6),A7(6),A8(6),
						A1(7),A2(7),A3(7),A4(7),A5(7),A6(7),A7(7),A8(7),
						A1(8),A2(8),A3(8),A4(8),A5(8),A6(8),A7(8),A8(8));//���R�r�A��
	enum G_TYPE{HOMOGRAPHY,};//�ϊ��s��̃^�C�v
	enum DERIVATION_TYPE{SOBEL,};//�摜�̌��z�̌v�Z���@

	/*=== �֐� ===*/
	//�ϊ�_G�ɂ��__p�̕ϊ�
	cv::Point warpPoint(cv::Point _p, cv::Mat _G, G_TYPE _TYPE=HOMOGRAPHY);
	//�摜_img�̓_warpPoint(_p,_G)�ɂ�������z���v�Z
	cv::Point derivation(cv::Mat _img, cv::Point _p, cv::Mat _G=cv::Mat::eye(3,3,CV_64F), DERIVATION_TYPE _TYPE=SOBEL);
	//�W�����̃p�����[�^�x�N�g��_x����C�z���O���t�B�ϊ��s����v�Z
	cv::Mat getHomography(cv::Mat _x);

	/*=== class: TemplateMatching_ESM ===*/
	class TemplateMatching_ESM{
	private:
		cv::Mat_<uchar> tmpImage;//�e���v���[�g�摜
		cv::Size2i tmpSize;//�e���v���[�g�̃T�C�Y
		int samplingInterval;//�e���v���[�g�̃T���v�����O�Ԋu
		int tmpPixels;//�e���v���[�g�̉�f��
		cv::Mat_<double> G;//�ߎ��ϊ��s��
		cv::Mat_<double> deltaX;//�X�V����ϊ��s��̃p�����[�^
		std::vector<cv::Mat_<double>> J_tmpI;//�e���v���[�g�摜�̃��R�r�A��
		std::vector<cv::Mat_<double>> J_wG;//���̑��C�K�v�ȃ��R�r�A��
		double v1,v2,vv;
		double cosSim;//�R�T�C���ގ��x
		cv::Mat resultImage;//�؂�o�����摜

		//�R�s�[�֎~
		TemplateMatching_ESM(const TemplateMatching_ESM& obj);

	public:
		//�R���X�g���N�^ �e���v���[�g_tmpImage�ƃz���O���t�B�̏����l_G���w��
		TemplateMatching_ESM(cv::Mat _tmpImage=cv::Mat(), cv::Mat _G=cv::Mat(), int _samplingInterval=0);
		//�f�X�g���N�^
		~TemplateMatching_ESM();

		/*--- �C���^�t�F�[�X ---*/
		//������ �e���v���[�g_tmpImage�ƃz���O���t�B�̏����l_G���w��
		bool init(cv::Mat _tmpImage, cv::Mat _G=cv::Mat());
		//�Q�Ɖ摜_refImage����C�e���v���[�g��؂�o���Ďw�肷��D
		bool makeTmpImage(cv::Mat _refImage);
		//�摜_curImage�ƃe���v���[�g�}�b�`���O�̌��ʓ�����C�Ή�����摜�̈��Ԃ�
		cv::Mat matching(cv::Mat _curImage);

		/*--- �A�N�Z�b�T ---*/
		cv::Mat getTmpImage();
		cv::Size getTmpSize();
		int getSamplingInterval();
		int getTmpPixels();
		cv::Mat getG();
		double getCosSim();
		cv::Mat getResultImage();
		bool setSamplingInterval(int _interval);
		void setG(cv::Mat _G);

		/*--- ���̑��֐� ---*/
	};
};

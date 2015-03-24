#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/opencv.hpp>

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


namespace CAPTURE{
	class Capture{
	public:
		//���z�f�X�g���N�^
		virtual ~Capture(){}

		/* �C���^�t�F�[�X */
		//I:�L���v�`�����J�n����
		virtual bool startCapture(unsigned int _sourceNo)=0;
		//I:�L���v�`��������������
		virtual bool initCapture()=0;
		//I:�o�b�t�@�ɃL���v�`�������f�[�^���i�[
		virtual cv::Mat capture(unsigned char* _buf)=0;
		//I:�L���v�`�������f�[�^��Mat�Ƃ��ĕԂ�
		virtual cv::Mat capture()=0;
		//I:�擾�ł���摜����\������
		virtual bool showInfo()=0;
		//I:�L���v�`�����I������
		virtual bool endCapture()=0;

		/*�A�N�Z�b�T*/
		//A:�J�����̏���Ԃ�
		virtual void* getCaptureInfo(unsigned int _CAP_INFO)=0;
		//A:�L���v�`��������ɃX�^�[�g���Ă��邩�ǂ���
		virtual bool isStarted()=0;
	};
};

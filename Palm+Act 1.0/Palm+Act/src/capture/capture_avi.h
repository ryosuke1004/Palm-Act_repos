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
		}info;//�r�f�I���
		OHNO_UTILITIES::Timer timer;//�^�C�}�[
		bool flgStarted;//�J�����Ɛڑ�����Ă��邩�ǂ���

	public:
		//�R���X�g���N�^
		Capture_avi();
		//�R�s�[�֎~
		Capture_avi(const Capture_avi& obj);
		//�f�X�g���N�^
		~Capture_avi();

		//I:�L���v�`�����J�n����
		bool startCapture(unsigned int _sourceNo);
		//I:�L���v�`��������������
		bool initCapture();
		//I:�o�b�t�@�ɃL���v�`�������f�[�^���i�[
		cv::Mat capture(unsigned char* _buf);
		//I:�L���v�`�������f�[�^��Mat�Ƃ��ĕԂ�
		cv::Mat capture();
		//I:�擾�ł���摜����\������
		bool showInfo();
		//I:�L���v�`�����I������
		bool endCapture();

		/*�A�N�Z�b�T*/
		//A:�J�����̏���Ԃ�
		void* getCaptureInfo(unsigned int _CAP_INFO);
		//A:�L���v�`��������ɃX�^�[�g���Ă��邩�ǂ���
		bool isStarted();
	};
}
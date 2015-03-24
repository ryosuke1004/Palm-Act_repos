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
		unsigned int sourceNo;//�J����No
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
		}info;//�J�������
		OHNO_UTILITIES::Timer timer;//�^�C�}�[
		bool flgStarted;//�J�����Ɛڑ�����Ă��邩�ǂ���

	public:
		//�R���X�g���N�^
		Capture_cam();
		//�R�s�[�֎~
		Capture_cam(const Capture_cam& obj);
		//�f�X�g���N�^
		~Capture_cam();

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

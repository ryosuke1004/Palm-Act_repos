#pragma once

#include <cstdio>
#include <fstream>

#include <opencv2/opencv.hpp>
// �o�[�W�����擾
#define CV_VERSION_STR CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)
// �r���h���[�h
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
		/*--- ���W���[���̐ݒ� ---*/
		std::string moduleName;//���W���[���̖��O
		REGION_EXTRACTION_METHOD method;//��l�������̕��@
		unsigned int thrLow;//�P��臒l�ɂ���l���̍ۂ́C����臒l
		unsigned int thrHigh;//�P��臒l�ɂ���l���̍ۂ́C���臒l
		Morphology morphology;//�����t�H���W�[�����̎�ށC��

		/*--- �ϐ� ---*/
		cv::Mat_<uchar> resultImage;//���ʉ摜
		unsigned int pixelsTtl;//���o�����̈�̑���f��
		cv::Point2i center;//���o�����̈�̏d�S�ʒu

		/*--- bool�ϐ� ---*/


		//�R�s�[�֎~
		RegionExtraction_color(const RegionExtraction_color& _regionExtraction_color);
		//�R�s�[�֎~
		RegionExtraction_color operator=(const RegionExtraction_color&);


	public:
		//�R���X�g���N�^
		RegionExtraction_color(
			REGION_EXTRACTION_METHOD _REGION_EXTRACTION_METHOD=THRESHOLD,
			int _thrLow = 0,
			int _thrHigh = 255,
			MORPHOLOGY _type=(MORPHOLOGY)0,
			unsigned int _num=0);
		//�f�X�g���N�^
		~RegionExtraction_color();

		/*--- �C���^�t�F�[�X ---*/
		//I:_moduleName�Ƃ������O�̃��W���[����ǂݍ���
		bool loadModule(std::string _moduleName);
		//I:_moduleName�Ƃ������O�Ń��W���[����ۑ�����
		bool saveModule(std::string _moduleName);
		//I:�̈撊�o���s��()
		cv::Mat getResult(cv::Mat_<uchar> _inputImage);
		//I:��������
		void showInfo();

		/*--- �A�N�Z�b�T ---*/
		//A:��l�������̕��@���Z�b�g
		void setMethod(REGION_EXTRACTION_METHOD _method);
		//A:����臒l���Z�b�g
		void setThrLow(unsigned int _thrLow);
		//A:���臒l���Z�b�g
		void setThrHigh(unsigned int _thrHigh);
		//A:�����t�H���W�[�������Z�b�g
		void setMorphology(MORPHOLOGY _type, unsigned int _num);
		//A:����臒l���擾
		unsigned int getThrLow();
		//A:���臒l���擾
		unsigned int getThrHigh();
		//A:���W���[�������擾
		std::string getModuleName();
		//A:�����t�H���W�[�������擾
		Morphology getMorphology();
		//A:���ʉ摜���擾
		cv::Mat_<uchar> getResultImage();
		//A:���o�����̈�̑���f�����擾
		unsigned int getPixelsTtl();
		//A:���o�����̈�̏d�S�ʒu���擾
		cv::Point2i getCenter();
	};

	/*=== class : RegionExtraction_multiColor ===*/
	class RegionExtraction_multiColor{
	private:
		/*--- ���W���[���̏�� ---*/
		std::string moduleName;//���W���[���̖��O
		Morphology morphology;//�����t�H���W�[�����̎�ށC��

		/*--- �ϐ� ---*/
		cv::Mat_<uchar> resultImage;//���ʉ摜
		unsigned int pixelsTtl;//���o�����̈�̑���f��
		cv::Point2i center;//���o�����̈�̏d�S�ʒu

		//�R�s�[�֎~
		RegionExtraction_multiColor(const RegionExtraction_multiColor& _regionExtraction_multiColor);
		//�R�s�[�֎~
		RegionExtraction_multiColor operator=(const RegionExtraction_multiColor&);

	public:
		REGION_EXTRACTION::RegionExtraction_color RE[3];//�e�`�����l���̗̈撊�o���W���[��

		//�R���X�g���N�^
		RegionExtraction_multiColor(std::string _moduleName=std::string());
		//�f�X�g���N�^
		~RegionExtraction_multiColor();

		/*--- �C���^�t�F�[�X ---*/
		//_moduleName�Ƃ������O�̃��W���[����ǂݍ���
		bool loadModule(std::string _moduleName);
		//_moduleName�Ƃ������O�Ń��W���[����ۑ�����
		bool saveModule(std::string _moduleName);
		//�̈撊�o���s��()
		cv::Mat getResult(cv::Mat_<cv::Vec3b> _inputImage);
		//��������
		void showInfo();

		/*--- �A�N�Z�b�T ---*/
		//�����t�H���W�[�������Z�b�g
		void setMorphology(MORPHOLOGY _type, unsigned int _num);
		//���W���[�������擾
		std::string getModuleName();
		//�����t�H���W�[�������擾
		Morphology getMorphology();
		//���o�����̈�̑���f�����擾
		unsigned int getPixelsTtl();
		//���o�����̈�̏d�S�ʒu���擾
		cv::Point2i getCenter();
		//�e�̈撊�o��ւ̃|�C���^���擾
		RegionExtraction_color* getRePtr(unsigned int _no);
		//���ʉ摜���擾
		cv::Mat getResultImage();
	};
};

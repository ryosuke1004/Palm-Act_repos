#pragma once

//=== opencv�̃C���N���[�h ===========================================================================
//�Eopencv��include�f�B���N�g�����C���N���[�h�f�B���N�g���Ɏw��D
//�Eopencv��lib�f�B���N�g�������C�u�����f�B���N�g���Ɏw��D
//====================================================================================================
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
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
#pragma comment(lib, "opencv_calib3d"  CV_VERSION_STR CV_EXT_STR)
//====================================================================================================

//=== .h�t�@�C���̃C���N���[�h ===================================================================
#include "../utilities/ohnoUtilities.h"
#include "../opticalFlow/opticalFlow_farneback.h"
#include "../regionExtraction/regionExtraction_color.h"
#include "../regionExtraction/regionExtraction_setting.h"
#include "../interfaces/keyboardFunc.h"
#include "../utilities/cvGraphics.h"
#include "../capture/capture_cam.h"
#include "../fingerTracking\fingerTracking.h"
//====================================================================================================

using namespace REGION_EXTRACTION;
using namespace OPTICAL_FLOW_FARNEBACK;
using namespace OHNO_UTILITIES;
using namespace CAPTURE_CAM;

namespace PALM_ACT{
	/*=== class : ForceSensor ===*/
	class ForceSensor{
	private:
		///*--- ���O ---*/
		std::string sensorName;//ForceSensor�̎��ʖ�

		/*--- ���W���[�� ---*/
		Capture* cap;//�摜�擾
		FingerTracking* FT;//�w��ǐ�
		RegionExtraction_multiColor* RE_skin;//���F�̈撊�o
		OpticalFlow* OF;//�I�v�e�B�J���t���[�v�Z
		OHNO_UTILITIES::SplitTimer ST;//�������Ԍv�Z

		/*--- �p�����[�^ ---*/
		unsigned int resizeRatio;//�摜�k����(%)
		unsigned int vectorsOnCircleNum;//���S�~��̓_�̐�
		unsigned int vectorsOnRadiusNum;//���a�����̓_�̐�
		unsigned int referencePointsNum;//�_�̐�
		unsigned int proportionOfCircle;//�摜���ɂ�����~�̊���(%)
		unsigned int proportionOfLocalDisp;//�摜���ɂ����鏶�̕ό`�ɂ��ψʂ��y�Ԕ͈͂̊���(%)
		double flowGain;//�I�v�e�B�J���t���[�̃Q�C��

		/*--- �ϐ� ---*/
		/* �摜 */
		cv::Mat_<cv::Vec3b> initFrame;//�����t���[��
		cv::Mat_<cv::Vec3b> prevFrame;//��O�̃t���[��
		cv::Mat originalFrame;//���݂̃t���[���i�I���W�i���T�C�Y�j
		cv::Mat_<cv::Vec3b> curFrame;//���݂̃t���[��
		/* �}�X�N */
		cv::Mat_<uchar> maskOfFinger;//�w�̈�̃}�X�N
		cv::Mat_<uchar> maskOfSkin;//���̈�̃}�X�N
		/* �w */
		struct Finger{
			cv::Point2i actionPoint;//�͂̍�p�_
			double angle;//�摜���ʂ�x������̎w�̕Ίp
			unsigned int thickness;//�w�̑���
			cv::Mat G;//�w��̕ϊ��s��
			bool isTracked;//�w���ǐՂ���Ă���
		}finger;//�w��
		/* �Q�Ɠ_ */
		cv::Point2f* referencePoints;//���ΎQ�Ɠ_
		bool* isOnSkin;//�Q�Ɠ_�����̈��ɂ��邩�ǂ���
		/* ���̑��̕ϐ� */
		cv::Mat_<double> H;//�����t���[�g�ƌ��݂̃t���[���Ԃł̏����ʂ̎ˉe�ϊ��s��(3,3,CV_64FC1)
		cv::Mat_<double> dH;//��O�̃t���[�g�ƌ��݂̃t���[���Ԃł̏����ʂ̎ˉe�ϊ��s��(3,3,CV_64FC1)
		unsigned int onSkinVectorsNum;//���̈��̎Q�Ɠ_�̐�
		cv::Vec3f dForce;//�t���[���Ԃ̗�
		/* �t���O */
		bool isStarted;//�Z���T���������N������Ă��邩�ǂ����̃t���O
		bool renderingFlg;//�������ʕ`�惂�[�h���ǂ����t���O
		bool timerFlg;//�������Ԃ��v�����邩�ǂ����t���O
		bool lockFlg;//���񏈗��p
		bool initFlg;//���������߂̃t���O
		bool compensationFlg;//���̓�����␳���邩�ǂ����i�����Œ肳��Ă���ꍇ��false�j
		/* �o�� */
		cv::Vec3f force;//�̓x�N�g��
		cv::Mat resultImage;//�������ʉ摜
		cv::Mat flowImage;//���݂̃t���[���ɂ�����I�v�e�B�J���t���[�̗l�q��`��
		cv::Mat flowImageInOriginalPlane;//�����t���[���ł̏��̈ʒu�p���ɂ�����I�v�e�B�J���t���[�̗l�q��`��

		/*--- �֐� ---*/
		//���W���[���̍폜
		void deleteModules();
		//�L���v�`��
		bool capturing();
		//���F�̈撊�o
		bool skinExtraction();
		//�͂̍�p�_�̌v�Z
		cv::Point2i calcActionPoint(cv::Mat _G);
		//�w�̑����̌v�Z
		double calcFingerThickness(cv::Mat _G);
		//�w�̌����̌v�Z
		double calcFingerAngle(cv::Mat _G);
		//�w�̏�Ԍv�Z
		bool fingerDetection();
		//�I�v�e�B�J���t���[�v�Z
		bool opticalFlowCalculation();
		//�z���O���t�B�ϊ��v�Z
		bool homographyCalculation();
		//�͐���
		bool forceEstimation();
		//�������ʂ̕`�悷��
		bool render();
		//�������Ԃ�\������
		void showTimer();
		//�L�[�{�[�h�C���^�t�F�[�X
		bool keyboard();

		//�R�s�[�֎~
		ForceSensor(const ForceSensor& _forceSensor);
		//�R�s�[�֎~
		ForceSensor operator=(const ForceSensor&);

	public:
		//�R���X�g���N�^
		ForceSensor(char* _sensorName=NULL);
		//�f�X�g���N�^
		~ForceSensor();

		/*--- �C���^�t�F�[�X ---*/
		//�Z���T�̐ݒ��S�ď���������
		bool initSensor();
		//_sensorName�Ƃ������O�̃Z���T��ǂݍ���
		bool loadSensor(std::string _sensorName);
		//_sensorName�Ƃ������O�ŃZ���T��ۑ�����
		bool saveSensor(std::string _sensorName);
		//_sensorName�Ƃ����Z���T�̃p�����[�^��ݒ肷��
		bool prmSetting();
		//�w�g���b�L���O�̏�����
		bool initFT();
		//�w�g���b�L���O
		bool fingerTracking();
		//�Z���V���O����
		bool run();
		//�f�o�C�X�̏���񎦂���
		void showInfo();
		//�I������
		void end();

		/*--- �A�N�Z�b�T ---*/
		/* set:�p�����[�^�i�K���Ō��init�����s�����j */
		void setResizeRatio(unsigned int _value);
		void setVectorsOnCircleNum(unsigned int _value);
		void setVectorsOnRadiusNum(unsigned int _value);
		void setProportionOfCircle(unsigned int _value);
		void setProportionOfLocalDisp(unsigned int _value);
		void setFlowGain(double _flowGain);
		/* set:�ϐ��i�}���`�X���b�h�����鎞�ɗ��p����j*/
		void setFinger(ForceSensor::Finger _finger);
		void setDH(cv::Mat _dH);
		void setH(cv::Mat _H);
		void setRendering(bool _flg);
		void setCompensation(bool _flg);
		/* get */
		Capture* getCap();
		OpticalFlow* getOF();
		RegionExtraction_multiColor* getRE_skin();
		unsigned int getResizeRatio();
		unsigned int getVectorsOnCircleNum();
		unsigned int getVectorsOnRadiusNum();
		unsigned int getReferencePointsNum();
		unsigned int getProportionOfCircle();
		unsigned int getProportionOfLocalDisp();
		double getFlowGain();
		cv::Vec3f getForce();
		cv::Mat getCurFrame();
		cv::Mat getResultImage();
		cv::Mat getFlowImage();
		cv::Mat getFlowImageInOriginalPlane();
		bool isLocked();
	};

	/*=== �֐� ===*/
	//_source����_target�֌������x�N�g����ɉ摜�T�C�Y_maskSize��_thickness�̑����̎w�}�X�N���쐬
	cv::Mat_<uchar> makeFingerMask(cv::Point2i _source, double _angle, cv::Size2i _maskSize, unsigned int _thickness);
	//�ˉe�ϊ��s��_H�ɂ��__p�̎ˉe�ϊ�
	cv::Point2f getProjectedPoint(cv::Point2f _p, cv::Mat_<double> _H);
	//�ˉe�ϊ��s��_H�ɂ��摜_image�̎ˉe�ϊ�
	cv::Mat getProjectedImage(cv::Mat _image, cv::Mat_<double> _H);
};

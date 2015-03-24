#pragma once

#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "../utilities/cvGraphics.h"

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


namespace OPTICAL_FLOW{
	/* === class : OpticalFlow === */
	class OpticalFlow{
	public:
		//���z�f�X�g���N�^
		virtual ~OpticalFlow(){}

		/*--- �C���^�t�F�[�X ---*/
		//I:_name�̃��W���[����ǂݍ���
		virtual bool loadModule(std::string _name)=0;
		//I:_name�Ƃ������O�Ń��W���[����ۑ�
		virtual bool saveModule(std::string _name)=0;
		//I:�p�����[�^��\��
		virtual void showInfo()=0;
		//I:_prevFrame����_curFrame�ւ̃t���[����v�Z���A_flowField�Ɋi�[
		virtual bool calcFlowField(cv::Mat _prevFrame, cv::Mat _curFrame, cv::Mat_<cv::Point2f>& _flowField)=0;
		//I:_prevFrame����_curFrame�ւ̃t���[����v�Z���AflowField�Ɋi�[
		virtual bool calcFlowField(cv::Mat _prevFrame, cv::Mat _curFrame)=0;
		//I:_refPointsNum�̎Q�Ɠ_�̔z��_refPoints�ɂ�����t���[�x�N�g�����o�b�t�@�Ɋi�[
		virtual bool calcFlowVectors(cv::Point2i* _refPoints, unsigned int _refPointsNum)=0;
		////I:�t���[�x�N�g���̉���
		//virtual bool renderFlowVectors(cv::Mat& _inputImage, float _maxNorm, unsigned int _thickness, cv::Mat_<uchar> _mask = cv::Mat())=0;

		//A:���W���[���̖��O��Ԃ�
		virtual std::string getModuleName()=0;
		//A:_prmNo�p�����[�^�̃|�C���^��Ԃ�
		virtual void* getPrmPtr(unsigned int _OPTICAL_FLOW_PRM)=0;
		//A:flowField���擾
		virtual cv::Mat getFlowField()=0;
		//A:�__refPoint�ɂ�����t���[�x�N�g�����擾
		virtual cv::Point2f getFlowVectorAt(cv::Point2i _refPoint)=0;
		//A:�t���[�x�N�g���̐����擾
		virtual unsigned int getFlowVectorsNum()=0;
		//A:�t���[�x�N�g���̐擪�|�C���^���擾
		virtual cv::Point2f* getFlowVectorsPtr()=0;
		//A:�Q�Ɠ_�̐擪�|�C���^���擾
		virtual cv::Point2i* getRefPointsPtr()=0;
	};
};

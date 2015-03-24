#pragma once
#include "opticalFlow.h"

using namespace OPTICAL_FLOW;
namespace OPTICAL_FLOW_FARNEBACK{

	enum OPTICAL_FLOW_PRM{
		PYR_SCALE,
		LEVELS,
		WINDOW_SIZE,
		ITERATIONS,
		POLY_N,
		POLY_SIGMA,
		FLAGS
	};

	/* === class : OpticalFlow_farneback === */
	class OpticalFlow_farneback : public OpticalFlow{
	private:
		/*--- ���W���[���̏�� ---*/
		std::string name;//���W���[���̖��O
		struct FlowPrm{
			//�摜�s���~�b�h�̃X�P�[���A0.5�͌ÓT�I�摜�s���~�b�h
			double pyrScale;
			//�摜�s���~�b�h�̑w�̐�
			int levels;
			//���ω����T�C�Y
			int windowSize;
			//�摜�s���~�b�h�̊e���x���ɂ�����A�A���S���Y���̔�����
			int iterations;
			//�s�N�Z���ߖT�̈�̃T�C�Y�A�傫���قǊ��炩�ȃT�[�t�F�C�X�ŋߎ������
			int polyN;
			//�K�E�X���z�̕W���΍��BPolyN=5�Ȃ�1.1���K��
			double polySigma;
			//
			int flags;
		}flowPrm;//�p�����[�^

		/*--- �ϐ� ---*/
		cv::Mat_<cv::Point2f> flowField;//�t���[��
		unsigned int flowVectorsNum;//�t���[�x�N�g�����Ƃ�_�̐�
		cv::Point2i* refPoints;//�Q�Ɠ_
		cv::Point2f* flowVectors;//�t���[�x�N�g��

		/*--- bool�ϐ� ---*/
		bool isCalcFlowField;//�t���[����v�Z�������ǂ���
		bool isCalcFlowVectors;//�t���[�x�N�g�����Z�b�g�������ǂ���

		//�R�s�[�֎~
		OpticalFlow_farneback(const OpticalFlow_farneback& _opticalFlow);
		//�R�s�[�֎~
		OpticalFlow_farneback operator=(const OpticalFlow_farneback&);


	public:
		//�R���X�g���N�^
		OpticalFlow_farneback();
		//�f�X�g���N�^
		~OpticalFlow_farneback();

		/*--- �C���^�t�F�[�X ---*/
		//I:_name�̃��W���[����ǂݍ���
		bool loadModule(std::string _name);
		//I:_name�Ƃ������O�Ń��W���[����ۑ�����
		bool saveModule(std::string _name);
		//I:�p�����[�^��\��
		void showInfo();
		//I:_prevFrame����_curFrame�ւ̃t���[����v�Z���A_flowField�Ɋi�[
		bool calcFlowField(cv::Mat _prevFrame, cv::Mat _curFrame, cv::Mat_<cv::Point2f>& _flowField);
		//I:_prevFrame����_curFrame�ւ̃t���[����v�Z���AflowField�Ɋi�[
		bool calcFlowField(cv::Mat _prevFrame, cv::Mat _curFrame);
		//I:_refPointsNum�̎Q�Ɠ_�̔z��_refPoints�ɂ�����t���[�x�N�g�����o�b�t�@�Ɋi�[
		bool calcFlowVectors(cv::Point2i* _refPoints, unsigned int _refPointsNum);
		//I:�t���[�x�N�g���̉���
		bool renderFlowVectors(cv::Mat& _inputImage, float _maxNorm, unsigned int _thickness, cv::Mat_<uchar> _mask = cv::Mat());

		/*--- �A�N�Z�b�T ---*/
		//A:���W���[���̖��O��Ԃ�
		std::string getModuleName();
		//A:_prmNo�p�����[�^�̃|�C���^��Ԃ�
		void* getPrmPtr(unsigned int _OPTICAL_FLOW_PRM);
		//A:flowField���擾
		cv::Mat getFlowField();
		//A:�__refPoint�ɂ�����t���[�x�N�g�����擾
		cv::Point2f getFlowVectorAt(cv::Point2i _refPoint);
		//A:�t���[�x�N�g���̐����擾
		unsigned int getFlowVectorsNum();
		//A:�t���[�x�N�g���̐擪�|�C���^���擾
		cv::Point2f* getFlowVectorsPtr();
		//A:�Q�Ɠ_�̐擪�|�C���^���擾
		cv::Point2i* getRefPointsPtr();

	};
}

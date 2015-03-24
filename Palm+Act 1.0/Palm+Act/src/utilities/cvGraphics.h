#pragma once
#include <cstdio>
#include <windows.h>
#undef min
#undef max
#include <opencv2/opencv.hpp>
#include "ohnoUtilities.h"
// �o�[�W�����擾
#define CV_VERSION_STR CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)
// �r���h���[�h
#ifdef _DEBUG
#define CV_EXT_STR "d.lib"
#else
#define CV_EXT_STR ".lib"
#endif
#pragma comment(lib, "opencv_imgproc"  CV_VERSION_STR CV_EXT_STR)

namespace CV_GRAPHICS{
	//�^���J���[�̍쐬
	static cv::Scalar calPseudoColor(double phase, double shift = 0.0)
	{
		phase = std::min(phase, 1.0);
		phase = std::max(phase, 0.0); //0����1��
		shift += PI+PI/4;     //����Ԃ�
		return cv::Scalar(
			uchar( 255*(sin(1.5*PI*phase + shift + PI ) + 1)/2.0 ),
			uchar( 255*(sin(1.5*PI*phase + shift + PI/2 ) + 1)/2.0 ), 
			uchar( 255*(sin(1.5*PI*phase + shift  ) + 1)/2.0 )
			);
	}
	//_center�𒆐S�Ƃ��āC+��`��
	static void cross(cv::Mat _image, cv::Point2i _center, cv::Scalar _color, unsigned int _thickness = 1)
	{
		cv::line(
			_image,
			cv::Point2i(_center.x - 3*_thickness, _center.y),
			cv::Point2i(_center.x + 3*_thickness, _center.y),
			_color,
			_thickness);
		cv::line(
			_image,
			cv::Point2i(_center.x, _center.y - 3*_thickness),
			cv::Point2i(_center.x, _center.y + 3*_thickness),
			_color,
			_thickness);
	}
	//_source����_target�Ɍ������x�N�g����`��
	static void arrow(cv::Mat _image, cv::Point2i _source, cv::Point2i _vector, cv::Scalar _color, unsigned int _thickness = 1)
	{
		cv::line(
			_image,
			_source,
			_source+_vector,
			_color,
			_thickness);

		if(cv::norm(_vector)>0.05){
			cv::line(
				_image,
				_source+_vector,
				cv::Point2i(
					_source.x+_vector.x+(_vector.x*cos(150.0/360*2*PI) - _vector.y*sin(150.0/360*2*PI))/cv::norm(_vector)*8,
					_source.y+_vector.y+(_vector.x*sin(150.0/360*2*PI) + _vector.y*cos(150.0/360*2*PI))/cv::norm(_vector)*8),
				_color,
				_thickness);
			cv::line(
				_image,
				_source+_vector,
				cv::Point2i(
					_source.x+_vector.x+(_vector.x*cos(-150.0/360*2*PI) - _vector.y*sin(-150.0/360*2*PI))/cv::norm(_vector)*8,
					_source.y+_vector.y+(_vector.x*sin(-150.0/360*2*PI) + _vector.y*cos(-150.0/360*2*PI))/cv::norm(_vector)*8),
				_color,
				_thickness);
		}
	}
	//_point�̐F���C_colorValue�i�O�`�P�j�ɑΉ������^���J���[�ɂ���D
	static void pseudoColoring(cv::Mat _image, cv::Point2i _point, double _colorValue)
	{
		cv::line(
			_image,
			_point,
			_point,
			CV_GRAPHICS::calPseudoColor(_colorValue),
			1);
	}
};

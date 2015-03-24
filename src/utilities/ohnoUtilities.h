#pragma once

#include <cstdio>
#include <windows.h>

#define PI 3.141592653

namespace OHNO_UTILITIES{
	/*=== ���w�I���� ===*/
	class Math{
	public:
		static double DEG2RAD(double deg);
		static double RAD2DEG(double rad);
	};

	/*=== �^�C�}�[ ===*/
	class Timer{
	protected:
		LARGE_INTEGER freq;
		LARGE_INTEGER start;
		LARGE_INTEGER end;
	public:
		Timer(void);
		~Timer(void);
		void setStartTime();
		void setEndTime();
		double getDeltaTime();
	};

	/*=== �X�v���b�g�E�^�C�}�[ ===*/
	class SplitTimer : public Timer{
	private:
		double* splitTimes;
		double splitTimesTtl;
		unsigned int num;
	public:
		SplitTimer();
		~SplitTimer();
		void setStartTime();
		void setSplitTime();
		void setEndTime();
		double* getSplitTimes();
		double getSplitTimesTtl();
		void showSplitTimes();
	};

};

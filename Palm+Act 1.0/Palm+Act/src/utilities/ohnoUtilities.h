#pragma once

#include <cstdio>
#include <windows.h>

#define PI 3.141592653

namespace OHNO_UTILITIES{
	/*=== 数学的処理 ===*/
	class Math{
	public:
		static double DEG2RAD(double deg);
		static double RAD2DEG(double rad);
	};

	/*=== タイマー ===*/
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

	/*=== スプリット・タイマー ===*/
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

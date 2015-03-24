#include "ohnoUtilities.h"

namespace OHNO_UTILITIES{
double Math::DEG2RAD(double deg)
{
	return deg*PI/180.0;
}
double Math::RAD2DEG(double rad)
{
	return rad*180.0/PI;
}

Timer::Timer(void)
{
}
Timer::~Timer(void)
{
}

void Timer::setStartTime()
{
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
}
void Timer::setEndTime()
{
	QueryPerformanceCounter(&end);
}
double Timer::getDeltaTime()
{
	return (double)(end.QuadPart-start.QuadPart)/(double)freq.QuadPart;
}


SplitTimer::SplitTimer()
{
	this->num=0;
	this->splitTimesTtl=0;
}
SplitTimer::~SplitTimer()
{
	if(this->num == 1)	delete this->splitTimes;
	else if(this->num)	delete [] this->splitTimes;
}

void SplitTimer::setStartTime()
{
	if(this->num == 1)	delete this->splitTimes;
	else if(this->num)	delete [] this->splitTimes;

	this->num=0;
	this->splitTimesTtl=0;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
}
void SplitTimer::setSplitTime()
{
	QueryPerformanceCounter(&end);
	double* tmp = new double[this->num+1];
	for(int i=0; i<this->num; i++){
		tmp[i] = this->splitTimes[i];
	}

	if(this->num == 1)	delete this->splitTimes;
	else if(this->num)	delete [] this->splitTimes;

	tmp[this->num++] = (double)(end.QuadPart-start.QuadPart)/(double)freq.QuadPart;
	this->splitTimesTtl += (double)(end.QuadPart-start.QuadPart)/(double)freq.QuadPart;
	this->splitTimes = tmp;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
}
void SplitTimer::setEndTime()
{
	QueryPerformanceCounter(&end);
	double* tmp = new double[this->num+1];
	for(int i=0; i<this->num; i++){
		tmp[i] = this->splitTimes[i];
	}

	if(this->num == 1)	delete this->splitTimes;
	else if(this->num)	delete [] this->splitTimes;

	tmp[this->num++] = (double)(end.QuadPart-start.QuadPart)/(double)freq.QuadPart;
	this->splitTimesTtl += (double)(end.QuadPart-start.QuadPart)/(double)freq.QuadPart;
	this->splitTimes = tmp;
}
double* SplitTimer::getSplitTimes()
{
	return this->splitTimes;
}
double SplitTimer::getSplitTimesTtl()
{
	return this->splitTimesTtl;
}
void SplitTimer::showSplitTimes()
{
	puts("===== split times =================");
	for(int i=0; i<this->num; i++){
		if(i == 0){
			printf("=   1st period : %f (sec)\n", this->splitTimes[i]);
		}
		else if(i == 1){
			printf("=   2nd period : %f (sec)\n", this->splitTimes[i]);
		}
		else if(i == 2){
			printf("=   3rd period : %f (sec)\n", this->splitTimes[i]);
		}
		else{
			printf("= %3dth period : %f (sec)\n", i+1, this->splitTimes[i]);
		}
	}
	puts("= ---------------------------------");
	printf("=   total time : %f (sec)\n", this->splitTimesTtl);
	puts("===================================\n");
}

}
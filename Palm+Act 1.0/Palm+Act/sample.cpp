/*==================================================*/
//�w�b�_�t�@�C��
/*==================================================*/
#include "src/PalmAct/Palm+Act.h"
#include <windows.h>
#include <process.h>

/*==================================================*/
//�O���[�o���ϐ�
/*==================================================*/
PALM_ACT::ForceSensor* FS;//�̓Z���T

/*==================================================*/
//�֐��錾
/*==================================================*/
//�̓Z���V���O�p�̃X���b�h
void thread1(LPVOID pParam);
//�w��g���b�L���O�p�̃X���b�h
void thread2(LPVOID pParam);

/*==================================================*/
//���C��
/*==================================================*/
int main(int argc, char *argv[])
{
	/*=== �Z���T��` ===*/
	char fileName[128] = "test";
	FS = new PALM_ACT::ForceSensor(fileName);
	FS->showInfo();

	/*=== �J�����m�F ===*/
	while(1){
		FS->run();
		printf("push any key to move on next step");
		if(cv::waitKey(30)>=0)	break;
	}
	
	/*=== �p�����[�^�ݒ� ===*/
	if(0){
		FS->prmSetting();
		FS->saveSensor(fileName);
	}
	cv::destroyAllWindows();

	/*=== �Z���T������ ===*/
	FS->initFT();
	
	/*=== �`��ݒ� ===*/
	if(1)	FS->setRendering(true);
	else	FS->setRendering(false);

	cv::destroyAllWindows();
	std::cout<<"3"<<std::endl;	Sleep(1000);
	std::cout<<"2"<<std::endl;	Sleep(1000);
	std::cout<<"1"<<std::endl;	Sleep(1000);
	FS->initSensor();

	HANDLE hMutex;
	HANDLE hThread[2];
	hMutex = CreateMutex(NULL,FALSE,NULL);
	hThread[0] = (HANDLE)_beginthread(thread1,0,NULL);
	hThread[1] = (HANDLE)_beginthread(thread2,0,NULL);
	WaitForMultipleObjects(2,hThread,TRUE,INFINITE);
	CloseHandle(hMutex);

	return 0;
}

/*==================================================*/
//�֐�����
/*==================================================*/
//�̓Z���V���O�p�̃X���b�h
void thread1(LPVOID pParam)
{
	while(1){
		if(FS->run()){
			cv::Vec3f force=FS->getForce();
			std::cout<<force<<std::endl;
		}
	}
}
//�w��g���b�L���O�p�̃X���b�h
void thread2(LPVOID pParam)
{
	OHNO_UTILITIES::Timer timer;
	timer.setStartTime();
	while(1){
		if(FS->isLocked()){
			Sleep(10);
			continue;
		}

		FS->fingerTracking();
		while(1){
			timer.setEndTime();
			if(timer.getDeltaTime()*3>=1)	break;
			Sleep(1);
		}
		timer.setStartTime();
	}
}

/*==================================================*/
//ヘッダファイル
/*==================================================*/
#include "src/PalmAct/Palm+Act.h"
#include <windows.h>
#include <process.h>

/*==================================================*/
//グローバル変数
/*==================================================*/
PALM_ACT::ForceSensor* FS;//力センサ

/*==================================================*/
//関数宣言
/*==================================================*/
//力センシング用のスレッド
void thread1(LPVOID pParam);
//指先トラッキング用のスレッド
void thread2(LPVOID pParam);

/*==================================================*/
//メイン
/*==================================================*/
int main(int argc, char *argv[])
{
	/*=== センサ定義 ===*/
	char fileName[128] = "test";
	FS = new PALM_ACT::ForceSensor(fileName);
	FS->showInfo();

	/*=== カメラ確認 ===*/
	while(1){
		FS->run();
		printf("push any key to move on next step");
		if(cv::waitKey(30)>=0)	break;
	}
	
	/*=== パラメータ設定 ===*/
	if(0){
		FS->prmSetting();
		FS->saveSensor(fileName);
	}
	cv::destroyAllWindows();

	/*=== センサ初期化 ===*/
	FS->initFT();
	
	/*=== 描画設定 ===*/
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
//関数実体
/*==================================================*/
//力センシング用のスレッド
void thread1(LPVOID pParam)
{
	while(1){
		if(FS->run()){
			cv::Vec3f force=FS->getForce();
			std::cout<<force<<std::endl;
		}
	}
}
//指先トラッキング用のスレッド
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

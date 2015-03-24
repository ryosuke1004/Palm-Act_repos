#pragma once
#include <Windows.h>
#include <WinUser.h>

class KeyboardFunc{
public:
	static bool discreteFlg;

	static bool getIsCharPushed(char c);
	static bool getIsLeftPushed();
	static bool getIsUpPushed();
	static bool getIsRightPushed();
	static bool getIsDownPushed();
	static bool getIsSpacePushed();
	static bool getIsShiftPushed();
	static bool getIsControlPushed();
	static bool getIsAltPushed();
	static bool getIsEscapePushed();
	static bool getIsBackspacePushed();
	static bool getIsReturnPushed();
	static bool getIsFkeyPushed(int _no);
};

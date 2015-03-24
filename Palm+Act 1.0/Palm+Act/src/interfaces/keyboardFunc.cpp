#pragma once
#include "keyboardFunc.h"

bool KeyboardFunc::discreteFlg=true;

bool KeyboardFunc::getIsCharPushed(char c){
	switch(c){
	case (int)('a'):
		if(GetAsyncKeyState(c & 0x41)){
			while(1){
				if(!GetAsyncKeyState(c & 0x41))	break;
			}
			return true;
		}
		else							return false;
	case (int)('b'):
		if(GetAsyncKeyState(c & 0x42)){
			while(1){
				if(!GetAsyncKeyState(c & 0x42))	break;
			}
			return true;
		}
		else							return false;
	case (int)('c'):
		if(GetAsyncKeyState(c & 0x43)){
			while(1){
				if(!GetAsyncKeyState(c & 0x43))	break;
			}
			return true;
		}
		else							return false;
	case (int)('d'):
		if(GetAsyncKeyState(c & 0x44)){
			while(1){
				if(!GetAsyncKeyState(c & 0x44))	break;
			}
			return true;
		}
		else							return false;
	case (int)('e'):
		if(GetAsyncKeyState(c & 0x45)){
			while(1){
				if(!GetAsyncKeyState(c & 0x45))	break;
			}
			return true;
		}
		else							return false;
	case (int)('f'):
		if(GetAsyncKeyState(c & 0x46)){
			while(1){
				if(!GetAsyncKeyState(c & 0x46))	break;
			}
			return true;
		}
		else							return false;
	case (int)('g'):
		if(GetAsyncKeyState(c & 0x47)){
			while(1){
				if(!GetAsyncKeyState(c & 0x47))	break;
			}
			return true;
		}
		else							return false;
	case (int)('h'):
		if(GetAsyncKeyState(c & 0x48)){
			while(1){
				if(!GetAsyncKeyState(c & 0x48))	break;
			}
			return true;
		}
		else							return false;
	case (int)('i'):
		if(GetAsyncKeyState(c & 0x49)){
			while(1){
				if(!GetAsyncKeyState(c & 0x49))	break;
			}
			return true;
		}
		else							return false;
	case (int)('j'):
		if(GetAsyncKeyState(c & 0x4A)){
			while(1){
				if(!GetAsyncKeyState(c & 0x4A))	break;
			}
			return true;
		}
		else							return false;
	case (int)('k'):
		if(GetAsyncKeyState(c & 0x4B)){
			while(1){
				if(!GetAsyncKeyState(c & 0x4B))	break;
			}
			return true;
		}
		else							return false;
	case (int)('l'):
		if(GetAsyncKeyState(c & 0x4C)){
			while(1){
				if(!GetAsyncKeyState(c & 0x4C))	break;
			}
			return true;
		}
		else							return false;
	case (int)('m'):
		if(GetAsyncKeyState(c & 0x4D)){
			while(1){
				if(!GetAsyncKeyState(c & 0x4D))	break;
			}
			return true;
		}
		else							return false;
	case (int)('n'):
		if(GetAsyncKeyState(c & 0x4E)){
			while(1){
				if(!GetAsyncKeyState(c & 0x4E))	break;
			}
			return true;
		}
		else							return false;
	case (int)('o'):
		if(GetAsyncKeyState(c & 0x4F)){
			while(1){
				if(!GetAsyncKeyState(c & 0x4F))	break;
			}
			return true;
		}
		else							return false;
	case (int)('p'):
		if(GetAsyncKeyState(c & 0x50)){
			while(1){
				if(!GetAsyncKeyState(c & 0x50))	break;
			}
			return true;
		}
		else							return false;
	case (int)('q'):
		if(GetAsyncKeyState(c & 0x51)){
			while(1){
				if(!GetAsyncKeyState(c & 0x51))	break;
			}
			return true;
		}
		else							return false;
	case (int)('r'):
		if(GetAsyncKeyState(c & 0x52)){
			while(1){
				if(!GetAsyncKeyState(c & 0x52))	break;
			}
			return true;
		}
		else							return false;
	case (int)('s'):
		if(GetAsyncKeyState(c & 0x53)){
			while(1){
				if(!GetAsyncKeyState(c & 0x53))	break;
			}
			return true;
		}
		else							return false;
	case (int)('t'):
		if(GetAsyncKeyState(c & 0x54)){
			while(1){
				if(!GetAsyncKeyState(c & 0x54))	break;
			}
			return true;
		}
		else							return false;
	case (int)('u'):
		if(GetAsyncKeyState(c & 0x55)){
			while(1){
				if(!GetAsyncKeyState(c & 0x55))	break;
			}
			return true;
		}
		else							return false;
	case (int)('v'):
		if(GetAsyncKeyState(c & 0x56)){
			while(1){
				if(!GetAsyncKeyState(c & 0x56))	break;
			}
			return true;
		}
		else							return false;
	case (int)('w'):
		if(GetAsyncKeyState(c & 0x57)){
			while(1){
				if(!GetAsyncKeyState(c & 0x57))	break;
			}
			return true;
		}
		else							return false;
	case (int)('x'):
		if(GetAsyncKeyState(c & 0x58)){
			while(1){
				if(!GetAsyncKeyState(c & 0x58))	break;
			}
			return true;
		}
		else							return false;
	case (int)('y'):
		if(GetAsyncKeyState(c & 0x59)){
			while(1){
				if(!GetAsyncKeyState(c & 0x59))	break;
			}
			return true;
		}
		else							return false;
	case (int)('z'):
		if(GetAsyncKeyState(c & 0x5A)){
			while(1){
				if(!GetAsyncKeyState(c & 0x5A))	break;
			}
			return true;
		}
		else							return false;
	case(int)('0'):
		if(GetAsyncKeyState(c & 0x30)){
			while(1){
				if(!GetAsyncKeyState(c & 0x30))	break;
			}
			return true;
		}
		else							return false;
	case(int)('1'):
		if(GetAsyncKeyState(c & 0x31)){
			while(1){
				if(!GetAsyncKeyState(c & 0x31))	break;
			}
			return true;
		}
		else							return false;
	case(int)('2'):
		if(GetAsyncKeyState(c & 0x32)){
			while(1){
				if(!GetAsyncKeyState(c & 0x32))	break;
			}
			return true;
		}
		else							return false;
	case(int)('3'):
		if(GetAsyncKeyState(c & 0x33)){
			while(1){
				if(!GetAsyncKeyState(c & 0x33))	break;
			}
			return true;
		}
		else							return false;
	case(int)('4'):
		if(GetAsyncKeyState(c & 0x34)){
			while(1){
				if(!GetAsyncKeyState(c & 0x34))	break;
			}
			return true;
		}
		else							return false;
	case(int)('5'):
		if(GetAsyncKeyState(c & 0x35)){
			while(1){
				if(!GetAsyncKeyState(c & 0x35))	break;
			}
			return true;
		}
		else							return false;
	case(int)('6'):
		if(GetAsyncKeyState(c & 0x36)){
			while(1){
				if(!GetAsyncKeyState(c & 0x36))	break;
			}
			return true;
		}
		else							return false;
	case(int)('7'):
		if(GetAsyncKeyState(c & 0x37)){
			while(1){
				if(!GetAsyncKeyState(c & 0x37))	break;
			}
			return true;
		}
		else							return false;
	case(int)('8'):
		if(GetAsyncKeyState(c & 0x38)){
			while(1){
				if(!GetAsyncKeyState(c & 0x38))	break;
			}
			return true;
		}
		else							return false;
	case(int)('9'):
		if(GetAsyncKeyState(c & 0x39)){
			while(1){
				if(!GetAsyncKeyState(c & 0x39))	break;
			}
			return true;
		}
		else							return false;

	case(int)(','):
		if(GetAsyncKeyState(VK_OEM_COMMA)){
			while(1){
				if(!GetAsyncKeyState(VK_OEM_COMMA))	break;
			}
			return true;
		}
		else							return false;

	case(int)('.'):
		if(GetAsyncKeyState(VK_OEM_PERIOD)){
			while(1){
				if(!GetAsyncKeyState(VK_OEM_PERIOD))	break;
			}
			return true;
		}
		else							return false;

	default:
		return false;
	}
}
bool KeyboardFunc::getIsLeftPushed(){
	if(GetAsyncKeyState(VK_LEFT & 0x25)){
		while(discreteFlg){
			if(!GetAsyncKeyState(VK_LEFT & 0x25))	break;
		}
		return true;
	}
	else									return false;
}
bool KeyboardFunc::getIsUpPushed(){
	if(GetAsyncKeyState(VK_UP & 0x26)){
		while(discreteFlg){
			if(!GetAsyncKeyState(VK_UP & 0x26))	break;
		}
		return true;
	}
	else									return false;
}
bool KeyboardFunc::getIsRightPushed(){
	if(GetAsyncKeyState(VK_RIGHT & 0x27)){
		while(discreteFlg){
			if(!GetAsyncKeyState(VK_RIGHT & 0x27))	break;
		}
		return true;
	}
	else									return false;
}
bool KeyboardFunc::getIsDownPushed(){
	if(GetAsyncKeyState(VK_DOWN & 0x28)){
		while(discreteFlg){
			if(!GetAsyncKeyState(VK_DOWN & 0x28))	break;
		}
		return true;
	}
	else									return false;
}
bool KeyboardFunc::getIsSpacePushed(){
	if(GetAsyncKeyState(VK_SPACE & 0x20)){
		while(1){
			if(!GetAsyncKeyState(VK_SPACE & 0x20))	break;
		}
		return true;
	}
	else									return false;
}
bool KeyboardFunc::getIsShiftPushed(){
	if(GetAsyncKeyState(VK_SHIFT & 0x10)){
		return true;
	}
	else									return false;
}
bool KeyboardFunc::getIsControlPushed(){
	if(GetAsyncKeyState(VK_CONTROL & 0x11)){
		return true;
	}
	else									return false;
}
bool KeyboardFunc::getIsAltPushed(){
	if(GetAsyncKeyState(VK_MENU & 0x12)){
		return true;
	}
	else									return false;
}
bool KeyboardFunc::getIsEscapePushed(){
	if(GetAsyncKeyState(VK_ESCAPE)){
		while(1){
			if(!GetAsyncKeyState(VK_ESCAPE))	break;
		}
		return true;
	}
	else	return false;
}
bool KeyboardFunc::getIsBackspacePushed(){
	if(GetAsyncKeyState(VK_BACK)){
		while(1){
			if(!GetAsyncKeyState(VK_BACK))	break;
		}
		return true;
	}
	else	return false;
}
bool KeyboardFunc::getIsReturnPushed(){
	if(GetAsyncKeyState(VK_RETURN)){
		while(1){
			if(!GetAsyncKeyState(VK_RETURN))	break;
		}
		return true;
	}
	else	return false;
}
bool KeyboardFunc::getIsFkeyPushed(int _no){
	switch(_no){
	case 1:
		if(GetAsyncKeyState(VK_F1 & 0x70)){
			while(1){
				if(!GetAsyncKeyState(VK_F1 & 0x70))	break;
			}
			return true;
		}
		else	return false;
	case 2:
		if(GetAsyncKeyState(VK_F2 & 0x71)){
			while(1){
				if(!GetAsyncKeyState(VK_F2 & 0x71))	break;
			}
			return true;
		}
		else	return false;
	case 3:
		if(GetAsyncKeyState(VK_F3 & 0x72)){
			while(1){
				if(!GetAsyncKeyState(VK_F3 & 0x72))	break;
			}
			return true;
		}
		else	return false;
	case 4:
		if(GetAsyncKeyState(VK_F4 & 0x73)){
			while(1){
				if(!GetAsyncKeyState(VK_F4 & 0x73))	break;
			}
			return true;
		}
		else	return false;
	case 5:
		if(GetAsyncKeyState(VK_F5 & 0x74)){
			while(1){
				if(!GetAsyncKeyState(VK_F5 & 0x74))	break;
			}
			return true;
		}
		else	return false;
	case 6:
		if(GetAsyncKeyState(VK_F6 & 0x75)){
			while(1){
				if(!GetAsyncKeyState(VK_F6 & 0x75))	break;
			}
			return true;
		}
		else	return false;
	case 7:
		if(GetAsyncKeyState(VK_F7 & 0x76)){
			while(1){
				if(!GetAsyncKeyState(VK_F7 & 0x76))	break;
			}
			return true;
		}
		else	return false;
	case 8:
		if(GetAsyncKeyState(VK_F8 & 0x77)){
			while(1){
				if(!GetAsyncKeyState(VK_F8 & 0x77))	break;
			}
			return true;
		}
		else	return false;
	case 9:
		if(GetAsyncKeyState(VK_F9 & 0x78)){
			while(1){
				if(!GetAsyncKeyState(VK_F9 & 0x78))	break;
			}
			return true;
		}
		else	return false;
	case 10:
		if(GetAsyncKeyState(VK_F10 & 0x79)){
			while(1){
				if(!GetAsyncKeyState(VK_F10 & 0x79))	break;
			}
			return true;
		}
		else	return false;
	case 11:
		if(GetAsyncKeyState(VK_F11 & 0x7A)){
			while(1){
				if(!GetAsyncKeyState(VK_F11 & 0x7A))	break;
			}
			return true;
		}
		else	return false;
	case 12:
		if(GetAsyncKeyState(VK_F12 & 0x7B)){
			while(1){
				if(!GetAsyncKeyState(VK_F12 & 0x7B))	break;
			}
			return true;
		}
		else	return false;
	default:
		return false;
	}
}

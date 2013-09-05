/*--------------------------------------------
	Xbox360 GamePad
	2013/07/07 - by.Uno
	参考資料
	http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.xinput_gamepad(v=vs.85).aspx
--------------------------------------------*/

#pragma once
#include <Windows.h>
#include <XInput.h>
#pragma comment(lib, "XInput.lib")

#define MAX_XBOX_CONTROLLER 4 //最大接続数(サポート限界)

class XboxController{
protected:
	int _num;
	XINPUT_STATE _state;
	DWORD Result;

	XINPUT_STATE GetState();
	XINPUT_VIBRATION Vibration;
public:
	XboxController();
	virtual ~XboxController();
	
	void Connect(int PlayerNumber=1);
	bool isConnected();
	void Vibrate(float left=0, float right=0); // input 0 - 1 (0-100%)
	bool isVibrate();
	
	inline int getPlayerNumber(){
		return _num;
	}
	inline int getButton(){
		return _state.Gamepad.wButtons;
	}
	inline float getTriger(bool LR=0){ // return 0 - 1
		return (!LR)? (_state.Gamepad.bLeftTrigger / 255.0f) : (_state.Gamepad.bRightTrigger / 255.0f);
	}
	inline float getThumbX(bool LR=0){ // return -1 - 1
		return (!LR)? (_state.Gamepad.sThumbLX / 32768.0f) : (_state.Gamepad.sThumbRX / 32768.0f);
	}
	inline float getThumbY(bool LR=0){ // return -1 - 1
		return (!LR)? (_state.Gamepad.sThumbLY / 32768.0f) : (_state.Gamepad.sThumbRY / 32768.0f);
	}
};
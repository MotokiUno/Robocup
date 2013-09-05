#include "XboxController.h"


XINPUT_STATE XboxController::GetState(){
	ZeroMemory(&_state, sizeof(XINPUT_STATE));
	XInputGetState(_num, &_state);

	return _state;
}

XboxController::XboxController(){
	_num = 0;
}
XboxController::~XboxController(){

}

void XboxController::Connect(int PlayerNumber){
	_num = PlayerNumber - 1;
}

bool XboxController::isConnected(){
	ZeroMemory(&_state, sizeof(XINPUT_STATE));
	Result = XInputGetState(_num, &_state);

    return (Result == ERROR_SUCCESS)? true : false;
}

void XboxController::Vibrate(float left, float right){
	//Init
    ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

    // Set Value
    Vibration.wLeftMotorSpeed = (int)(left*65535.0f);
    Vibration.wRightMotorSpeed = (int)(right*65535.0f);

    XInputSetState(_num, &Vibration);
}
bool XboxController::isVibrate(){
	return (Vibration.wLeftMotorSpeed > 0 || Vibration.wRightMotorSpeed > 0)? true : false;
}
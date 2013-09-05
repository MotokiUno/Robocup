#include <stdio.h>
#include <string>

#include "serial.h"
using namespace std;
#define ROBOT_NUM 5

struct robot{
	unsigned int motor[4];//回転数0-127
	bool motor_d[4];//正・逆
	bool hold;//ホールド on off
	bool kick;//キック on off
	unsigned int action;
};
enum{
	DO_NOTHING,//何もしない
	SEND,//送信
	PASS,//パス
	KICKM//キック
};


//パリティチェック
bool parity(char a){
	int cnt = 0;
	for(int i=0; i<8; i++){
		if(a&(1<<i)){ cnt++; };
	}
	if(cnt%2){
		return false;
	}else{
		return true;
	}
}

int main(){
	serial Serial;

	robot robot[ROBOT_NUM];
	memset(&robot, 0, sizeof(robot));

	unsigned char data[1+5*ROBOT_NUM];


	if(Serial.Setup("COM1", 19200)){
		int speed = 60;

/*
//前進
robot[0].motor[0] = speed;
robot[0].motor[1] = speed;
robot[0].motor[2] = speed;
robot[0].motor[3] = speed;

robot[0].motor_d[0] = 0;
robot[0].motor_d[1] = 1;
robot[0].motor_d[2] = 0;
robot[0].motor_d[3] = 1;


//後進
robot[0].motor[0] = speed;
robot[0].motor[1] = speed;
robot[0].motor[2] = speed;
robot[0].motor[3] = speed;

robot[0].motor_d[0] = 1;
robot[0].motor_d[1] = 0;
robot[0].motor_d[2] = 1;
robot[0].motor_d[3] = 0;


//左サイド
robot[0].motor[0] = speed;
robot[0].motor[1] = speed;
robot[0].motor[2] = speed;
robot[0].motor[3] = speed;

robot[0].motor_d[0] = 1;
robot[0].motor_d[1] = 1;
robot[0].motor_d[2] = 0;
robot[0].motor_d[3] = 0;

//右サイド
robot[0].motor[0] = speed;
robot[0].motor[1] = speed;
robot[0].motor[2] = speed;
robot[0].motor[3] = speed;

robot[0].motor_d[0] = 0;
robot[0].motor_d[1] = 0;
robot[0].motor_d[2] = 1;
robot[0].motor_d[3] = 1;

//ホールド
robot[0].hold = true;

//右旋回
robot[0].motor[0] = speed;
robot[0].motor[1] = speed;
robot[0].motor[2] = speed;
robot[0].motor[3] = speed;

robot[0].motor_d[0] = 0;
robot[0].motor_d[1] = 0;
robot[0].motor_d[2] = 0;
robot[0].motor_d[3] = 0;
*/
//左旋回
robot[0].motor[0] = speed;
robot[0].motor[1] = speed;
robot[0].motor[2] = speed;
robot[0].motor[3] = speed;

robot[0].motor_d[0] = 1;
robot[0].motor_d[1] = 1;
robot[0].motor_d[2] = 1;
robot[0].motor_d[3] = 1;

		bool flag = false;
		while(!flag){
			//Escapeで停止信号を送った後　終了する。
			if(GetKeyState(VK_ESCAPE)<0){
				memset(&robot, 0, sizeof(robot));
				flag = true;
			}

///////////////////////////////////////////////////////////////////////////////
	//送信用の信号に変換する。

	data[0] = 0x42; //スタートビット

	for(int i=0; i<ROBOT_NUM; i++){
		//モーター回転数+パリティ 4byte
		for(int j=0; j<4; j++){
			data[i*5+1+j] = robot[i].motor[j] << 1 | (unsigned int)parity(robot[i].motor[j]);
		}
		//5byte目
		unsigned char tmp[1] = "";
		for(int j=0; j<4; j++){
			tmp[0] = tmp[0] | robot[i].motor_d[j] << (6-j);
		}
		tmp[0] =  tmp[0] | robot[i].hold << 2;
		tmp[0] =  tmp[0] | robot[i].kick << 1;
		tmp[0] =  tmp[0] | robot[i].action;

		data[(i+1)*5] = tmp[0] << 1 | (unsigned int)parity(tmp[0]);
	}

	//送信信号の表示
	for(int i=0; i<(1+5*ROBOT_NUM);i++){
		printf("%2x",data[i]);
	}
	printf("\n");

//////////////////////////////////////////////////////////////////////////////////////

			Serial.WriteBytes((unsigned char *)data, 1+5*ROBOT_NUM);
			Sleep(10);
		}
		Serial.Close();
	}
	return 0;
}
#include <stdio.h>
#include "serial.h"

int main(){
	serial Serial;

	if(Serial.Setup("COM8", 38400)){
		int i, sum;
		int com=1;// 1 前進 16 キーパー
		BYTE comRCB[8]="";

		//コマンドであることを指定する。
		comRCB[0] = 0x80;

		// 引数comの下位 2 byte を代入する。
		comRCB[1] = ((com & 0x7F00) >> 8);
		comRCB[2] = (com & 0x7F);

		// チェックサムを計算
		sum = 0;
		for(i=0; i<6; i++){
			sum += (int)comRCB[i];
		}
		comRCB[7] = (sum & 0x7F);

		// シリアルポートから送信
		while(1){
			for(i=0;i<3;i++){
				Serial.WriteBytes((unsigned char *)comRCB,8);
				Sleep(10);
			}
			//Escapeで終了
			if(GetKeyState(VK_ESCAPE)){
				break;
			}
		}
		Serial.Close();
	}

	return 0;
}
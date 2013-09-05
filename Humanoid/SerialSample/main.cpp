#include <stdio.h>
#include "serial.h"

int main(){
	serial Serial;

	if(Serial.Setup("COM8", 38400)){
		int i, sum;
		int com=1;// 1 �O�i 16 �L�[�p�[
		BYTE comRCB[8]="";

		//�R�}���h�ł��邱�Ƃ��w�肷��B
		comRCB[0] = 0x80;

		// ����com�̉��� 2 byte ��������B
		comRCB[1] = ((com & 0x7F00) >> 8);
		comRCB[2] = (com & 0x7F);

		// �`�F�b�N�T�����v�Z
		sum = 0;
		for(i=0; i<6; i++){
			sum += (int)comRCB[i];
		}
		comRCB[7] = (sum & 0x7F);

		// �V���A���|�[�g���瑗�M
		while(1){
			for(i=0;i<3;i++){
				Serial.WriteBytes((unsigned char *)comRCB,8);
				Sleep(10);
			}
			//Escape�ŏI��
			if(GetKeyState(VK_ESCAPE)){
				break;
			}
		}
		Serial.Close();
	}

	return 0;
}
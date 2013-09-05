/*--------------------------------------------------
	ƒVƒŠƒAƒ‹’ÊM ver 1.0

	2013/08/21 -

--------------------------------------------------*/


#pragma once
#include <Windows.h>
#include <string>
#define DEFAULT_SERIAL_RATE 9600
#define SERIAL_ERROR -1

class serial{
public:
	serial();
	virtual ~serial();

	bool Setup(std::string name, int rate=0);
	bool Close();

	int WriteBytes(unsigned char * buffer, int length);
	int ReadBytes(unsigned char * buffer, int length);

	int WriteByte(unsigned char singleByte);
	int ReadByte(unsigned char singleByte);

	//î•ñæ“¾
	int getRate(){
		return _rate;
	}
protected:
	HANDLE _hCom;
	int _rate;
};
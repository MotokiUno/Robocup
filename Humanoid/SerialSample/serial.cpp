#include "serial.h"


serial::serial(){
	_hCom = INVALID_HANDLE_VALUE;
	_rate = DEFAULT_SERIAL_RATE;
}
serial::~serial(){
	Close();
}

bool serial::Setup(std::string name, int rate){
	if(rate>0){ _rate = rate; }
	//シリアルポートを開く
	_hCom = CreateFile(name.c_str(), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//有効かどうか
	if(_hCom==INVALID_HANDLE_VALUE){
		#ifdef _DEBUG
			printf("Serial::Unable to open port\n");
		#endif
		return false;
	}

	//現在の設定を取得し、DCB構造体に格納
	COMMCONFIG cfg;
	DWORD cfgsize = sizeof(cfg);
	char buf[80];

	GetCommConfig(_hCom, &cfg, &cfgsize);

	sprintf(buf,"baud=%d parity=N data=8 stop=1",_rate);
	if(!BuildCommDCBA(buf,&cfg.dcb)){
		//BuildCommDCBA　解説　http://msdn.microsoft.com/ja-jp/library/cc429172.aspx
		#ifdef _DEBUG
			printf("Serial::Unable to build comm dcb\n");
		#endif
	}

	//DCB構造体を反映する。
	if(!SetCommState(_hCom, &cfg.dcb)){
		#ifdef _DEBUG
			printf("Serial::Can't set com state\n");
		#endif
	}

	//タイムアウトの設定
	COMMTIMEOUTS tout;
	GetCommTimeouts(_hCom,&tout);//現在の値を取得

	tout.ReadIntervalTimeout = MAXDWORD;
	tout.ReadTotalTimeoutMultiplier = 0;
	tout.ReadTotalTimeoutConstant = 0;

	SetCommTimeouts(_hCom,&tout);//設定を反映する。

	return true;
}

bool serial::Close(){
	if(_hCom!=INVALID_HANDLE_VALUE){
		CloseHandle(_hCom);
		_hCom = INVALID_HANDLE_VALUE;
	}
	return true;
}


int serial::WriteBytes(unsigned char *buffer, int length){
	if(_hCom==INVALID_HANDLE_VALUE){
		#ifdef _DEBUG
			printf("Serial::Serial not inited\n");
		#endif
		return SERIAL_ERROR;
	}
	DWORD written;
	if(!WriteFile(_hCom, buffer, length, &written, 0)){
		#ifdef _DEBUG
			printf("Serial::Can't write to com port\n");
		#endif
		return SERIAL_ERROR;
	}
	return (int)written;
}

int serial::ReadBytes(unsigned char *buffer, int length){
	if(_hCom==INVALID_HANDLE_VALUE){
		#ifdef _DEBUG
			printf("Serial::Serial not inited\n");
		#endif
		return SERIAL_ERROR;
	}
	DWORD read;
	if(!ReadFile(_hCom, buffer, length, &read, 0)){
		#ifdef _DEBUG
			printf("Serial::Can't read from com port\n");
		#endif
		return SERIAL_ERROR;
	}
	return (int)read;
}

int serial::WriteByte(unsigned char singleByte){
	if(_hCom==INVALID_HANDLE_VALUE){
		#ifdef _DEBUG
			printf("Serial::Serial not inited\n");
		#endif
		return SERIAL_ERROR;
	}

	unsigned char tmp[1];
	tmp[0] = singleByte;

	DWORD written;
	if(!WriteFile(_hCom, tmp, 1, &written, 0)){
		#ifdef _DEBUG
			printf("Serial::Can't write to com port\n");
		#endif
		return SERIAL_ERROR;
	}
	return ( ((int)written > 0)? true : false );
}

int serial::ReadByte(unsigned char singleByte){
	if(_hCom==INVALID_HANDLE_VALUE){
		#ifdef _DEBUG
			printf("Serial::Serial not inited\n");
		#endif
		return SERIAL_ERROR;
	}

	unsigned char tmp[1];
	tmp[0] = singleByte;

	DWORD read;
	if(!ReadFile(_hCom, tmp, 1, &read, 0)){
		#ifdef _DEBUG
			printf("Serial::Can't read from com port\n");
		#endif
		return SERIAL_ERROR;
	}
	return ( ((int)read > 0)? true : false );
}
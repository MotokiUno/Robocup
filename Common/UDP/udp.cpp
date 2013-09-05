#include "udp.h"

udp::udp(){
	//winsock 初期化
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);

	//変数初期化
	_sock = INVALID_SOCKET;
}
udp::~udp(){
	Close();
	WSACleanup();
}

bool udp::Create(){
	if(_sock != INVALID_SOCKET){
		return false;
	}

	//設定
	_sock = socket(AF_INET, SOCK_DGRAM, 0);//UDPなのでSOCK_DGRAMを指定。

	if(_sock != INVALID_SOCKET){
		int unused = true;
		setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&unused, sizeof(unused));//ソケットのオプションを設定する。
	}

	return (_sock != INVALID_SOCKET);
}

bool udp::Bind(unsigned short port){
	_addr.sin_family = AF_INET; 
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = INADDR_ANY;

	//bindシステムコール
	int r = bind(_sock, (struct sockaddr*)&_addr, sizeof(struct sockaddr));
	return (r==0);
}

bool udp::BindMcast(char *mcast, unsigned short port){
	//bind
	if(!Bind(port)){
		#ifdef _DEBUG
			printf("Can't bind to port\n");
		#endif
		return false;
	}

	//マルチキャストグループに参加する。(bind後に行う必要あり)
	struct ip_mreq mreq;
	memset(&mreq, 0, sizeof(mreq));
	mreq.imr_interface.s_addr = INADDR_ANY;
	mreq.imr_multiaddr.s_addr = inet_addr(mcast);

	if(setsockopt(_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char FAR*)&mreq, sizeof(mreq)) == SOCKET_ERROR){
		//Error
		#ifdef _DEBUG
			printf("setsockopt error\n");
		#endif
		return false;
	}
	
	return true;
}

int udp::Send(const char *buff, const int size){
	if(_sock == INVALID_SOCKET){
		return SOCKET_ERROR;
	}

	//指定されたアドレスにデータを送信する。
	int dsize = sendto(_sock, buff, size, 0, (struct sockaddr *)&_client, sizeof(_addr));

	return dsize;
}

int udp::Receive(char *buff, const int size){
	if(_sock == INVALID_SOCKET){
		return SOCKET_ERROR;
	}

	//初期化
	memset(buff, 0, size);
	int len = sizeof(_addr);

	//データと送信側のアドレスを受信する。
	int dsize = 0;
	dsize = recvfrom(_sock, buff, size, 0, (sockaddr *)&_client, &len);

	return dsize;
}

bool udp::SetNonBlocking(bool b){
	unsigned long arg = b;
	int r = ioctlsocket(_sock, FIONBIO, &arg);
	return (r>=0);
}

bool udp::Close(){
	if(_sock != INVALID_SOCKET){
		if(closesocket(_sock) == SOCKET_ERROR){
			return false;
		}
		_sock = INVALID_SOCKET;
	}
	return true;
}
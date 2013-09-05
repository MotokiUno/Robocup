/*----------------------------------------
	UDP通信

	2013/08/19 - 

	参考サイト
	http://www.golgo.jp/winsock.html
	http://www.geekpage.jp/programming/linux-network/multicast-2.php
----------------------------------------*/

#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>

class udp{
public:
	//コンストラクタ
	udp();
	//デストラクタ
	virtual ~udp();

	//関数
	bool Create(); 
	bool Bind(unsigned short port);
	bool BindMcast(char *mcast, unsigned short port);//マルチキャスト指定

	int Send(const char *buff, const int size);//送信
	int Receive(char *buff, const int size);//受信

	bool SetNonBlocking(bool b);//非同期通信　有効/無効

	bool Close();//閉じる

	bool isAvailable(){
		return (_sock!=INVALID_SOCKET);
	}
protected:
	struct sockaddr_in _addr,_client;
	SOCKET _sock;
};
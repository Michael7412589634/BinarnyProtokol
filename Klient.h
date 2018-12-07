#pragma once
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib") 

class Klient
{
public:
	u_int64 sessionID;
	SOCKET clientSocket;
	Klient(SOCKET &sock, u_int64 id);
	~Klient();
};

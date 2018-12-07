#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include "Protokol.h"
#include "Klient.h"
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

#pragma comment(lib,"ws2_32.lib") 

class Serwer
{

	//pola danych
	static vector<Klient*> klienci;
	WSAData wsaData;
	WORD DllVersion;
	SOCKADDR_IN addr; //adres
	SOCKET sListen;
	int addrLen;

	bool wyslij(Klient * klient, Protokol * protokol); //funkcja wysylajaca

	Protokol * odbierz(Klient * klient);//funkcja odbierajaca wiadomosc

	bool processerweraq(Klient * klient, Protokol * protokol);//zarzadzanie serwerem
	static void obslugaklientow(Klient * klient);

public:
	
	Serwer(int PORT, bool lokalnie = false);
	bool czekajnapolaczenie();//czeka na polaczenie

};

static Serwer * wskserwer;

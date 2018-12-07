#pragma once

#include "Bajty.h"
#include "Pakiet.h"
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib") 

class Protokol
{
	Bajty pakiet;
	Bajt ph;
	Bajt8 rozmiar;
	string dane;
	Bajt8 id;

public:

	Protokol(ph _packetHeader, u_int64 rozmiar = 0, string dane = "", u_int64 _id = 0);//protokol wysylajacy
	Protokol(char * otrzymane, int rozmiar);//protokol otrzymujacy
	char *  wiadomoscdowyslania();//string do wyslania
	int wezrozmiarwiadomosci();
	ph wezpolaprotokolu();//z otrzymanych danych
	u_int64 wezrozmiardanych();
	string wezdane();
	u_int64 wezid();
	~Protokol();
};

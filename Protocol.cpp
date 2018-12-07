#include "Protokol.h"


Protokol::Protokol(ph ph, u_int64 rozmiardanych, string dane, u_int64 idsesji)
{
	ph = (char)ph;
	rozmiar = rozmiardanych;
	dane = dane;
	id = idsesji;
	pakiet.dodaj_bity(ph, 7);
	pakiet.dodaj_bajty(rozmiar);
	if (rozmiardanych > 0)
	{
		pakiet.dodaj_bajty(dane, dane.size());
	}
	pakiet.dodaj_bajty(id);
}

Protokol::Protokol(char * received, int _size)
{
	pakiet.dodaj_bajty(received, _size);
	ph = pakiet.wez_bity(0, 0, 7);
	rozmiar = pakiet.wez_Bajt8(0, 8, 7); // take 8 bytes from 7 bit of 0 byte
	if (_size > 9)
	{
		if (rozmiar.to_ullong() > 0) 
		{
			dane = wezdane();
		}
		else
		{
			dane = "";
		}
		id = pakiet.wez_Bajt8(_size - 9, 8, 7);
	}
}

char * Protokol::wiadomoscdowyslania()
{
	vector<Bajt> temp = pakiet.wez_bajty();
	int messageLenght = temp.size();
	char * toSend = new char[messageLenght];
	for (int i = 0; i < messageLenght; i++)
	{
		toSend[i] = temp[i].to_ullong();
	}
	return toSend;
}

int Protokol::wezrozmiarwiadomosci()
{
	return pakiet.wez_rozmiar();
}

ph Protokol::wezpolaprotokolu()
{
	return static_cast<ph>(ph.to_ulong());
}

u_int64 Protokol::wezrozmiardanych()
{
	return rozmiar.to_ullong();
}

string Protokol::wezdane()
{
	string result = "";
	vector<Bajt> temp = pakiet.wez_bajty(8, rozmiar.to_ullong(), 7);
	for (auto a : temp)
	{
		result += static_cast<char>(a.to_ullong());
	}
	return result;
}

u_int64 Protokol::wezid()
{
	return id.to_ullong();
}

Protokol::~Protokol()
{

}

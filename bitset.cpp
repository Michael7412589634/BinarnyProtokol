#include "stdafx.h"
#include "Bajty.h"


Bajty::Bajty()
{
	obecny_bit = 7;
	obecny_bajt = 0;
	rozmiar = 0;
}

Bajty::~Bajty() {}

void Bajty::dodaj_bajty(string _bytes, size_t _size)
{
	for (int i = 0; i < _size; i++) {
		Bajt bity8 = static_cast<Bajt>(_bytes[i]);
		dodaj_bity(bity8, 8);
	}
}

void Bajty::dodaj_bajty(char * _bytes, size_t _size)
{
	for (int i = 0; i < _size; i++) {
		Bajt bity8 = static_cast<Bajt>(_bytes[i]);
		dodaj_bity(bity8, 8);
	}
}

void Bajty::dodaj_bity(Bajt _byte, size_t _size)
{
	int test = obecny_bit;
	if (_size > test || rozmiar == 0)
	{
		bajty.push_back(0);
		rozmiar++;
	}
	for (int i = 0, j = 7; i < _size && j >= 0; i++, j--) {
		bajty[obecny_bajt][obecny_bit] = _byte[j];
		obecny_bit--;
		if (obecny_bit < 0)
		{
			obecny_bit = 7;
			obecny_bajt++;
		}
	}
}

void Bajty::dodaj_bajty(Bajt8 _bytes, size_t _sizeInBits)
{
	int rozmiar = 8;
	for (int i = 7; i >= 0; i--)
	{
		Bajt bity8;
		for (int j = 7 + i*rozmiar, k = 7; j >= 0 + i*rozmiar; j--, k--) {
			bity8[k] = _bytes[j];
		}
		dodaj_bity(bity8, 8);
	}
}

Bajt Bajty::wez_bity(size_t _bitBeginIndex, size_t _byteIndex, size_t _bitNumber)
{
	Bajt wynik;
	int aktualnybit = 7 - _bitBeginIndex;
	for (int i = 0, j = 7; i < _bitNumber && j >= 0; i++, j--)
	{
		wynik[j] = bajty[_byteIndex][aktualnybit];
		aktualnybit--;
		if (aktualnybit < 0)
		{
			aktualnybit = 7;
			_byteIndex++;
		}
	}
	return wynik;
}

Bajt8 Bajty::wez_Bajt8(size_t poczatkowyindeks, size_t liczbabajtow, size_t poczindeksbitu)
{
	Bajt8 wynik;
	Bajt bit8;
	int rozmiar = 8;
	for (int i = 0; i < 8; i++)
	{
		bit8 = wez_bity(poczindeksbitu, poczatkowyindeks + liczbabajtow - 1 - i);
		for (int j = 7 + i*rozmiar, k = 7; j >= 0 + i*rozmiar; j--, k--) {
			wynik[j] = bit8[k];
		}
	}
	return wynik;
}

vector<Bajt> Bajty::wez_bajty(size_t poczatkowyindeks, size_t liczbabitow, size_t poczindeksbitu)
{
	vector<Bajt> wynik;
	Bajt bajt;
	if (liczbabitow<0 || liczbabitow > rozmiar)
	{
		liczbabitow = rozmiar;
	}
	for (int i = 0; i < liczbabitow; i++)
	{
		bajt = wez_bity(poczindeksbitu, poczatkowyindeks + i);
		wynik.push_back(bajt);
	}
	return wynik;
}

string Bajty::na_string()
{
	string wynik = "", bajtstring;
	for (auto a : bajty) {
		bajtstring = a.to_string();
		wynik += bajtstring + " ";
	}
	return wynik;
}

size_t Bajty::wez_rozmiar()
{
	return rozmiar;
}

#pragma once

#include <bitset>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

typedef ustawbit<8> Bajt;
typedef ustawbit <64> Bajt8;

class Bajty
{
	vector<Bajt> bajty;
	int obecny_bajt;
	int obecny_bit;
	int rozmiar;

public:
	Bajty();
	~Bajty();
	void dodaj_bajty(string zbajty, size_t zrozmiar);
	void dodaj_bity(Bajt zbajty, size_t zrozmiar);
	void dodaj_bajty(char * zbajty, size_t zrozmiar);
	void dodaj_bajty(Bajt8 zbajty, size_t zrozmiarwbitach = 64);
	Bajt wez_bity(size_t poczatkowyindeks, size_t indeks, size_t nrbitu = 8);
	Bajt8 wez_Bajt8(size_t poczatkowyindeks, size_t liczbabajtow, size_t nrbitu = 0);
	vector<Bajt> wez_bajty(size_t poczatkowyindeks = 0, size_t liczbabajtow = -1, size_t ntbitu = 0);
	size_t wez_rozmiar();
	string na_string();
};

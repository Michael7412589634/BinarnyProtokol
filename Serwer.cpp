#include "Serwer.h"

vector<Klient*> Serwer::klienci;

Serwer::Serwer(int PORT, bool broadcastLocally)
{
	DllVersion = MAKEWORD(2, 2);
	//na bazie Winsock inicjalizacja IP, portu itd.
	
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	
	addrLen = sizeof(addr); 
	if (broadcastLocally) { 
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	addr.sin_port = htons(PORT); 
	addr.sin_family = AF_INET; 

							  
	sListen = socket(AF_INET, SOCK_STREAM, NULL); 
	if (::bind(sListen, (SOCKADDR*)&addr, addrLen) == SOCKET_ERROR) 
	{
		cout << "Blad bindowania: " << WSAGetLastError() << ".\n";
	}
	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR) 
	{
		cout << "Blad nasluchiwania: " << WSAGetLastError() << ".\n";
	}
	wskserwer = this;
}

bool Serwer::czekajnapolaczenie()
{
	SOCKET nowepolaczenie; 
	nowepolaczenie = accept(sListen, (SOCKADDR*)&addr, &addrLen); //akcptuj nowe polaczenie
	if (nowepolaczenie == 0) //jesli poloczenie sie nie powiedzie
	{
		std::cout << "Blad przy akceptacji klienta: " << WSAGetLastError() << ".\n";
		return false;
	}
	else //polaczenie sie powiodlo
	{
		default_random_engine engine(static_cast<long unsigned int>(chrono::high_resolution_clock::now().time_since_epoch().count()));
		uniform_int_distribution<int> distro(50, 700);

		Klient* nowyklient = new Klient(nowepolaczenie, distro(engine));
		std::cout << "Klient polaczony!" << std::endl;
		klienci.push_back(nowyklient);
		
		//watek do obslugi klienta

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)obslugaklientow, (LPVOID)nowyklient, NULL, NULL);
		return true;
	}
}

bool Serwer::processerweraq(Klient * klient, Protokol * protokol)
{
	if (protokol == nullptr)
		return false;

	ph p = protokol->wezpolaprotokolu();
	u_int64 rozmiar = protokol->wezrozmiardanych();
	string data = "";
	u_int64 SesionID = protokol->wezid();
	if (rozmiar > 0)
	{
		protokol->wezdane();
	}
	switch (p)
	{
	case ph::ServerRequestID:
	{
		wyslij(klient, new Protokol(ph::ServerSendID, 0, "", klient->sessionID));
		cout << "Wyslane ID: " << klient->sessionID << endl;
		break;
	}
	case ph::Quit:
	{
		cout << "Klient o ID: " << klient->sessionID << " rozlaczyl sie"<< endl;
		
		for (auto a : klienci)
		{
			if (a == klient) {
				continue;
			}
			if (wyslij(a, new Protokol(ph::ServerAlone, 0, "", a->sessionID))) {
				cout << "Wiadomosc wyslana do klienta o ID = " << a->sessionID << endl;
			}
			else {
				cout << "Nie ma juz uzytkownika o ID:" << a->sessionID << endl;
			}
		}
		break;
	}
	case ph::Message:
	{
		for (auto a : klienci)
		{
			if (a == klient) {
				continue;
			}
			if (wyslij(a, protokol)) {
				cout << "Klient o ID:" << a->sessionID << " otrzymal wiadomosc"<< endl;
			}
			else {
				cout << "Problem z wyslaniem wiadomosci do uzytkownika o ID = " << a->sessionID << endl;
			}
		}
		break;
	}
	case ph::Bye:
	{
		for (auto a : klienci)
		{
			if (a == klient) {
				continue;
			}
			if (wyslij(a, protokol)) {
				cout << "Zakonczono rozmowe z klientem o ID:" << a->sessionID << endl;
			}
			else {
				cout << "Problem z zakonczeniem rozmowy z klientem o ID = " << a->sessionID << endl;
			}
		}
		break;
	}
	case ph::Invite:
	{
		cout << "Klient o ID = " << klient->sessionID << " wyslal zaproszenie"<<endl;
		if (klienci.size() == 1)
		{
			if (wyslij(klient, new Protokol(ph::ServerAlone, 0, "", klient->sessionID))) {
				cout << "Wyslano ServerAlone do klienta o ID = " << klient->sessionID << endl;
			}
			else {
				cout << "Problem z wyslaniem ServerAlone do uzytkownika o ID = " << klient->sessionID << endl;
			}
			break;
		}
		for (auto a : klienci)
		{
			if (a == klient) {
				continue;
			}
			if (wyslij(a, protokol)) {
				cout << "Wyslano zaproszenie do klienta o ID = " << a->sessionID << endl;
			}
			else {
				cout << "Problem z wyslaniem Invite do uzytkownika o ID = " << a->sessionID << endl;
			}
		}
		break;
	}
	case ph::InviteAccpet:
	{
		cout << "Klient o ID = " << klient->sessionID << " przyjal zaproszenie"<< endl;
		for (auto a : klienci)
		{
			if (a == klient) {
				continue;
			}
			if (wyslij(a, protokol)) {
				cout << "Przeslano InviteAccept do klienta o ID = " << a->sessionID << endl;
			}
			else {
				cout << "Wystapil problem z wyslaniem InviteAccept do uzytkownika o ID = " << a->sessionID << "\n";
			}
		}
		break;
	}
	case ph::InviteDecline:
	{
		cout << "Klient o ID = " << klient->sessionID << " odrzucil zaproszenie"<< endl;
		for (auto a : klienci)
		{
			if (a == klient) {
				continue;
			}
			if (wyslij(a, protokol)) {
				cout << "Przeslano InviteDecline do klienta o ID = " << a->sessionID << endl;
			}
			else {
				cout << "Problem z wyslaniem InviteDecline do uzytkownika o ID = " << a->sessionID << endl;
			}
		}
		break;
	}
	default:
		cout << "Nieznany typ" << endl;
		break;
	}
	return true;
}

bool Serwer::wyslij(Klient * client, Protokol * protocol)
{
	char * message = protocol->wiadomoscdowyslania();
	int messageLenght = protocol->wezrozmiarwiadomosci();
	int check = send(client->clientSocket, message, messageLenght, NULL);
	if (check == SOCKET_ERROR) //jesli blad
		return false;
	return true;
}

Protokol *  Serwer::odbierz(Klient * klient)
{
	Protokol * protokol;
	int rozmiarbufora = 1500;
	char * bufor = new char[rozmiarbufora];
	int sprawdz = recv(klient->clientSocket, bufor, rozmiarbufora, NULL);
	if (sprawdz == SOCKET_ERROR || sprawdz == 0)
		return nullptr;
	protokol = new Protokol(bufor, 9);
	u_int64 rozmiar = protokol->wezrozmiardanych();
	delete protokol;
	protokol = new Protokol(bufor, 17 + rozmiar);
	delete[] bufor;

	return protokol;
}

void Serwer::obslugaklientow(Klient * klient) 
{
	Protokol * protokol;
	while (true)
	{
		protokol = wskserwer->odbierz(klient);
		if (protokol == nullptr) //
			break; 
		if (!wskserwer->processerweraq(klient, protokol))
			break; 
	}
	std::cout << "Rozlaczono z klientem o ID sesji: " << klient->sessionID << "." << std::endl;
	if (klient->clientSocket != INVALID_SOCKET)
		closesocket(klient->clientSocket); 
	klienci.erase(remove(klienci.begin(), klienci.end(), klient), klienci.end());
	delete klient;
}

#include "Klient.h"


Klient::Klient(SOCKET &sock, u_int64 id) : clientSocket(sock), sessionID(id) {}

Klient::~Klient()
{}

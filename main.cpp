#include <iostream>
//#include "socket.h"
#include "TcpServer.h"

#pragma comment (lib,"Ws2_32.lib")

using namespace std;

int main(void)
{
	setlocale(LC_ALL, "RUS");

	TcpServer *server = new TcpServer;
	server->ServerFunction("localhost", "8888", 5);

	delete server;

	system("pause");
	return 0;
}
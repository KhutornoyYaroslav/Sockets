#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <Ws2tcpip.h>

class SocketClass {

private:

	SOCKET socket_handle;
	sockaddr_in sockaddr_struct;


	int set_address( char *hname, char *sname, struct sockaddr_in *sap, char *protocol, int family );
	int create_WSA( void );
	int clear_WSA( void );

public:

	SocketClass(void);
	~SocketClass(void);

	SOCKET create_TCP_listen_socket( char *hname, char *sname );
	SOCKET create_TCP_connect_socket( char *hname, char *sname );
	SOCKET create_UDP_server_socket( char *hname, char *sname ); 
	SOCKET create_UDP_client_socket( char *hname, char *sname ); 

	void set_error( int status, int err, char *fmt, ... );
	int clear_socket( int how );
};


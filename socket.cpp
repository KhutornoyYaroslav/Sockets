#include "socket.h"

SocketClass::SocketClass(void) {

};

SocketClass::~SocketClass(void) {

};

int SocketClass::set_address(char *hname, char *sname, sockaddr_in *sap, char *protocol, int family = AF_INET ) {

	servent *sp;
	hostent *hp;
	char *endptr;
	long port;

	memset( sap, 0, sizeof( *sap ) );
	sap->sin_family = family;
	
	if( hname != NULL )
	{
		if( !InetPton( family, hname, &sap->sin_addr ) )
		{
			hp = gethostbyname( hname );
			if ( hp == NULL ) {
				set_error( 0, 0, "Неизвестный хост: %s\n", hname);
				return 1;
			}
			sap->sin_addr = *( in_addr* )hp->h_addr;
		}
	}
	else
		sap->sin_addr.s_addr = htonl( INADDR_ANY );

	port = strtol( sname, &endptr, 0);
	if ( *endptr == '\0' )
		sap->sin_port = htons( port );
	else
	{
		sp = getservbyname( sname, protocol );
		if ( sp == NULL ) {
			set_error( 0, 0, "Неизвестный сервис: %s\n", sname );
			return 1;
		}
		sap->sin_port = sp->s_port;
	}

	return 0;
}

void SocketClass::set_error( int status, int err, char *fmt, ... )
{
	va_list argptr;
	va_start( argptr, fmt );
	vfprintf( stderr, fmt, argptr );
	va_end( argptr );
	if ( err )
		fprintf( stderr, ": %s (%d)\n", strerror( err ), err );
	if ( status )
		exit( status );
}

int SocketClass::create_WSA( void ) {

	WSADATA wsa_struct;
	WORD wVersionRequested;
	
	wVersionRequested = MAKEWORD(2, 2);

	if( WSAStartup(wVersionRequested, &wsa_struct) ) {
		set_error(0, 0, "Неудалось запустить WSA.\n");
		return 1;
	}

	if (LOBYTE(wsa_struct.wVersion) != 2 || HIBYTE(wsa_struct.wVersion) != 2) {
		set_error( 0, 0, "Рабочая версия библиотеки WinSock не найдена.\n");
        WSACleanup();
        return 1;
    }
    else
        return 0;
};

int SocketClass::clear_WSA( void ) {
	
	if(WSACleanup()) {
		set_error(0, 0, "Неудалось завершить WSA.\n");
		return 1;
	}
	else
		return 0;
};

SOCKET SocketClass::create_TCP_listen_socket( char *hname, char *sname ) {

	if( create_WSA() ) 
		return -1;

	if( set_address(hname, sname, &sockaddr_struct, "tcp", AF_INET) ) {
		clear_WSA();
		return -1;
	}

	socket_handle = socket( AF_INET, SOCK_STREAM, 0 );
	if( socket_handle == -1) {
		set_error( 0, errno, "Ошибка вызова socket" );
		clear_WSA();
		return -1;
	}

	if ( bind( socket_handle, ( sockaddr * ) &sockaddr_struct, sizeof( sockaddr_struct ) ) ) {
		set_error( 0, errno, "Ошибка вызова bind");
		clear_WSA();
		return -1;
	}

	if ( listen( socket_handle, SOMAXCONN ) ) {
		set_error( 0, errno, "Ошибка вызова listen");
		clear_WSA();
		return -1;
	}

	return socket_handle;
};

SOCKET SocketClass::create_TCP_connect_socket( char *hname, char *sname ) {

	if(create_WSA()) 
		return -1;

	if( set_address(hname, sname, &sockaddr_struct, "tcp", AF_INET) ) {
		clear_WSA();
		return -1;
	}

	socket_handle = socket( AF_INET, SOCK_STREAM, 0 );
	if( socket_handle == -1) {
		set_error( 0, errno, "Ошибка вызова socket" );
		clear_WSA();
		return -1;
	}

	if( connect( socket_handle, ( sockaddr * ) &sockaddr_struct, sizeof( sockaddr_struct ) ) ) {
		set_error(0, errno, "Ошибка вызова connect" );
		clear_WSA();
		return -1;
	}

	return socket_handle;
};

SOCKET SocketClass::create_UDP_server_socket( char *hname, char *sname ) {

	if( create_WSA() ) 
		return -1;

	if( set_address(hname, sname, &sockaddr_struct, "udp", AF_INET) ) {
		clear_WSA();
		return -1;
	}

	socket_handle = socket( AF_INET, SOCK_DGRAM, 0 );
	if( socket_handle == -1) {
		set_error( 0, errno, "Ошибка вызова socket" );
		clear_WSA();
		return -1;
	}

	if ( bind( socket_handle, ( sockaddr * ) &sockaddr_struct, sizeof( sockaddr_struct ) ) ) {
		set_error( 0, errno, "Ошибка вызова bind");
		clear_WSA();
		return -1;
	}

	return socket_handle;
};

SOCKET SocketClass::create_UDP_client_socket( char *hname, char *sname ) {

	if( create_WSA() ) 
		return -1;

	if( set_address(hname, sname, &sockaddr_struct, "udp", AF_INET) ) {
		clear_WSA();
		return -1;
	}

	socket_handle = socket( AF_INET, SOCK_DGRAM, 0 );
	if( socket_handle == -1) {
		set_error( 0, errno, "Ошибка вызова socket" );
		clear_WSA();
		return -1;
	}

	return socket_handle;
};

int SocketClass::clear_socket( int how = SD_BOTH ) {

	//if ( shutdown( socket_handle, how ) != 0 ) {
	//	set_error(0, errno, "Ошибка вызова shutdown");
	//	return 1;
	//};

	if ( closesocket( socket_handle ) != 0 ) {
		set_error(0, errno, "Ошибка вызова closesocket");
		return 1;
	};

	if ( clear_WSA() != 0 ) {
		return 1;
	};

	return 0;
};
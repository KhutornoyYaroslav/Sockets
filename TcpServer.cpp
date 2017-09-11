#include "TcpServer.h"

int TcpServer::assert_counter = 0;

TcpServer::TcpServer(void) {

}


TcpServer::~TcpServer(void) {

}

int TcpServer::ServerFunction( char *hname, char *sname, int num_threads ) {

	listen_handle = create_TCP_listen_socket( hname, sname );
	if( listen_handle < 0 ) {
		set_error(0, errno, "��������� ������� ������.\n");
		return -1;
	}

	if( num_threads > 0 )
		nthreads = num_threads;
	else {
		set_error(0, errno, "����������� ����� ��������� ������� �������.\n");
		return -1;
	}

	//�������� ��������� �������
	HANDLE *thread_handle = new HANDLE;

	for (int i = 0; i != nthreads; i++ ) {

		thread_handle[i] = CreateThread(NULL, 0, ThreadExecute, this, 0, NULL); 

		if( thread_handle == NULL ) {
			set_error(0, errno, "��������� ����� CreateThread.\n");
			return -1;
		}
	}


	while(true) { 
	
	};
	
	delete []thread_handle;
	return 0;
}


DWORD WINAPI TcpServer::ThreadExecute( PVOID arg )  { 

	TcpServer *this_ptr = ( TcpServer * ) arg;
	this_ptr->ThreadRun();
	return 0;
};


inline int TcpServer::ThreadRun( void ) {

	TcpServer::assert_counter++;
	//std::cout << "Assert_counter = " << TcpServer::assert_counter << std::endl;
	printf("Assert_counter = %i\n", TcpServer::assert_counter);



	////�� �������� ������������ ����������� ������. 
	////� �������� ��-�� ����� ��� ������, ���������
	////� ������������� ����� ����� ������

	//TcpServer::assert_counter++;
	//SOCKET accept_handle;
	//sockaddr_in accept_addr;
	//int accpet_addr_size = sizeof( accept_addr );

	////��� �������� ����� �������
	//accept_handle = accept(listen_handle, ( sockaddr* )&accept_addr, &accpet_addr_size );

 //   if (accept_handle == INVALID_SOCKET) {	
	//	//set_error
	//	std::cout << " ERRRRRROR \n" << std::endl;
	//	return -1; //continue;
	//}
	//	
	//char buf[128];


	//send(accept_handle, "Kello\n", 6, 0);

	//closesocket(accept_handle); //shutdown �����?
	return 0;
};



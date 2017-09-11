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
	//int i =0;//
		thread_handle[i] = CreateThread(NULL, 0, ThreadFunction, this, 0, NULL); 

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

// ����������� ������� ��� ������� ��� ����
DWORD WINAPI TcpServer::ThreadFunction( PVOID arg )  { 

	int i;//////////////
	i = rand();//////////////
	//std::cout << i << std::endl;//////////////
	//std::cout << TcpServer::assert_counter << std::endl;//////////////
	//�� �������� ������������ ����������� ������. 
	//� �������� ��-�� ����� ��� ������, ���������
	//� ������������� ����� ����� ������

	TcpServer::assert_counter++;
	SOCKET accept_handle;
	sockaddr_in accept_addr;

	//����������� ����
	TcpServer *this_ptr = (TcpServer *) arg;
	SOCKET listen_handle = this_ptr->listen_handle;
	int accpet_addr_size = sizeof( accept_addr );

	//��� �������� ����� �������
	accept_handle = accept(listen_handle, ( sockaddr* )&accept_addr, &accpet_addr_size );

    if (accept_handle == INVALID_SOCKET) {	
		//set_error
		std::cout << " ERRRRRROR \n" << std::endl;
		return -1; //continue;
	}
		
	char buf[128];
	int s;
	//� ����� ����� ����� ����� �������
	//std::cout << "���������� accpet = " << accept_handle << " ; ������� ����������, �� ������� ��������� ���������: ";
	//std::cin >> s;
	//send(s, "Kello\n", 6, 0);



	send(accept_handle, "Kello\n", 6, 0);

	closesocket(accept_handle); //shutdown �����?
	return 0; 
}


#include "TcpServer.h"

int TcpServer::assert_counter = 0;

TcpServer::TcpServer(void) {

}


TcpServer::~TcpServer(void) {

}

int TcpServer::ServerFunction( char *hname, char *sname, int num_threads ) {

	listen_handle = create_TCP_listen_socket( hname, sname );
	if( listen_handle < 0 ) {
		set_error(0, errno, "Неудалось создать сервер.\n");
		return -1;
	}

	if( num_threads > 0 )
		nthreads = num_threads;
	else {
		set_error(0, errno, "Некорретное число начальных потоков сервера.\n");
		return -1;
	}

	//Создание начальных потоков
	HANDLE *thread_handle = new HANDLE;

	for (int i = 0; i != nthreads; i++ ) {
	//int i =0;//
		thread_handle[i] = CreateThread(NULL, 0, ThreadFunction, this, 0, NULL); 

		if( thread_handle == NULL ) {
			set_error(0, errno, "Неудачный вызов CreateThread.\n");
			return -1;
		}
	}
	
	while(true) { 
	
	};
	
	delete []thread_handle;
	return 0;
}

// попробовать сделать эту функцию как мост
DWORD WINAPI TcpServer::ThreadFunction( PVOID arg )  { 

	int i;//////////////
	i = rand();//////////////
	//std::cout << i << std::endl;//////////////
	//std::cout << TcpServer::assert_counter << std::endl;//////////////
	//Не забывать использовать критические секции. 
	//В основном из-за этого все ошибки, связанные
	//с использование общих полей класса

	TcpServer::assert_counter++;
	SOCKET accept_handle;
	sockaddr_in accept_addr;

	//бесконечный цикл
	TcpServer *this_ptr = (TcpServer *) arg;
	SOCKET listen_handle = this_ptr->listen_handle;
	int accpet_addr_size = sizeof( accept_addr );

	//Тут наверное нужен мьютекс
	accept_handle = accept(listen_handle, ( sockaddr* )&accept_addr, &accpet_addr_size );

    if (accept_handle == INVALID_SOCKET) {	
		//set_error
		std::cout << " ERRRRRROR \n" << std::endl;
		return -1; //continue;
	}
		
	char buf[128];
	int s;
	//А здесь нужен вызов самой функции
	//std::cout << "Дескриптор accpet = " << accept_handle << " ; Введите дескриптор, на который отправить сообщение: ";
	//std::cin >> s;
	//send(s, "Kello\n", 6, 0);



	send(accept_handle, "Kello\n", 6, 0);

	closesocket(accept_handle); //shutdown нужен?
	return 0; 
}


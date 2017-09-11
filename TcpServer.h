#include "socket.h"

class TcpServer : public SocketClass {

private:
	int nthreads;
	static int assert_counter;
	SOCKET listen_handle;

public:
	TcpServer(void);
	~TcpServer(void);


	int ServerFunction( char *hname, char *sname, int num_threads );
	static DWORD WINAPI ThreadFunction( LPVOID t );

};


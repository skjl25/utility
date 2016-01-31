#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <vector>

using namespace std;

class TCPServerSocket
{
public:
	int sock;
	int result;
	int connected;
	int bytes_recieved;
	unsigned int sin_size;
	struct sockaddr_in server_addr,client_addr;


	TCPServerSocket();
	void PrepareConnection(int portNum);
	void AcceptConnection();
	char* ReceiveData(char *sendBuffer);
	char* ReceiveData_auto(char *sendBuffer);
	char* ReceiveFileName();
	void CloseConnection();

	int ReceiveData_cvideo(char *sendBuffer);

};

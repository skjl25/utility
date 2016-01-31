#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <fstream>

using namespace std;

class TCPClientSocket
{
public:
	int sock;
	struct hostent *host;
	struct sockaddr_in server_addr;


	TCPClientSocket();
	void EstablishConnection(char* serverIPAddress, int portNum);
	char* ReadContentsOfFile(char* fileName);
	void SendData(char* sendBuffer, int sizeOfBuffer);
	void EndSendData();

	void CloseConnection();

};

/* tcpserver.c */

#include "TCPServerSocket.h"

TCPServerSocket::TCPServerSocket()
{
	result=1;
	//	int maxSizeToReceive=100000;

}

void TCPServerSocket::PrepareConnection(int portNum)
{
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&result,sizeof(int)) == -1) {
		perror("Setsockopt");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNum);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero),8);

	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
			== -1) {
		perror("Unable to bind");
		exit(1);
	}

	if (listen(sock, 5) == -1) {
		perror("Listen");
		exit(1);
	}
//	printf("\nTCPServer Waiting for client on port %d",portNum);
	fflush(stdout);
}

void TCPServerSocket::AcceptConnection()
{
	sin_size = sizeof(struct sockaddr_in);

	connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);
//	printf("\n I got a connection from (%s , %d)\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

}

char* TCPServerSocket::ReceiveData(char *sendBuffer)
{
	int size=1000;
	int size1=1000000;
	int flag=0;
//	int sizeOfBuffer=153600;
//	int sizeOfBuffer=307200;
	int sizeOfBuffer=921600;
	//char* returnArray=new char[sizeOfBuffer];

	char* buf=(char*)malloc(sizeof(char)*size);
	int totalByteReceived=0;
	while(flag==0)
	{
		int bytes_recieved = recv(connected,buf,size,0);
		if(totalByteReceived<sizeOfBuffer && bytes_recieved>0)
		{
			memcpy(&sendBuffer[totalByteReceived],buf,bytes_recieved);
			totalByteReceived=totalByteReceived+bytes_recieved;
		}
		else if(bytes_recieved<=0)
		{
//			printf("TCPServerSocket::ReceiveData() Total Byte Received:  %d\n",totalByteReceived);
			flag=1;
		}
	}
	delete[] buf;
	buf=NULL;
	return NULL;
}

int TCPServerSocket::ReceiveData_cvideo(char *sendBuffer)
{
	int size=1000;
	int size1=1000000;
	int flag=0;
//	int sizeOfBuffer=153600;
//	int sizeOfBuffer=307200;
	int sizeOfBuffer=3000000;
	//char* returnArray=new char[sizeOfBuffer];

	char* buf=(char*)malloc(sizeof(char)*size);
	int totalByteReceived=0;
	while(flag==0)
	{
		int bytes_recieved = recv(connected,buf,size,0);
		if(totalByteReceived<sizeOfBuffer && bytes_recieved>0)
		{
			memcpy(&sendBuffer[totalByteReceived],buf,bytes_recieved);
			totalByteReceived=totalByteReceived+bytes_recieved;
		}
		else if(bytes_recieved<=0)
		{
//			printf("TCPServerSocket::ReceiveData() Total Byte Received:  %d\n",totalByteReceived);
			flag=1;
		}
	}
	delete[] buf;
	buf=NULL;
	return totalByteReceived;
}



char* TCPServerSocket::ReceiveData_auto(char *sendBuffer)
{
	int size=1000;
	int size1=1000000;
	int flag=0;
//	int sizeOfBuffer=153600;
//	int sizeOfBuffer=307200;
	int sizeOfBuffer=921601;
	//char* returnArray=new char[sizeOfBuffer];

	char* buf=(char*)malloc(sizeof(char)*size);
	int totalByteReceived=0;
	while(flag==0)
	{
		int bytes_recieved = recv(connected,buf,size,0);
		if(totalByteReceived<sizeOfBuffer && bytes_recieved>0)
		{
			memcpy(&sendBuffer[totalByteReceived],buf,bytes_recieved);
			totalByteReceived=totalByteReceived+bytes_recieved;
		}
		else if(bytes_recieved<=0)
		{
//			printf("TCPServerSocket::ReceiveData() Total Byte Received:  %d\n",totalByteReceived);
			flag=1;
		}
	}
	delete[] buf;
	buf=NULL;
	return NULL;
}


char* TCPServerSocket::ReceiveFileName()
{
	int flag=0;
	int size1=100000000;
	char buf[size1];

	bytes_recieved = recv(connected,buf,size1,0);
	buf[bytes_recieved] = '\0';
	if(strcmp("\n",buf)==0)
	{
		printf("Receive File Name Done()\n");
		flag=1;
	}
	else
	{
		size1=bytes_recieved;
		//			fprintf(pFile,"%s",buf);
		printf("%s\n",buf);
	}
	return buf;
}

void TCPServerSocket::CloseConnection()
{
	close(connected);
	close(sock);
}

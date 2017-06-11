/*
TCP client socket for utility library

The MIT License (MIT)

Copyright (c) 2014-2017 Suk Kyu Lee <skjl25@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "TCPClientSocket.h"
#define length(a) ( sizeof ( a ) / sizeof ( *a ) )

#if !defined(ARRAY_SIZE)
		#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#endif

TCPClientSocket::TCPClientSocket() {
	sock=0;
}

char* TCPClientSocket::ReadContentsOfFile(char* fileName){
	size_t result;
	FILE* pFile;
	long lSize;
	char* buffer;

	pFile=fopen(fileName,"r");
	char filename[100];
	strcpy(filename,fileName);

	if(pFile==NULL)
		perror ("Error opening file\n");
	else
	{
		fseek (pFile , 0 , SEEK_END);
		lSize = ftell (pFile);
		rewind (pFile);
		buffer = (char*) malloc (sizeof(char)*lSize);
		result = fread (buffer,sizeof(unsigned char),lSize,pFile);
	}
	fclose(pFile);
	return buffer;
}

double* tcpreadTextFile(char* fileName) {
	double* returnArray=new double[307200];
	ifstream in("/home/sklee25/Desktop/string.txt");
		 if(!in){
				cout << "Cannot open file.";
				exit (1);
		 }
		 char str[255];
		 int counter=0;
		 while(in){
				in.getline(str, 255);      // Delimiter defaults to newline
				returnArray[counter]=atof(str);
//	      cout << returnArray[counter] << "  "<<counter<<endl;
				counter++;
		 }
		 in.close();
		 return returnArray;
}


void TCPClientSocket::SendData(char* sendBuffer, int sizeOfBuffer) {
	int dataSend=send(sock,sendBuffer,sizeOfBuffer,0);
	printf("TCPClientSocket::SendData() total data transmitted %d  %d\n",dataSend);
	close(sock);
}

void TCPClientSocket::EndSendData()
{
	char endBuffer[3];
	strcpy(endBuffer,"end");
	int dataSend=send(sock,endBuffer,3,0);
	printf("TCPClientSocket::EndSendData() total data transmitted\n");
}


void TCPClientSocket::EstablishConnection(char* serverIPAddress, int portNum) {
	host = gethostbyname(serverIPAddress);
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNum);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8);
	if (connect(sock, (struct sockaddr *)&server_addr,
			sizeof(struct sockaddr)) == -1)
	{
		perror("Connect");
		exit(1);
	}
}

void TCPClientSocket::CloseConnection() {
	close(sock);
}

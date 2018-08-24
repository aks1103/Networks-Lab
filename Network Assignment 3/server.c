#include<arpa/inet.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include <time.h>
#include "color.h"


#define MAX_LEN 1024
#define MAXPENDING 50

struct msg{
	int type; 
	int length;
	char message[MAX_LEN];
};

void DieWithError(char* error){
	perror(error);
	exit(1);
}


 
void delay(int number_of_seconds)
{
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}

int main(int argc, char const *argv[])
{
	
	// USE FORMAT : ./filename serverport
	char *filename = argv[0];
	int echoServPort = atoi(argv[1]);
	int servSock;
	struct msg msg;
	int childpid;

	struct sockaddr_in echoClntAddr;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in echoTempAddr;

	// creating server socket




		if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
			DieWithError("socket() failed");
		

		// defining the port, family used,  
		echoServAddr.sin_family = AF_INET; /* Internet address family */
		echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		echoServAddr.sin_port = htons(echoServPort);
		

		int attempt = 100;
		while((bind(servSock , (struct sockaddr *) &echoServAddr , sizeof(echoServAddr)) < 0) && attempt){
				printf("attempt : %d bind() failed\n",attempt);
				attempt--;
				delay(1000);	
			}
		if(attempt==0) DieWithError("bind failed");


		if (listen (servSock , MAXPENDING) < 0)
			DieWithError("listen() failed");
		printf("Server is up. Waiting for client...\n");

		int clntSock;
		int recvMsgSize;
		int RCVBUFSIZE = MAX_LEN;
		char echoBuffer[MAX_LEN];
		char str[INET_ADDRSTRLEN];
		
	while(1)
	{
		int clntLen = sizeof(echoClntAddr); 
		if((clntSock =accept(servSock,(struct sockaddr *)&echoClntAddr,&clntLen))<0)
			DieWithError("accept() failed");
		
		struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&echoClntAddr;
		struct in_addr ipAddr = pV4Addr->sin_addr;
		inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN );	
		printf("%sGot New Connection from %s\n", KGRN, str);


		if ( (childpid = fork ()) == 0 )  //Child process
        {

		close(servSock);

		if((recvMsgSize = recv(clntSock, &msg, sizeof(msg), 0)) < 0)
			DieWithError("recv() failed");
		
		printf("%sType1:\t", KRED);
		printf("%sMessage recieved from client %d\t%d\n",KWHT,msg.type,msg.length);

		echoServAddr.sin_family = AF_INET;
		echoServAddr.sin_port = htons(0);
		echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		
		if((servSock = socket (AF_INET, SOCK_DGRAM, 0)) <0)
		    DieWithError("socket() failed while creating UDP socket");

	    if(bind (servSock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr))<0)
		    DieWithError("bind() failed attaching to UDP port\n");

		socklen_t adddr_len = sizeof echoTempAddr;
	    getsockname(servSock, (struct sockaddr*)&echoTempAddr,&adddr_len);


	    msg.type = 2; 
	    int udp_port = echoTempAddr.sin_port;
	    msg.length = (int)((ceil(log10(udp_port))+1)*sizeof(char));
	    char str[msg.length];
		sprintf(msg.message, "%d", udp_port);
	    msg.message[msg.length] = '\0';

	    if(send(clntSock, &msg, sizeof(msg), 0)!=sizeof(msg))
	    	DieWithError("send() couldn't send all the bytes");
		printf("%sType2:\t", KRED);
		printf("%sUDP port send to the client for further connection\n",KWHT );

	    if(recvfrom(servSock,&msg,sizeof(msg),0,(struct sockaddr *)&echoClntAddr, &clntLen)!=sizeof(msg))
	    	DieWithError("recvfrom() failed");

		printf("%sType3:\t", KRED);
	    printf("%sRecieved msg is %d\t%d\t%s\n",KWHT,msg.type,msg.length,msg.message);


	    msg.type=4;
	    msg.length=0;
	    msg.message[0]='\0';
	    if(sendto(servSock, &msg, sizeof(msg), 0,(struct sockaddr *)&echoClntAddr, clntLen)!=sizeof(msg))
	    	DieWithError("sendto() failed");
		printf("%sType4:\t", KRED);
		printf("%sSending Acknowledgment to client\n",KWHT);

		printf("%sConnection closed..\n\n", KGRN);
	    return 0;
	}
	    close(clntSock);
		fflush(stdout);


	}
	close(servSock);
	return 0;
}




#include<arpa/inet.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "color.h"


#define MAX_LEN 1024

struct msg
{
	int type; 
	int length;
	char message[MAX_LEN];
};

void DieWithError(char* error){
	perror(error);
	exit(1);
}

int main(int argc, char const *argv[])
{
	int clientSock;

	// USE FORMAT : ./filename serverip serverport
	if ((clientSock = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		DieWithError("socket() failed");
	

	char* echoservIP = argv[1];
	char* echoServPort = atoi(argv[2]);
	
	struct sockaddr_in echoServAddr;
	struct sockaddr_in echoClntAddr;

	
	echoServAddr.sin_family = AF_INET; /* Internet address family */ 
	echoServAddr.sin_addr.s_addr = inet_addr(echoservIP);
	echoServAddr.sin_port = htons(echoServPort);


	if (connect (clientSock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr)) < 0)
		DieWithError("connect() failed");
	

	

	//sending type 1 message asking for the UDP port to connect next
	
	struct msg msg;
	msg.type = 1;
	msg.length = 0;

	// send and checking weather msg have been properly send or not
	int datalen = sizeof(msg);
	if(send(clientSock , &msg, datalen, 0) != datalen)
		DieWithError("send() could not send entire data");
	
	printf("%sType1:\t", KRED);
	printf("%sTCP request(Type 1 message) successfully send to server\n",KWHT);
	

	// recieving Type 2 message
	if (recv(clientSock, &msg, sizeof(msg),0) == 0)
		DieWithError("Server Terminated: recv() failed");
	
	printf("%sType2:\t",KRED);
	printf("%sServer responded with(Type 2 message) : %d\t%d\t%s\n",KWHT,msg.type,msg.length,msg.message);


	// close the TCP connection (iska kaam hogya)
	close(clientSock);




	// creating UDP socket based on the udp port recieved
	int udp_port = atoi(msg.message);
	if ((clientSock = socket (AF_INET, SOCK_DGRAM, 0)) <0)
		DieWithError("socket() failed while creating UDP server");

	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_port = udp_port;
	echoServAddr.sin_addr.s_addr = inet_addr(echoservIP);

    msg.type = 3;
    printf("%sEnter a message to send : ",KYEL);
    fgets(msg.message,sizeof(msg.message),stdin);
    msg.length = strlen(msg.message);



    // sending type 3 message
    if(sendto(clientSock, &msg, sizeof(msg), 0,(struct sockaddr *)&echoServAddr, sizeof(echoServAddr))!=sizeof(msg))
    	DieWithError("send() failed while sending type 3 message");
    
    printf("%sType3:\t",KRED);
    printf("%sTCP request(Type 3 message) successfully send to server\n",KWHT);


    // recieving type 4 message
    if(recvfrom(clientSock,&msg,sizeof(msg),0,(struct sockaddr *)&echoServAddr, sizeof(echoServAddr))==0)
    	DieWithError("recv failed while recieving type 4 message");

    printf("%sType4:\t",KRED);
    printf("%sReceived data(Type 4 message) from the server %d\t %d\t %s\n",KWHT,msg.type,msg.length,msg.message);

    fflush(stdout);
    close(clientSock);
	return 0;
}
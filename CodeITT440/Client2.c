//include libraries that is going to be used in this program
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>
#include <netdb.h>

int main (int argc, char *argv[])
{
	//variable declaration
	int client,server,client2;
	struct sockaddr_in nCat_client,remote_server,remote_client;
	char output[1024];
	char revbuf[1024];
	unsigned int sockaddr_len = sizeof(struct sockaddr_in);
	
	//create a socket as a server
	if((server = socket(AF_INET,SOCK_STREAM,0))==-1)
	{perror("socket creation failed:");exit(-1); }			
	
	//assigning socket's type,port number and ip address
	memset(&remote_server,0,sockaddr_len);
	remote_server.sin_family=AF_INET;
	remote_server.sin_port=htons(5001);
	remote_server.sin_addr.s_addr=inet_addr("192.168.56.102");
	bzero(&remote_server.sin_zero,8);

	//bind to a specific port
	if((bind(server,(struct sockaddr *)&remote_server,sockaddr_len))==-1)
	{perror("Bind:");exit(-1);}
	
	//listen for an attempt for connect from remote client
	if((listen(server,2))==-1)
	{ perror("LISTEN:");exit(-1);}
	int con;
	
	while (1){
	con = accept(server,(struct sockaddr *)&remote_client,&sockaddr_len);
	
	if (con<0)
	{perror("accept:");exit(-1);}
	else if(con>0)
	{printf("SUCCESS\n");}
	
	client = socket(AF_INET,SOCK_STREAM,0);
	//checking if socket creation failed
	if(client==-1)
	{
		perror("socket creation failed :");
		exit(-1);
	}
	
	nCat_client.sin_family =AF_INET;
	nCat_client.sin_port=htons(5000);
	nCat_client.sin_addr.s_addr=inet_addr("192.168.56.1");
	bzero(&nCat_client.sin_zero,8);
	
	//connect and error checking if connect failed
	
	if(connect(client,(struct sockaddr *)&nCat_client,sizeof(struct sockaddr_in))==-1)
	{
		perror("Connect Failed :");
		exit(-1);
	}
	
	//receiving file from Ncat remote server
	
	printf("[machine 1] Receiving from 192.168.56.1....\n");
	char *fr_name ="receivedNcat.txt";
	FILE *fr = fopen(fr_name,"abc");
	
	if(fr==NULL)
	{ printf("FILE %s Cannot be opened. \n" ,fr_name); }
	else
	{
		bzero(revbuf,1024);
		int fr_block_sz = 0;
		while(( fr_block_sz=recv(client,revbuf,1024,0)) > 0)
		{
			int write_sz = fwrite(revbuf,sizeof(char),fr_block_sz,fr);
			if(write_sz < fr_block_sz)
			{
				perror("File Write Failed \n");
			}
			
			bzero(revbuf,1024);
			if(fr_block_sz == 0|| fr_block_sz != 1024)
			{break;}
			
			if (fr_block_sz < 0)
			{
				if(errno == EAGAIN)
				{
					printf("recv() timed out. \n");
				}
				else
				{
					fprintf(stderr, "recv() failed due to errno = %d\n", errno);
				}
			}
		}
		printf("File have been read from Ncat Server\n");
		fclose(fr);
		printf("Attempting to send File to Client\n");
		
		//sending file to remote client
		
		if(!fork())
		{
			char *fs_name ="receivedNcat.txt";
			char sndbuf[1024];
			printf("Sending to client...\n");
			FILE *fs = fopen(fs_name,"r");
			if(fs==NULL)
			{
				printf("ERROR. FILE NOT FOUND");
				exit(1);
			}
			bzero(sndbuf,1024);
			int fs_block_sz;
			while((fs_block_sz = fread(sndbuf,sizeof(char),1024,fs))>0)
			{
				if(send(con,sndbuf,fs_block_sz,0)<0)
				{
					fprintf(stderr, "ERROR: FAILED TO SEND FILE %s (errno = %d)\n",fs_name,errno);
					break;
				}
				bzero(sndbuf,1024);
			}
			printf("FILE SUCCESFULLY TRANSFERED \n");
			close(con);
		}
	}
	}
	}

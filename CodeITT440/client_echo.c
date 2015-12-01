#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/wait.h>
#include<ctype.h>
#include<netdb.h>
int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in remote_server;
	char input[1024];
	char output[1024];
	int len;
	char revbuf[1024];
	
	sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock==-1)
	{perror("socket:");exit(-1);}
	
	remote_server.sin_family=AF_INET;
	remote_server.sin_port=htons(atoi(argv[2]));
	remote_server.sin_addr.s_addr=inet_addr(argv[1]);
	bzero(&remote_server.sin_zero,8);
	
	if(connect(sock,(struct sockaddr *)&remote_server,sizeof(struct sockaddr_in))==-1)
	{perror("connect:");exit(-1);}
	
	//getting file from the server
	printf("Receiving file from Server.....");
	char *fr_name ="receivedClient.txt";
	FILE *fr = fopen(fr_name, "abc");
	if(fr == NULL)
	{
		printf("File Cannot be Opened\n");
	}
	else
	{
		bzero(revbuf,1024);
		int fr_block_sz =0;
		
		while((fr_block_sz = recv (sock, revbuf, 1024,0)) > 0)
		{
			int write_sz = fwrite(revbuf, sizeof(char),fr_block_sz, fr);
			if (write_sz <fr_block_sz)
			{
				perror("File write failed.\n");
			}
			bzero(revbuf,1024);
			if (fr_block_sz <0 )
		{
			if(errno==EAGAIN)
			{
				printf("recv() timed out\n");
			}
			else
			{
				fprintf(stderr, "recv() dailed due to errno = %d",errno);
			}
		}
		printf("OK received from server\n");
		fclose(fr);
		
	close(sock);
}
}
}

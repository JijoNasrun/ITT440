#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in remote_server;
	char input[1024];
	char output[1024];
	int len;
	
	sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock==-1)
	{perror("socket:");exit(-1);}
	
	remote_server.sin_family=AF_INET;
	remote_server.sin_port=htons(atoi(argv[2]));
	remote_server.sin_addr.s_addr=inet_addr(argv[1]);
	bzero(&remote_server.sin_zero,8);
	
	if(connect(sock,(struct sockaddr *)&remote_server,sizeof(struct sockaddr_in))==-1)
	{perror("connect:");exit(-1);}
	
	while(1)
	{
		fgets(input,1024,stdin);
		send(sock,input,strlen(input),0);
	
		len=recv(sock,output,1024,0);
		output[len]='\0';
		printf("%s\n",output);

	}
	close(sock);
}

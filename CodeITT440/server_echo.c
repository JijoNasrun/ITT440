#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc,char *argv[])
{
	struct sockaddr_in server,client;
	int sock,new;
	int sockaddr_len=sizeof(struct sockaddr_in);
	int data_len;
	char data[1024];
	
	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
	{perror("socket creation:");exit(-1);}
	
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[1]));
	server.sin_addr.s_addr=INADDR_ANY;
	bzero(&server.sin_zero,8);
	
	if((bind(sock,(struct sockaddr *)&server,sockaddr_len))==-1)
	{perror("Bind:");exit(-1);}
	
	if((listen(sock,2))==-1)
	{ perror("Listen:");exit(-1);}
	
}


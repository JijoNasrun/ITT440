#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	struct sockaddr_in server,client;
	int sock,new;
	unsigned int sockaddr_len=sizeof(struct sockaddr_in);
	int data_len;
	char data[1024];
	int ret,on;
	on=1;

	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
	{perror("socket creation:");exit(-1);}//PITFALL 1
	
	ret=setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));//PITFALL3	
	
	memset(&server,0,sockaddr_len);
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[1]));//pitfall 4
	server.sin_addr.s_addr=INADDR_ANY;
	bzero(&server.sin_zero,8);
	
	if((bind(sock,(struct sockaddr *)&server,sockaddr_len))==-1)
	{perror("Bind:");exit(-1);}
	
	if((listen(sock,2))==-1)
	{ perror("Listen:");exit(-1);}
	
	
	while(1)
	{	new = accept(sock,(struct sockddr *)&client,&sockaddr_len);
		if(new<0)
		{perror("accept:");exit(-1);}
		else if(new>0)
		{printf("SUCCESS:\n");}
	
		
		printf("NEW CLIENT CONNECTED FROM PORT NO %d AND IP %s\n",ntohs(client.sin_port),inet_ntoa(client.sin_addr));
		
		data_len=1;
		
		while(data_len)
		{
			data_len=recv(new,data,1024,0);
			if(data_len>0)
			{printf("Read Success.\n");}
			else if((data<0)||(data==0))//pitfall 1
			{printf("Peer Socket is closed. Error");exit(-1);}
			
			if(data_len)
			{
				send(new,data,data_len,0);
				data[data_len]='\0';
				printf("Sent Message:%s",data);
			}
			
		}	
		printf("CLIENT DISCONNECTED\n");
		close(new);
	}
	
	
}


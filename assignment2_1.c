#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
int main()
{
	int sock,val,a;
	struct sockaddr_in socket1;

	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
	{	perror("socket malfunction:"); exit(-1);	}
	
	socket1.sin_family=AF_INET;
	socket1.sin_port=htons(1040);
	socket1.sin_addr.s_addr=INADDR_ANY;

	int len=sizeof(struct sockaddr_in);
	
	if(bind(sock,(struct sockaddr *)&socket1,len)==-1)
	{	perror("bind malfunction:"); exit(-1);	}
	
	//setting the socket's send buffer and receiving buffer//
	val=32768;
	a = setsockopt( sock,SOL_SOCKET,SO_SNDBUF,(void *)& val,sizeof(val));
	
	a = setsockopt( sock,SOL_SOCKET,SO_RCVBUF,(void *)& val,sizeof(val));
	struct timeval time;
	socklen_t len2; 
	a = getsockopt(sock ,SOL_SOCKET,SO_SNDTIMEO,(void *)& time,&len2);

}

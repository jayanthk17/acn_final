#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

int main()
{
    int welcome; //new_soc, fd, n;
    char buffer[1024], msg[100];
    struct  sockaddr_in addr; //

    welcome = socket(PF_INET, SOCK_DGRAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(7891);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(welcome, (struct sockaddr *) &addr, sizeof(addr));
    printf("\nServer is Online");
    recvfrom(welcome, msg,sizeof(msg), 0,(struct sockaddr*)&addr, sizeof(addr));
    printf("Message: %s",msg);
	    
//	    sendto(welcome,msg, sizeof(msg),0,(struct sockaddr*)&addr, sizeof(addr));
   
   
   
    close(welcome);

    return 0;
}

#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int soc; // n;
    char buffer[1024], msg[100];
    struct sockaddr_in addr; //

    soc = socket(PF_INET, SOCK_DGRAM, 0);
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7891);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
   
    printf("\nClient is connected to Server");
    printf("\nEnter message: ");
    scanf("%s",  msg);

    sendto(soc, msg, sizeof(msg), 0,(struct sockaddr*)&addr, sizeof(addr));
    close(soc);
    return 0;
}

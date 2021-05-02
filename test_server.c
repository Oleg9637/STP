// Server program
#include <errno.h>
#include <netinet/in.h>
//#include <signal.h>
#include <stdio.h>
//#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
//#include <unistd.h>
#define PORT 5000
#define MAXLINE 1024

int main()
{
    int listenfd, connfd, nready, maxfdp1;
    char buffer[MAXLINE];
    pid_t childpid;
    fd_set rset;
    ssize_t n;
    socklen_t len;
    const int on = 1;
    struct sockaddr_in cliaddr, servaddr;
    char* message = "Hello Client";
    void sig_chld(int);
  
    /* create listening UDP socket */
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
  
    // binding server addr structure to listenfd
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

  
    // clear the descriptor set
    FD_ZERO(&rset);
  
    // get maxfd
    maxfdp1 = listenfd + 1;
    for (;;) {
  
        // set listenfd and udpfd in readset
        FD_SET(listenfd, &rset);
  
        // select the ready descriptor
        nready = select(maxfdp1, &rset, NULL, NULL, NULL);
  
        // if listen socket is readable receive the message.
        if (FD_ISSET(listenfd, &rset)) {
            len = sizeof(cliaddr);
            bzero(buffer, sizeof(buffer));
            printf("\nMessage from UDP client: ");
            n = recvfrom(listenfd, buffer, sizeof(buffer), 0,
                         (struct sockaddr*)&cliaddr, &len);
            puts(buffer);
            sendto(listenfd, (const char*)message, sizeof(buffer), 0,
                   (struct sockaddr*)&cliaddr, sizeof(cliaddr));
        }
    }
}

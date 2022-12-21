#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024

void write_file(int sockfd)
{
    int n;
    char buffer[SIZE];

    while(1)
    {
        n=recv(sockfd, buffer, SIZE, 0);
        
        if(n<=0)
        {
            return;
        }

        printf("%s", buffer);
        bzero(buffer, SIZE);
    }
    return;
}

int main()
{
    char *ip = "127.0.0.1";
    int port =8082;
    int e;

    int sockfd, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0)
    {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if(e<0)
    {
        perror("[-]Error in bind");
        exit(1);
    }
    printf("[+]binding successful.\n");

    if(listen(sockfd, 10)==0)
    {
        printf("[+]Listening....\n");
    }
    else
    {
        perror("[-]Error in listening");
        exit(1);
    }

    addr_size=sizeof(new_addr);
    new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
    write_file(new_sock);
    printf("[+]Data wrriten successfully.\n");

    // char pass[]="110100000001100111100110";
    // if(send(new_sock, pass, sizeof(pass), 0)==-1)
    // {
    //     perror("error in sending authentication");
    // }


    return 0;
}   
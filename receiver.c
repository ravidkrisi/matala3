#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>


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

    //set var to start and stop time. record time for start time
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    int n;

    //receive part 1 of the file
    write_file(new_sock);
    printf("[+]Data wrriten successfully.\n");

    //record time for stop time 
    gettimeofday(&stop, NULL);

    //print message of the time it took to receive the first file
    printf("took %lu us\n", (stop.tv_sec - start.tv_sec) * 10000 + stop.tv_usec - start.tv_usec);
    
    //send authentication
    char pass[]="110100000001100111100110";
    if(send(new_sock, pass, sizeof(pass), 0)==-1)
    {
        perror("error in sending authentication");
    }

    //record time of start time for sencond part of the file 
    gettimeofday(&start, NULL);

    //receiving the sencond part of the file 
    write_file(new_sock);


    //record stop time of the sencond part of the file 
    gettimeofday(&stop, NULL);

    //print the time it took to receive the second part of the file
    printf("took %lu us\n", (stop.tv_sec - start.tv_sec) * 10000 + stop.tv_usec - start.tv_usec);

    return 0;
}   
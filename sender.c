
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#define SIZE 1024

//func gets a file and socket and sends it to the receiver
void send_file(FILE *fp, int sockfd)
{
    char data[SIZE] = {0};

    while(fgets(data, SIZE,fp)!=NULL)
    {
        if(send(sockfd, data, sizeof(data), 0) ==-1)
        {
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

int main()
{
    char *ip= "127.0.0.1";
    int port =8082;
    int e;

    int sockfd;
    struct sockaddr_in server_addr;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(e == -1)
    {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Connected to Server.\n");

    //read the file
    FILE *file;
    file = fopen("ex3.txt", "r");
    if(file == NULL)
    {
        printf("couldnt load the file");
    }

    //split the file into 2 equal size files 
    fseek(file,0,SEEK_END);
    int length = ftell(file);
    fseek(file,0,SEEK_SET);

    int length1 = length/2;
    int length2 = length-length1;

    FILE *part1, *part2;
    part1 = fopen("part1.txt", "w+");
    part2 = fopen("part2.txt", "w+");

    char c;
    int i =0;
    while((c=fgetc(file))!=EOF)
    {
        if(i<=length1)
        {
            fprintf(part1,"%c", c);
        }
        else
        {
            fprintf(part2,"%c", c);
        }
        i++;
    }
   
   fseek(part1,0,SEEK_SET);
   fseek(part2,0,SEEK_SET);

    //set the cc algorithm to CUBIC 
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, "cubic", 5) < 0)
    {
      printf("set socket error from client\n");
    }

    //send the first part of the file 
    send_file(part1, sockfd);
    printf("[+]File data sent successfully.\n");

    ////receive the authentatication from the receiver
    int n;
    char buffer[SIZE];
    while(1)
    {
        n=recv(sockfd, buffer, SIZE, 0);
        
        if(n<=0)
        {
            break;
        }

        printf("%s", buffer);
        bzero(buffer, SIZE);
    }

    // set the cc algorithm to RENO 
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, "reno", 4) < 0)
        {
            printf("set socket error from client\n");
        }

    printf("[+]Closing the connection.\n");
    close(sockfd);
    
    return 0;
}
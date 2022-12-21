#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 2120000

//send fucntion gets file and send it to the reciever
void send_file(char arr[SIZE], int sockfd){
int n;
char data[SIZE] = {0};
// while(fgets(data, SIZE, fp) != NULL) {
if (send(sockfd, arr, SIZE/2, 0) == -1) 
{
    perror("[-]Error in sending file.");
    exit(1);
}
bzero(data, SIZE);
}


int main(){
char *ip = "127.0.0.1";
int port = 8082;
int e;


int sockfd;
struct sockaddr_in server_addr;

sockfd = socket(AF_INET, SOCK_STREAM, 0);
if(sockfd < 0) {
perror("[-]Error in socket");
exit(1);
}
printf("[+]Server socket created successfully.\n");

server_addr.sin_family = AF_INET;
server_addr.sin_port = port;
server_addr.sin_addr.s_addr = inet_addr(ip);

//split the file into 2 equal files 
    FILE *file;
    file = fopen("ex3.txt", "r");
    if(file == NULL)
    {
        printf("couldnt load the file");
    }
     
    fseek(file,0,SEEK_END);
    int length = ftell(file);
    fseek(file,0,SEEK_SET);

    int length1 = length/2;
    int length2 = length-length1;

    char arr[SIZE] = {'\0'};

    char c;
    int i =0;
    while((c=fgetc(file))!=EOF)
    {
        arr[i]=c;
        i++;
    }
   printf("%s", arr);
//create a connection with the receiver
e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
if(e == -1) {
perror("[-]Error in socket");
exit(1);
}
printf("[+]Connected to Server.\n");


//send the first part of the file
send_file(arr, sockfd);
printf("[+]File1 data sent successfully.\n");

// //receive authentication from the receiver
// char buffer[SIZE]={'\0'};
// int n=recv(sockfd, buffer, SIZE, 0);
// if(n<=0)
// {
//     printf("[+]failed to get authentication");
//     exit(1);
// }
// printf("%s", buffer);


//send the second part of the file 
send_file(arr+length1, sockfd);
printf("[+]File2 data sent successfully.\n");

printf("[+]Closing the connection.\n");
close(sockfd);

return 0;
}
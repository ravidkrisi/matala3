#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#define AUTH_SIZE 25
#define SIZE 2120000
#define HALF_SIZE 1060001

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

//read the file  
FILE *file;
file = fopen("ex3.txt", "r");
if(file == NULL)
{
    printf("couldnt load the file");
}
//count the number of char in the file   
fseek(file,0,SEEK_END);
int length = ftell(file);
fseek(file,0,SEEK_SET);

//sets the length and the arrays of the 2 parts of the file
int length1 = length/2;
int length2 = length-length1;


char arr1[HALF_SIZE] = {'\0'};
char arr2[HALF_SIZE] = {'\0'};


//fill the the arrays of the two parts of the file
char c;
int i =0;
int j=0;
while((c=fgetc(file))!=EOF)
{
    if(i<length1)
    {
        arr1[i]=c;
        i++;
    }
    else
    {
        arr2[j]=c;
        j++;
    }
}

//create a connection with the receiver
e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
if(e == -1) {
perror("[-]Error in socket");
exit(1);
}
printf("[+]Connected to Server.\n");

char user_input='z';
int times_run=0;
do{
//set the cc algorithm to CUBIC if times_run is even else to RENO
if(times_run%2==0)
{
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, "cubic", 5) < 0)    
    {
    printf("\nset socket error from client\n");
    }
    else
    {
    printf("\nset CC algorithm to CUBIC\n");
    }
}
else
{
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, "reno", 4) < 0)
    {
    printf("\nset socket error from client\n");
    }
    else
    {
    printf("\nset CC algorithm to RENO\n");
    }
}

//send the first part of the file
// send_file(arr, sockfd);
send(sockfd, arr1, HALF_SIZE, 0);
printf("[+]File1 data sent successfully.\n");

//receive authentication from the receiver
char message[]= "110100000001100111100110";
char buffer[SIZE]={'\0'};
int n=0;
while(1)
{
    int n=recv(sockfd, buffer, SIZE, 0);
    if(n>=0)
    {
        break;
    }
}
if(strcmp(message, buffer)!=0)
{
    printf("\nauthentication is not matched\n");
    exit(1);
}
printf("\nreceived auth: %s\n", buffer);

// set the cc algorithm to CUBIC if times_run is even else to RENO
if(times_run%2!=0)
{
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, "cubic", 5) < 0)    
    {
    printf("\nset socket error from client\n");
    }
    else
    {
    printf("\nset CC algorithm to CUBIC\n");
    }
}
else
{
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, "reno", 4) < 0)
    {
    printf("\nset socket error from client\n");
    }
    else
    {
    printf("\nset CC algorithm to RENO\n");
    }
}

//send the second part of the file 
send(sockfd, arr2, HALF_SIZE, 0);
printf("[+]File2 data sent successfully.\n");

//ask the user for input wheater he want to send the message again
printf("if you want to run the sender again press 'Y' to close press any key\n");
scanf(" %c", &user_input);
times_run++;
}
while(user_input=='Y');

//closing the connection with receiver 
printf("[+]Closing the connection.\n");
close(sockfd);



return 0;
}
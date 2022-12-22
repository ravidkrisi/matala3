#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include<unistd.h>
#include <netinet/tcp.h>


#define AUTH_SIZE 25
#define SIZE 2120000
#define HALF_SIZE 1060001


//a func that gets a the clientsocket and prints the file it gets from the sender and stops when the receiver gets the full message it suppused to get
void write_file(int sockfd)
{
int sum=0;
int n;
char buffer[SIZE];
while(1)
{
    n = recv(sockfd, buffer, SIZE, 0);
    bzero(buffer, SIZE);
    sum=sum+n;
    if(sum==HALF_SIZE)
    {
        break;
    }
}
printf("\nreceived %d bytes from the sender\n", sum);
}

int main(){


char *ip = "127.0.0.1";
int port = 8082;
int e;

int sockfd, new_sock;
struct sockaddr_in server_addr, new_addr;
socklen_t addr_size;
char buffer[SIZE];

sockfd = socket(AF_INET, SOCK_STREAM, 0);
if(sockfd < 0) {
perror("[-]Error in socket");
exit(1);
}
printf("[+]Server socket created successfully.\n");

server_addr.sin_family = AF_INET;
server_addr.sin_port = port;
server_addr.sin_addr.s_addr = inet_addr(ip);

e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
if(e < 0) {
perror("[-]Error in bind");
exit(1);
}
printf("[+]Binding successfull.\n");

if(listen(sockfd, 10) == 0){
printf("[+]Listening....\n");
}else{
perror("[-]Error in listening");
exit(1);
}

addr_size = sizeof(new_addr);
new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);


char user_input = 'Z';
int times_run=0;
do{

// set vars for time.
struct timeval stop, start;
gettimeofday(&start, NULL);

//gets the first part of the file 
write_file(new_sock);

//record stop time part1
gettimeofday(&stop, NULL);


//print the time it took to receive part1
double t = (stop.tv_sec - start.tv_sec)*1000 + ((double)stop.tv_usec - start.tv_usec)/1000;
printf("part 1 took %f ms\n", t);
printf("[+]received data successfully.\n");


// set the cc algorithm to CUBIC if times_run is even else to RENO
if(times_run%2!=0)
{
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, "cubic", 5) < 0)    
    {
    printf("set socket error from client\n");
    }
    else
    {
    printf("set CC algorithm to CUBIC\n");
    }
}
else
{
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, "reno", 4) < 0)
    {
    printf("set socket error from client\n");
    }
    else
    {
    printf("set CC algorithm to RENO\n");
    }
}
//set authentication in a string 
char message[] = "110100000001100111100110";

//send the authentication to the server
send(new_sock, message, AUTH_SIZE, 0);

//record start time part2
gettimeofday(&start, NULL);

//gets the second part of the file
write_file(new_sock);


//record stop time part2
gettimeofday(&stop, NULL);


//print time it took to receive part2
t = (stop.tv_sec - start.tv_sec)*1000 + ((double)stop.tv_usec - start.tv_usec)/1000;
printf("part 2 took %f ms\n", t);
printf("[+]received data successfully.\n");

//ask the user for input wheater he want to send the message again
printf("\nif you want to run the receiver again press 'Y' to close press any key\n");
scanf(" %c", &user_input);
times_run++;

}
while(user_input=='Y');

//closing the connection
close(new_sock);
close(sockfd);
printf("[+]Closing the connection.\n");


return 0;
}
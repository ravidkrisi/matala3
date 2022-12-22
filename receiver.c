#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include<unistd.h>

#define AUTH_SIZE 25
#define SIZE 2120000
#define HALF_SIZE 1060001


//a func that gets a the clientsocket and prints the file it gets from the sender and stops when the receiver gets the full message it suppused to get
void write_file(int sockfd)
{
int sum=0;
int n;
char buffer[SIZE];
while(sum<HALF_SIZE)
{
    n = recv(sockfd, buffer, SIZE, 0);
    printf("%s", buffer);
    bzero(buffer, SIZE);
    sum=sum+n;
}
printf("sum is:%d\n", sum);
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

// set vars for time. record time for start part1
struct timeval stop, start;
gettimeofday(&start, NULL);

//gets the first part of the file 
write_file(new_sock);
// char buffer1[HALF_SIZE]={'\0'};
// recv(new_sock, buffer1, HALF_SIZE, 0);


//record stop time part1
gettimeofday(&stop, NULL);
// printf("%s\n",buffer1);

//print the time it took to receive part1
printf("\ntook %lu ms\n", (stop.tv_sec - start.tv_sec) * 1000 + (stop.tv_usec - start.tv_usec)/1000);
printf("\n[+]Data1 written in the file successfully.\n");


//set authentication in a string 
char message[] = "110100000001100111100110";

//send the authentication to the server
send(new_sock, message, AUTH_SIZE, 0);

//record start time part2
gettimeofday(&start, NULL);

//gets the second part of the file
write_file(new_sock);
// char buffer2[HALF_SIZE]={'\0'};
// recv(new_sock, buffer2, HALF_SIZE, 0);

//record stop time part2
gettimeofday(&stop, NULL);
// printf("%s", buffer2);

//print time it took to receive part2
printf("\ntook %lu ms\n", (stop.tv_sec - start.tv_sec) * 1000 + (stop.tv_usec - start.tv_usec)/1000);
printf("[+]Data2 written in the file successfully.\n");

return 0;
}
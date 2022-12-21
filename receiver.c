#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#define SIZE 2120000

//a func that gets a the clientsocket and prints the file it gets from the sender
void write_file(int sockfd)
{
int n;
char buffer[SIZE];

n = recv(sockfd, buffer, SIZE, 0);
// printf("%d\n",n);
if (n <= 0)
{
    return;
}
printf("%s", buffer);
bzero(buffer, SIZE);
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

//record stop time part1
gettimeofday(&stop, NULL);

//print the time it took to receive part1
printf("\ntook %lu ms\n", (stop.tv_sec - start.tv_sec) * 10000 + (stop.tv_usec - start.tv_usec)/1000);
printf("\n[+]Data1 written in the file successfully.\n");


//set authentication in a string 
// char message[] = "110100000001100111100110";

//send the authentication to the server
// if (send(sockfd, message, sizeof(message), 0) == -1) 
// {
// perror("[-]Error in sending file.");
// exit(1);
// }

//record start time part2
gettimeofday(&start, NULL);

//gets the second part of the file
write_file(new_sock);

//record stop time part2
gettimeofday(&stop, NULL);

//print time it took to receive part2
printf("\ntook %lu ms\n", (stop.tv_sec - start.tv_sec) * 10000 + (stop.tv_usec - start.tv_usec)/1000);
printf("[+]Data2 written in the file successfully.\n");

return 0;
}
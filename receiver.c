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

double times_part1[5]={0};
double times_part2[5]={0};

//a func that print the times of every run and exit the prog the avg in every run and avg in all runs
void printtimes(){
    int i=0;
    double sum=0;
    int counter=0;
    double avg;
    while(times_part1[i]!=0)
    {
        printf("\nfor the %d run part 1 it took %f ms\n", i+1, times_part1[i]);
        printf("for the %d run part 2 it took %f ms\n", i+1, times_part2[i]);
        avg=(times_part1[i]+times_part2[i])/2;
        printf("the average of this run is: %f\n",avg);
        i++;
        counter=counter+2;
        sum=sum+times_part1[i]+times_part2[i];
    }
    printf("\n the averge of the total runs is: %f\n",sum/counter);
    exit(1);
}

//a func that gets a the clientsocket and prints the file it gets from the sender and stops when the receiver gets the full message it suppused to get
void write_file(int sockfd, struct timeval *start)
{
//intialize vars for string that end the prog flag to set the start time and sum to count the num of bytes it received
char end [50]={'*'};
int sum=0;
int n;
char buffer[SIZE];
int flag=0;
while(1)
{
    n = recv(sockfd, buffer, SIZE, 0);
    //record the start time
    if(n>0&&flag==0)
    {
    gettimeofday(start, NULL);
    flag++;
    }
    //check if we recevied the exit message from the sender
    if(strcmp(end, buffer)==0)
    {
        printtimes();
    }
    //zero the buffer for the next recv
    bzero(buffer, SIZE);
    sum=sum+n;
    //if we received the expected num of bytes break from the while
    if(sum==HALF_SIZE)
    {   
    break;
    }
}
//print how many bytes we received 
printf("\nreceived %d bytes from the sender\n", sum);
}



int main(){

//create receiver socket
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

// set vars for time and pointer to start
struct timeval stop, start;
struct timeval *s= &start;

//gets the first part of the file and record the start time
write_file(new_sock, s);

//record stop time part1
gettimeofday(&stop, NULL);


//save the time it took to receive part one of the current run 
double t = (stop.tv_sec - start.tv_sec)*1000 + ((double)stop.tv_usec - start.tv_usec)/1000;
times_part1[times_run]=t;



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


//gets the second part of the file and record the start time
write_file(new_sock, s);


//record stop time part2
gettimeofday(&stop, NULL);


//save the time it took te receive part 2 in the current run
t = (stop.tv_sec - start.tv_sec)*1000 + ((double)stop.tv_usec - start.tv_usec)/1000;
times_part2[times_run]=t;


times_run++;

}
//check there is still connection fron the sender
while(new_sock>0);

//print the times it took to receive every part in each run
printtimes();
return 0;
}
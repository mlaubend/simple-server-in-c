#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <assert.h>'
#include <pthread.h>

void* server(void*);

//variables to send to client
char BUFFER[1055];
time_t start_t, end_t;
int total_clients = 0;
double runTime;

//server variables
int sockfd = 0;
int connfd = 0;
struct sockaddr_in serv_addr;

int main(int argc, char *argv[])
{
    time(&start_t);  //start clock running

    //thread variables
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int thread;

    //create a socket using internet protocol(AF_INET) as opposed to Unix protocol (AF_UNIX)
    //parameter 2 defines the type of socket. SOCK_STREAM defines a stream socket
    //parameter 3 is protocol parameter.  kernel takes care of this
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //initializes BUFFER and serv_addr
    memset(BUFFER, '0', sizeof(BUFFER));
    memset(&serv_addr, '0', sizeof(serv_addr));

    //set the server address to internet protocol
    serv_addr.sin_family = AF_INET;

    //set the IP address the server will connect to
    //INADDR_ANY will connect to any IP address
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //assigns socket to port 5432
    serv_addr.sin_port = htons(5432);

    //binds sockfd to serv_addr
    //type casted for conversion from unix domain to internet domain
    bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    //listen() sets maximum number of clients to 10 and sets to lisen
    listen(sockfd, 10);

    while(1)
    {
        if (connfd = accept(sockfd, NULL, NULL)) //socket is woken up
        {
            thread = pthread_create(&tid, NULL, &server, NULL);
        }

        sleep(1); //socket goes back to sleep and waits for another accept() call
     }
}

void* server(void *arg)
{
    int count = 0;
    char integers[1055];
    int integer = 0;
    total_clients += 1;
    time(&end_t);
    runTime = difftime(end_t, start_t);

    //store the stuff I want to send to the client in BUFFER
    snprintf(BUFFER, sizeof(BUFFER), "\n Runtime: %f seconds\n Total Clients: %i \n\n", runTime, total_clients);


    write(connfd, BUFFER, strlen(BUFFER));//write to client


    while(count < 5)
    {

        fflush(stdout); //restart
        snprintf(BUFFER, sizeof(BUFFER), "enter an integer: \n");
        write(connfd, BUFFER, strlen(BUFFER));

        read(connfd, integers, 1055); //read input from client and store it in integers string array
        integer = integer + (*integers -'0'); //convert string to int and add

        fflush(stdout); //restart
        snprintf(BUFFER, sizeof(BUFFER), "total sum of integers: %i\n\n", integer);
        write(connfd, BUFFER, strlen(BUFFER));
        count += 1;
    }

    close(connfd);
    pthread_exit(NULL);
}




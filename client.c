// C program for the Client Side
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

// inet_addr
#include <arpa/inet.h>
#include <unistd.h>

// For threading, link with lpthread
#include <pthread.h>
#include <semaphore.h>

void* clienthread(void* args);

void* clienthread(void* args){

    int client_reqest = *((int*)args);
    int network_socket;

    //tworzenie socketu strumienia
    network_socket = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in servAddr;
    servAddr.sin_port = htons(9001);
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_family = AF_INET;

    //inicjuj poleczenie

    int connectionStatus = connect(network_socket,(struct sockaddr*)&servAddr, sizeof(servAddr));

    if(connectionStatus<0){
        printf("CONNECTION ERROR\n");
        return 0;
    }

    printf("CONNECTED!\n");
    send(network_socket,&client_reqest, sizeof(client_reqest),0);

    close(network_socket);
    pthread_exit(NULL);

    return 0;
}


int main() {

    printf("1. READ\n2.WRITE\n");

    int choice;

    if(scanf("%d",&choice)!=1){
        return 1;
    }

    pthread_t tid;

    switch (choice) {
        case 1:{
            int clientRequest = 1;

            //tworze thread
            pthread_create(&tid,NULL,clienthread,&clientRequest);
            sleep(20);
            break;
        }
        case 2:{
            int clientRequest = 2;

            //tworze thread
            pthread_create(&tid,NULL,clienthread,&clientRequest);
            sleep(20);
            break;
        }
        default:{
            printf("NIEPOPRWANY WYBOR\n");
            break;
        }
    }

    pthread_join(tid,NULL);
    return 0;
}

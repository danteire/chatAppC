// Cpp program for the Server Side


// For threading, link with lpthread
#ifdef _WIN32

#include <iostream>
#include <winsock2.h>
#include <windows.h>


#else

#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
// inet_addr
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#endif



// Semaphore variables

#ifdef _WIN32



#else

sem_t x, y;
pthread_t tid;
pthread_t writerthreads[100];
pthread_t readerthreads[100];

#endif




int readercount = 0;

void* reader(void* param);
void* writer(void* param);

#ifdef _WIN32

void* reader(void* param){

    return 0;
}
void* writer(void* param){

    return 0;
}
#else
void* reader(void* param){
    //blokowanie semaphora
    sem_wait(&x);
    readercount++;

    if(readercount==1){
        sem_wait(&y);
    }

    //odblokuj semaphor???
    sem_post(&x);

    printf("\n%d reader is inside",readercount);

    sleep(5);

    //znowu blokuj semaphor???

    sem_wait(&x);
    readercount--;

    sem_post(&x);

    printf("\n%d Reader is leaving",
           readercount + 1);
    pthread_exit(NULL);
}

void* writer(void* param){
    printf("\nProba dolacza Writer...");

    //sem Block
    sem_wait(&y);
    printf("\n Writer dolaczyl!");
    //sem Unblock
    sem_post(&y);

    printf("\n Writer wychodzi!");
    pthread_exit(NULL);
}

#endif


#ifdef _WIN32



#else



#endif

using namespace std;

int main(){

    // Initialize variables
    #ifdef _WIN32

    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequired = MAKEWORD(2,2);
    wsaerr = WSAStartup(wVersionRequired,&wsaData);

    if(wsaerr!= 0){
        cout<<"The Winsock dll not found!\n";
        return 1;
    }else{
        printf("The Winsock dll found\n");
        printf("Status: %s\n",wsaData.szSystemStatus);
    }

    // Create a socket

    SOCKET serverSocket;
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(serverSocket == INVALID_SOCKET){
        printf("ERROR Socket Creation!\n");
        WSACleanup();
        return 2;
    }else{
        printf("Is OK!\n");
    }

    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");  // Replace with your desired IP address
    service.sin_port = htons(55555);  // Choose a port number

    #else
        int serverSocket, newSocket;
        struct sockaddr_in serverAddr;
        struct sockaddr_storage serverStorage;

        socklen_t addr_size;
        sem_init(&x,0,1);
        sem_init(&y,0,1);

        serverSocket = socket(AF_INET,SOCK_STREAM,0);

        serverAddr.sin_addr.s_addr= INADDR_ANY;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(9001);
    #endif




    //bind socket do adresu i portu

    #ifdef _WIN32

    if(bind(serverSocket, reinterpret_cast<SOCKADDR*>((&service)), sizeof(service)) == SOCKET_ERROR){
        printf("Bind Failed: %d\n",WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 3;
    }else{
        printf("Bind Sucesfull!\n");
    }


    if(listen(serverSocket,1) == SOCKET_ERROR){
        cout<<"listen Unsecesfull: ERROR listening on socket: "<<WSAGetLastError()<<endl;
    }else{
        cout<<"Listen is Git:) Waiting for connections..\n";
    }

    //winapi listen but no threads yet :(

    SOCKET acceptSocket;
    acceptSocket = accept(serverSocket, nullptr, nullptr);

    if(acceptSocket == INVALID_SOCKET){
        cout<<"Accept Failed!\n"<<WSAGetLastError()<<endl;
        WSACleanup();
        return 4;
    }else{
        printf("Accept Ok!\n");
    }
    #else

    // Listen on the socket, with 40 max connection requests queued


    bind(serverSocket,(struct sockaddr*)&serverAddr, sizeof(serverAddr));

    // Listen on the socket, with 40 max connection requests queued
        if (listen(serverSocket, 50) == 0){
            printf("Listening\n");
        }else{
            printf("Error\n");
        }
    #endif

    //receive and send with winapi / no threads yet :(





    #ifdef _WIN32

        //get data
        char receiveBuffer[200];
        int rbytecount = recv(acceptSocket,receiveBuffer,200,0);
        if(rbytecount<0){
            printf("No Data to process Or Error: %d!\n",WSAGetLastError());
            return 5;
        }else{
            printf("Data Received: %s\n",receiveBuffer);
        }


        //send data

        printf("Enter message to send: ");

        char Buffer[200];
        char input;
        int maxMessSize = 200,index = 0;

        while ((input=getchar())!='\n'){
            if(index<maxMessSize){
                *(receiveBuffer+index) = input;
                index++;
            }
        }

        int sbyteCount = send(acceptSocket,Buffer,200,0);
        if(sbyteCount == SOCKET_ERROR){
            printf("No Data to process Or Error: %d!\n",WSAGetLastError());
            return 6;
        }else{
            printf("Server: Sent %d Bytes\n",sbyteCount);
        }

    return 0;

    #else

        pthread_t tid[60];

    #endif

    int i =0;
    while (1){
            #ifdef _WIN32



            #else

                    addr_size = sizeof(serverStorage);

                    // Extract the first connection in the queue
                    newSocket = accept(serverSocket,(struct sockaddr*)&serverStorage,&addr_size);

            #endif


        int choice = 0;
        #ifdef _WIN32



        #else

            recv(newSocket,&choice, sizeof(choice), 0);

        #endif


        if(choice==1){
            #ifdef _WIN32



            #else

                //nowe poloczenie, tworze thread
                if(pthread_create(&readerthreads[i++],NULL,reader,&newSocket)!=0){
                    // Error in creating thread
                    printf("Failed to create thread\n");
                }

            #endif

        }else if(choice==2){
            #ifdef _WIN32



            #else

                        if(pthread_create(&writerthreads[i++],NULL,writer,&newSocket)!=0){
                            // Error in creating thread
                            printf("Failed to create thread\n");
                        }

            #endif

        }

        if(i>=50){

            i=0;

            while (i<50){
                #ifdef _WIN32



                #else

                    pthread_join(writerthreads[i++],NULL);
                    pthread_join(readerthreads[i++],NULL);

                #endif

            }

            i=0;

        }

    }

    return 0;
}
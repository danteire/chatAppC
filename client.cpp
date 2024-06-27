// C program for the Client Side
//


#ifdef _WIN32

#include <iostream>
#include <winsock2.h>
#include <windows.h>

#else

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
#endif


using namespace std;

void* clienthread(void* args);

void* clienthread(void* args){

    int client_reqest = *((int*)args);
    int network_socket;

    //tworzenie socketu strumienia

    #ifdef _WIN32



    #else

    network_socket = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in servAddr;
    servAddr.sin_port = htons(9001);
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_family = AF_INET;
    #endif





    //inicjuj poleczenie
    #ifdef _WIN32



    #else

        int connectionStatus = connect(network_socket,(struct sockaddr*)&servAddr, sizeof(servAddr));

        if(connectionStatus<0){
            printf("CONNECTION ERROR\n");
            return 0;
        }
        printf("CONNECTED!\n");
    #endif







    #ifdef _WIN32



    #else
        send(network_socket,&client_reqest, sizeof(client_reqest),0);

        close(network_socket);
        pthread_exit(NULL);

    #endif


    return 0;
}


int main() {




    #ifdef _WIN32

    //inzjalizacja zmiennych WSA

    WSAData wsaData;
    int wserr;
    WORD mVersionRequired = MAKEWORD(2,2);
    wserr = WSAStartup(mVersionRequired,&wsaData);
    if (wserr != 0) {
        cout << "The winsock dll not found" << endl;
        return 1;
    } else {
        cout << "The Winsock dll found\n";
        cout << "The status: " << wsaData.szSystemStatus<<endl;
    }

    //Create Socket
    SOCKET clientSocket;
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    //CHECK FOR success

    if(clientSocket == INVALID_SOCKET) {
        cout<<"Socket Creation Error: "<<WSAGetLastError()<<endl;
        WSACleanup();
        return 2;
    }else {
        cout<<"Socket Creation Git:)\n";
    }

    // Connect to the server
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1");  // Replace with the server's IP address
    clientService.sin_port = htons(55555);  // Use the same port as the server

    if(connect(clientSocket,reinterpret_cast<SOCKADDR*>(&clientService), sizeof(clientService))==SOCKET_ERROR) {
        cout<<"Client Failed to connect: "<<WSAGetLastError()<<endl;
        WSACleanup();
        return 3;
    }else{
        cout<<"Client Connected!\n";
    }

    // Sending data to the server but no threads :(
    char buffer[200];
    std::cout << "Enter the message: ";
    std::cin.getline(buffer, 200);
    int sbyteCount = send(clientSocket, buffer, 200, 0);
    if (sbyteCount == SOCKET_ERROR) {
        std::cout << "Client send error: " << WSAGetLastError() << std::endl;
        return -1;
    } else {
        std::cout << "Client: Sent " << sbyteCount << " bytes" << std::endl;
    }

    // Receiving data from the server
    char receiveBuffer[200];
    int rbyteCount = recv(clientSocket, receiveBuffer, 200, 0);
    if (rbyteCount < 0) {
        std::cout << "Client recv error: " << WSAGetLastError() << std::endl;
        return 0;
    } else {
        std::cout << "Client: Received data: " << receiveBuffer << std::endl;
    }


    #else //TODO very much :(

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

    #endif

    return 0;
}

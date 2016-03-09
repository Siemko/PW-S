#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <string.h>
#include <WS2tcpip.h>
 
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")
int main()
{
        WSADATA wsaData;
 
        int result = WSAStartup( MAKEWORD( 2, 2 ), & wsaData );
        if( result != NO_ERROR )
        printf( "Initialization error.\n" );
 
        SOCKET mainSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
        if( mainSocket == INVALID_SOCKET )
        {
                printf( "Error creating socket: %ld\n", WSAGetLastError() );
                WSACleanup();
                return 1;
        }
 
        sockaddr_in service;
        memset( & service, 0, sizeof( service ) );
        service.sin_family = AF_INET;
        service.sin_addr.s_addr = inet_addr( "127.0.0.1" );
        service.sin_port = htons( 27015 );
 
        if( connect( mainSocket,( SOCKADDR * ) & service, sizeof( service ) ) == SOCKET_ERROR )
        {
                printf( "Failed to connect.\n" );
                WSACleanup();
                return 1;
        }
        /*
        //Pobieranie informacji o WinSock
         printf("Wersja WinSock: %d.%d\n",
        HIBYTE(wsaData.wHighVersion),
        LOBYTE(wsaData.wHighVersion));
    printf("Opis: %s\n", wsaData.szDescription);
    printf("Stan systemu: %s\n", wsaData.szSystemStatus);
    printf("Maksymalna liczba gniazd: %u\n",
        wsaData.iMaxSockets);
    printf("Maksymalny rozmiar datagramu UDP: %u\n",
        wsaData.iMaxUdpDg);
                */
        //Wysylanie danych
 
        int bytesSent;
        int bytesRecv = SOCKET_ERROR;
        //char sendbuf[ 32 ] = "Client says hello!";
        char recvbuf[ 32 ] = "";
 
        char sendbuf[32];
       
        do
        {
        printf("Podaj liczbe do wyslania: ");
        scanf("%s",&sendbuf);
 
        bytesSent = send( mainSocket, sendbuf, strlen( sendbuf ), 0 );
        printf( "Bytes sent: %ld\n", bytesSent );
 
        while( bytesRecv == SOCKET_ERROR )
        {
                bytesRecv = recv( mainSocket, recvbuf, 32, 0 );
   
                if( bytesRecv == 0 || bytesRecv == WSAECONNRESET )
                {
                        printf( "Connection closed.\n" );
                        break;
                }
        }
                if( bytesRecv < 0 )
                         return 1;
   
                printf( "Bytes received: %ld\n", bytesRecv );
                printf( "Received text: [%s]\n", recvbuf );
 
                //int getpeername( int sockfd, struct sockaddr * addr, int * addrlen );
 
                //int sd = 0; /* init to invalid socket */
                sockaddr_in sa = {0}; /* init to all zeros */
socklen_t sl = sizeof(sa);
if (getpeername(mainSocket, (sockaddr *) &sa,  &sl))
  perror("getpeername() failed");
else
        printf("peer is: %s:%hu\n", inet_ntoa(sa.sin_addr), ntohs(sa.sin_port));
 
        }while(sendbuf > 0);
        system( "pause" );
 
 
 
    WSACleanup();
 
        _getch();
}
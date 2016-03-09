//Klient
#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <string.h>
#include <locale.h>
 
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")

struct test{
	int t1;
	int t2;
	float t3;
	double t4;
};

 
int main()
{
        //setlocale(LC_CTYPE, ".1250");
        SetConsoleTitle(TEXT("Aplikacja klienta"));
 
        WSADATA ws;
        int result = WSAStartup(MAKEWORD(2,2), &ws);
        if(result != NO_ERROR)
        {
                printf("Blad funkcji WSAStartup().\n");
                _getch();
                return 0;
        }
        printf("Biblioteka WinSock zainicjalizowana poprawnie.\n\n");
 
        SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if(s == INVALID_SOCKET) printf("Wywolanie funkcji socket zakonczone niepowodzeniem. Kod bledu %d\n",
                WSAGetLastError());
        else
        {
                printf("Gniazdo utworzone poprawnie. \n");
                char buff[4096];
                while(true)
                {
                        sockaddr_in server;
                        server.sin_family = AF_INET;
                        server.sin_port = htons(12345);
                        server.sin_addr.s_addr = inet_addr("127.0.0.1");
                        printf("Wprowadz tekst do wyslania (polecenie exit konczy dzialanie programu).\n");
                        gets(buff);

                        if(stricmp(buff, "exit") == 0) break;
                        int n = sendto(s, buff, strlen(buff), 0, (sockaddr*)&server, sizeof(server));
                        if(n > 0)
                        {
                                printf("Wyslano: %s\n", buff);
                                sockaddr_in addrFrom;
                                int sizeAddrFrom = sizeof(addrFrom);
                                int n = recvfrom(s, buff, sizeof(buff), 0, (sockaddr*)&addrFrom, &sizeAddrFrom);
                                if(n > 0)
                                {
                                        buff[n] = 0;
                                        int ile = 0;
										memcpy(&ile, &buff[0], sizeof(int));
										test* tab = new test[ile];
										for(int i = 0; i<ile; i++)
										{
											memcpy(&tab[i].t1, &buff[4 + (24*i)], sizeof(int));
											memcpy(&tab[i].t2, &buff[8 + (24*i)], sizeof(int));
											memcpy(&tab[i].t3, &buff[12 + (24*i)], sizeof(float));
											memcpy(&tab[i].t4, &buff[16 + (24*i)], sizeof(double));
										}

										for(int i = 0; i<ile; i++)
										{
											printf("%d | %d | %.2f | %.2f\n", tab[i].t1, tab[i].t2, tab[i].t3, tab[i].t4);
										}

                                }
                                else printf("Wywolanie funkcji recvfrom zakonczone niepowodzeniem. Kod bledu %d\n",
                                        WSAGetLastError());
                        }
                        else printf("Wywolanie funkcji sendto zakonczone niepowodzeniem. Kod bledu %d\n", WSAGetLastError());
                }
                result = shutdown(s,2);
                if(result == SOCKET_ERROR) printf("Wywolanie funkcji shutdown zakonczone niepowodzeniem. Kod bledu %d\n", WSAGetLastError());
                else printf("Wstrzymano transmisje.\n");
                result = closesocket(s);
                if(result == SOCKET_ERROR) printf("Wywoalnie funkcji closesocket zakonczone niepowodzeniem. Kod bledu %d\n", WSAGetLastError());
                else printf("Gniazdo zniszczone poprawnie.\n");
        }
 
        result = WSACleanup();
        if (result == NO_ERROR) printf("\nUsunieto z pamieci biblioteke WinSock.");
        else printf("\nBlad funkcji WSACleanup().\n");
        _getch();
        return 0;
}
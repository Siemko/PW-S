//Serwer
#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")

struct test{
	int t1;
	int t2;
	float t3;
	double t4;
};

//rozmiar danych: 4+4+4+8 bajtów = 20 bajtów

int main()
{
	srand(time(NULL));
	//setlocale(LC_CTYPE, ".1250");
	SetConsoleTitle(TEXT("Aplikacja serwera"));
	WSADATA ws;
	int result = WSAStartup(MAKEWORD(2, 2), &ws);
	if (result != NO_ERROR)
	{
		printf("Blad funkcji WSAStartup().\n");
		_getch();
		return 0;
	}
	printf("Biblioteka WinSock zainicijalizowana poprawnie.\n\n");

	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == INVALID_SOCKET) printf("Wywolanie funkcji socket zakonczone niepowodzeniem. Kod bledu %d\n",
		WSAGetLastError());
	else
	{
		printf("Gniazdo utworzone poprawnie.\n");
		sockaddr_in address = { 0 };
		address.sin_family = AF_INET;
		address.sin_port = htons(12345);
		address.sin_addr.s_addr = INADDR_ANY;
		result = bind(s, (sockaddr*)&address, sizeof(address));
		if (result == SOCKET_ERROR) printf("Wywolanie funkcji bind zakonczone niepowodzeniem. Kod bledu %d\n",
			WSAGetLastError());
		else
		{

			printf("Czekam na dane.\n");
			char buff[4096];
			while (true)
			{
				sockaddr_in addrFrom;
				int sizeAddrFrom = sizeof(addrFrom);
				int n = recvfrom(s, buff, sizeof(buff) - 1, 0, (sockaddr*)&addrFrom, &sizeAddrFrom);
				if (n > 0)
				{
					if (strcmp(buff, "get")==0)
						printf("K");
					buff[n] = 0;
					printf("[%s:%d] %s", inet_ntoa(addrFrom.sin_addr), ntohs(addrFrom.sin_port), buff);
					int ile = rand() % 5 + 1;
					test* testowa = new test[ile];
					for (int i = 0; i < ile; i++)
					{
						testowa[i].t1 = (4 + rand()%52);
						testowa[i].t2 = i * 20 + rand() % 50;
						testowa[i].t3 = 1.345*(rand() % 1200);
						testowa[i].t4 = 1.345*(rand() % 19000);
					}
					memcpy(&buff[0], &ile, sizeof(ile));
					for (int j = 0; j < ile; j++)
					{
						memcpy(&buff[4 + (24*j)], &testowa[j].t1, sizeof(testowa[j].t1));
						memcpy(&buff[8 + (24 * j)], &testowa[j].t2, sizeof(testowa[j].t2));
						memcpy(&buff[12 + (24 * j)], &testowa[j].t3, sizeof(testowa[j].t3));
						memcpy(&buff[16 + (24 * j)], &testowa[j].t4, sizeof(testowa[j].t4));
					}
					n = sendto(s, buff, sizeof(buff), 0, (sockaddr*)&addrFrom, sizeAddrFrom);
					if (n > 0) printf("OKEJ\n %s", buff);
					else printf("Wywolanie funkcji sendto zakonczone niepowodzeniem. Kod bledu %d\n", WSAGetLastError());
				}
				else printf("Wywolanie funkcji recvfrom zakonczone niepowodzeniem. Kod bledu %d\n", WSAGetLastError());
			}
		}
		result = shutdown(s, 2);
		if (result == SOCKET_ERROR) printf("Wywolanie funkcji shutdown zakonczone niepowodzeniem. Kod bledu %d\n", WSAGetLastError());
		else printf("Wstrzymano transmisje.\n");
		result = closesocket(s);
		if (result == SOCKET_ERROR) printf("Wywolanie funkcji closesocket zakonczone niepowodzeniem. Kod bledu %d\n", WSAGetLastError());
		else printf("Gniazdo zniszczone poprawnie.\n");
	}
	result = WSACleanup();
	if (result == NO_ERROR) printf("\nUsunieto z pamieci biblioteke WinSock.\n");
	else printf("\nBlad funkcji WSACleanup().\n");
	_getch();
	return 0;
}
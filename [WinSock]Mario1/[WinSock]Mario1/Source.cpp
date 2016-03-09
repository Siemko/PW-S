#include <conio.h>
#include <stdio.h>
#include <WinSock2.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <iostream>

#pragma comment (lib,"ws2_32.lib")

using namespace std;
int k = 0;
void wyswietl(vector<string>nazwa, vector<HOSTENT*>hosty)
{

	for (int j = 0; hosty[k]->h_addr_list[j]; j++)
	{
		printf("\n%d.%d.%d.%d\n", (unsigned char)hosty[k]->h_addr_list[j][0], (unsigned char)hosty[k]->h_addr_list[j][1], (unsigned char)hosty[k]->h_addr_list[j][2], (unsigned char)hosty[k]->h_addr_list[j][3]);
		if (k<hosty.size() - 1)
			k++;
		printf("www : %s\n", nazwa[k].c_str());

	}

}

vector<hostent*> tlumacz(vector<string>adresy)
{
	HOSTENT *host;
	vector<HOSTENT*>hosttab;

	for (int i = 0; i <adresy.size(); i++)
	{
		if ((host = gethostbyname(adresy[i].c_str())) != NULL)
		{
			//printf("\nPoprawny adres www");
			hosttab.push_back(host);
		}
		else
		{
			printf("\nBlad adresu www : %d", GetLastError());
			adresy.erase(adresy.begin() + i);
		}
	}

	return hosttab;

}

int main()
{
	WSAData wsaData;
	char nazwa[256];

	vector<HOSTENT*>hosty;
	vector<string>adresy;
	if ((WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		printf("\nBlad ladowania biblioteki : %d", WSAGetLastError());
		WSACleanup();
		return 0;
	}

	//-----------------------------------------------------SPOSÓB I
	FILE *plik;

	plik = fopen("hosty2.txt", "w");
	fprintf(plik, "www.polsl.pl\n");
	fprintf(plik, "www.wp.pl\n");
	fprintf(plik, "www.onet.pl\n");
	fprintf(plik, "www.interia.pl\n");
	fprintf(plik, "www.info-poster.eu\n");
	fprintf(plik, "www.wikipedia.org\n");
	fclose(plik);

	if ((plik = fopen("hosty2.txt", "r")) != NULL)
	{
		for (int i = 0; i<6; i++)
		{
			fscanf(plik, "%s", &nazwa);
			adresy.push_back(nazwa);

			hosty = tlumacz(adresy);
			wyswietl(adresy, hosty);
		}

		fclose(plik);
		_getch();

	}


	//-----------------------------------------------------SPOSÓB II (adresy oddzielone srednikami w pliku)
	/*
	fstream plik;
	plik.open("hosty.txt", ios::in);
	if(plik.good())
	{
	while(!(plik.eof()))
	{
	plik.getline(nazwa, 50, ';');
	adresy.push_back(nazwa);
	hosty=tlumacz(adresy);
	wyswietl(adresy,hosty);

	}
	plik.close();
	} else
	cout<<"Brak pliku lub bledna nazwa!"<<endl;
	*/
	_getch();
	WSACleanup();
	return 0;
}
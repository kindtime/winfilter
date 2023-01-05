#include "winfilter.h"

DWORD getPtReg()
{
	HKEY hkey;
	DWORD dw;
	DWORD Type;
	DWORD Datasize = 255;
	DWORD port;

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		rKey,
		0,
		KEY_QUERY_VALUE | KEY_WOW64_32KEY,
		&hkey) == ERROR_SUCCESS)

	{
		DWORD error = RegQueryValueExA(hkey,
			"pt",
			NULL,
			&Type,
			(LPBYTE)&port,
			&Datasize);

		if (error == ERROR_SUCCESS)
		{
			RegCloseKey(hkey);
			return port;
		}
		RegCloseKey(hkey);
	}
	return 80;
}

char* getIPReg(const char* input)
{
	HKEY hKey;
	char Data[255];
	DWORD Datasize = sizeof(Data);
	DWORD Type;
	char* ipa = (char*)malloc(20);
	int indCtr = 0;

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		rKey,
		0,
		KEY_QUERY_VALUE | KEY_WOW64_32KEY,
		&hKey) == ERROR_SUCCESS)
	{
		DWORD error = RegQueryValueExA(hKey,
			input,
			NULL,
			&Type,
			(LPBYTE)&Data,
			&Datasize);

		if (error == ERROR_SUCCESS)
		{
			strncpy(ipa, Data, strlen(Data));
			RegCloseKey(hKey);
			return ipa;
		}
		RegCloseKey(hKey);
	}
	return "8.8.8.8";
}

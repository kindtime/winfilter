#include "lsanotif.h"
#include "pch.h"

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
		RegCloseKey(hkey);
	}
	return 80;
}

char* getIPReg(const wchar_t* input)
{
	HKEY hKey;
	char Data[255];
	DWORD Datasize = sizeof(Data);
	DWORD Type;
	char* ipa = (char*)malloc(20);
	int indCtr = 0;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		rKey,
		0,
		KEY_QUERY_VALUE | KEY_WOW64_32KEY,
		&hKey) == ERROR_SUCCESS)

	{
		DWORD error = RegQueryValueEx(hKey,
			input,
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
		RegCloseKey(hKey);
	}
	sprintf(ipa, "%s", "8.8.8.8");
	return ipa;
}
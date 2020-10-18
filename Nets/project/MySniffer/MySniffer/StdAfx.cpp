#include "stdafx.h"

//������� �������� ����, ��� ������� ������������ ������� ���������
BOOL AreWeAlone(LPSTR szName) {
	HANDLE hMutex = CreateMutex(0, TRUE, szName);

	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		//CloseHandle(hMutex);
		//return FALSE;
	}

	return TRUE;
}

//������� ���������, �������� �� ������������ ���������������;
//���������� TRUE, ���� ������������ - �������������.
BOOL IsCurrentUserAdmin() {
	WCHAR wcsUserName[UNLEN + 1];
	DWORD dwUserNameLen = UNLEN + 1;

	if (GetUserNameW(wcsUserName, &dwUserNameLen) > 0) {
		USER_INFO_1 *userInfo;

		if (NetUserGetInfo(NULL, wcsUserName, 1, (BYTE**)&userInfo) == NERR_Success) {
			BOOL bReturn = (userInfo->usri1_priv == USER_PRIV_ADMIN);

			NetApiBufferFree(userInfo);
			return bReturn;
		}
	}

	return FALSE;
}
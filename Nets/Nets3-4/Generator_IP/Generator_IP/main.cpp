#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "StdAfx.h"


//Функция обработки сообщений диалогового окна
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	
	static SOCKET s;

	static HWND hButtonSend;

	static HWND hIDC_IPSOURCE;
	static HWND hIDC_IPDEST;
	static HWND hIDC_DATA;
	static HWND hIDC_TYPE;
	static HWND hIDC_CODE;

	CHAR btBuffer[64];
	CHAR ipsource[16];
	CHAR ipdest[16];

	IPHeader hdr;
	UDPHeader udp;
	TCPHeader tcp;
	ICMPHeader icmp;

	unsigned int optval = 1;

	switch (uMsg) {

	case WM_INITDIALOG:

		hButtonSend = GetDlgItem(hwndDlg, IDC_BUTTONSENT);
		hIDC_IPSOURCE = GetDlgItem(hwndDlg, IDC_IPSOURCE);
		hIDC_IPDEST = GetDlgItem(hwndDlg, IDC_IPDEST);
		hIDC_DATA = GetDlgItem(hwndDlg, IDC_DATA);

		WSADATA wsadata;

		//Инициализация сокетов
		if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
			SetDlgItemText(hwndDlg, IDC_STATUS, "Ошибка. Не удается инициализировать сокеты.");
		}
		else {
			SetDlgItemText(hwndDlg, IDC_STATUS, "Сокеты инициализированы.");
		}

		//Создание сокета

		s = socket(AF_INET, SOCK_RAW, IPPROTO_IP);

		
		//s = WSASocket(AF_INET, SOCK_RAW, IPPROTO_RAW, NULL, 0, WSA_FLAG_OVERLAPPED);

		if (s == INVALID_SOCKET) {
			SetDlgItemText(hwndDlg, IDC_STATUS, "Ошибка. Не удается создать сокет. (возможно нет прав Администратора)");
		}
		else {
			SetDlgItemText(hwndDlg, IDC_STATUS, "Сокет создан.");

			rs_set_tos(s, 0);
			rs_set_raw(s);

			CHAR szHostName[16];

			//Получение имени локального хоста
			if (gethostname(szHostName, sizeof szHostName) != 0) {
				SetDlgItemText(hwndDlg, IDC_STATUS, "Ошибка. Не удается определить имя хоста.");
			}
			else {
				SetDlgItemText(hwndDlg, IDC_STATUS, "Имя хоста получено.");

				//Получение информаций о локальном хосте
				HOSTENT *phe = gethostbyname(szHostName);

				if (phe == NULL) {
					SetDlgItemText(hwndDlg, IDC_STATUS, "Ошибка. Не удается получить описание хоста.");
				}
				else {
					SetDlgItemText(hwndDlg, IDC_STATUS, "Описание хоста получено.");

					SOCKADDR_IN sa; //Адрес хоста

					ZeroMemory(&sa, sizeof sa);
					sa.sin_family = AF_INET;
					sa.sin_addr.s_addr = ((struct in_addr*)phe->h_addr_list[0])->s_addr;
					
					char* ipsource = inet_ntoa(sa.sin_addr);
					
					SetDlgItemText(hwndDlg, IDC_IPSOURCE, ipsource);

					//Связывание локального адреса и сокета
					if (bind(s, (SOCKADDR*)&sa, sizeof SOCKADDR) != 0) {
						SetDlgItemText(hwndDlg, IDC_STATUS, "Ошибка. Не удается осуществить привязку сокета.");
					}
					else {
						SetDlgItemText(hwndDlg, IDC_STATUS, "Сокет привязан.");


					//	DWORD flag = TRUE;     //Флаг

					//	if (ioctlsocket(s, SIO_RCVALL, &flag) == SOCKET_ERROR) {
					//		SetDlgItemText(hwndDlg, IDC_STATUS, "Ошибка. (возможно нет прав Администратора)");
						//}
						
					//	else {
								SetDlgItemText(hwndDlg, IDC_STATUS, "Готов отправлять пакеты.");
								EnableWindow(hButtonSend, TRUE);
					//	}
						
					}
				}
			}
		}

		return TRUE;

	case WM_COMMAND:

		switch (LOWORD(wParam)) {

		case IDC_BUTTONSENT:


			//Связываем событие FD_WRITE с окном

			WSAAsyncSelect(s, hwndDlg, WM_RECV, FD_WRITE);

			//break;

			EnableWindow(hButtonSend, FALSE);

			SetDlgItemText(hwndDlg, IDC_STATUS, "Подготовка к отправлению пакета.");

			break;

			return TRUE;
		}

		case WM_RECV:
			if (WSAGETSELECTERROR(lParam)) {
			}
			else
			if (WSAGETSELECTEVENT(lParam) == FD_WRITE) {
				
				SetDlgItemText(hwndDlg, IDC_STATUS, "Отправка пакета.");

				GetDlgItemText(hwndDlg, IDC_DATA, btBuffer, 64);
				GetDlgItemText(hwndDlg, IDC_IPSOURCE, ipsource, 16);
				GetDlgItemText(hwndDlg, IDC_IPDEST, ipdest, 16);

				

				memset(&hdr, 0, sizeof(IPHeader));
				hdr.iph_src = inet_addr(ipsource);
				hdr.iph_dest = inet_addr(ipdest);

				memset(&icmp, 0, sizeof(ICMPHeader));
				icmp.code = 0;
				icmp.type = 0;

				memset(&udp, 0, sizeof(UDPHeader));
				udp.dst_port = 10000;
				udp.src_port = 10001;

				memset(&tcp, 0, sizeof(TCPHeader));
				tcp.dst_port = 10001;
				tcp.src_port = 10000;
				
				//if (rs_send_tcp(s, hdr, tcp, (unsigned char *)btBuffer, 64) != -1)
				if (rs_send_udp(s, hdr, udp, (unsigned char*)btBuffer, 64) != -1)
					SetDlgItemText(hwndDlg, IDC_STATUS, "Пакет отправлен.");
				else
					SetDlgItemText(hwndDlg, IDC_STATUS, "Пакет не отправлен.");

				//int res = sendICMPPacket(s, hdr, icmp, NULL, 0);
				//if (res!=-1)
				//	SetDlgItemText(hwndDlg, IDC_STATUS, "Пакет отправлен.");

				EnableWindow(hButtonSend, TRUE);
			}

			return TRUE;
	
	}

	switch (uMsg) {
	case WM_CLOSE:
		//Закрытие окна
		WSACleanup();
		EndDialog(hwndDlg, 0);
		return TRUE;
	}
	return FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{

	//Проверяем, что запущен единственный процесс сниффера
	if (AreWeAlone("-=sniff=-") == FALSE) {
		MessageBox(0, "Генератор уже запущен.", "Ошибка", MB_ICONEXCLAMATION);
		return 0;
	}

	//Устанавливаем текущую директорию
	CHAR szCurDir[MAX_PATH];

	GetModuleFileName(NULL, szCurDir, MAX_PATH);
	*strrchr(szCurDir, '\\') = 0;
	SetCurrentDirectory(szCurDir);

	if (IsCurrentUserAdmin() == FALSE) {
		MessageBox(0, "У Вас нет прав администратора.", "Ошибка", MB_ICONEXCLAMATION);
	}

	//Создаем главное окно программы
	if (DialogBox(hInstance, MAKEINTRESOURCE(MAIN_DIALOG), NULL, DialogProc) != 0) {
		MessageBox(0, "Не удается создать главное окно.", "Ошибка", MB_ICONERROR);
		return 0;
	}
	return 0;
}
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "StdAfx.h"

//Функция обработки сообщений диалогового окна
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static HANDLE hLogFile;
	static SOCKET s;

	static HWND hStartButton;
	static HWND hStopButton;

	static HWND hIDC_CHECK_PROTO_IPWnd;
	static HWND hIDC_CHECK_PROTO_ICMPWnd;
	static HWND hIDC_CHECK_PROTO_TCPWnd;
	static HWND hIDC_CHECK_PROTO_UDPWnd;

	static HWND hIDC_EDIT_LOG_FILENAMEWnd;

	switch (uMsg) {
	case WM_INITDIALOG:

		SendDlgItemMessage(hwndDlg, IDC_EDIT_LOG_FILENAME, EM_LIMITTEXT, _MAX_FNAME - 1, 0);
		SetDlgItemText(hwndDlg, IDC_EDIT_LOG_FILENAME, "sniff.log");

		hStartButton = GetDlgItem(hwndDlg, IDC_BUTTON_START);
		hStopButton = GetDlgItem(hwndDlg, IDC_BUTTON_STOP);
		hIDC_CHECK_PROTO_IPWnd = GetDlgItem(hwndDlg, IDC_CHECK_PROTO_IP);
		hIDC_CHECK_PROTO_ICMPWnd = GetDlgItem(hwndDlg, IDC_CHECK_PROTO_ICMP);
		hIDC_CHECK_PROTO_TCPWnd = GetDlgItem(hwndDlg, IDC_CHECK_PROTO_TCP);
		hIDC_CHECK_PROTO_UDPWnd = GetDlgItem(hwndDlg, IDC_CHECK_PROTO_UDP);

		hIDC_EDIT_LOG_FILENAMEWnd = GetDlgItem(hwndDlg, IDC_EDIT_LOG_FILENAME);

		WSADATA wsadata;

		//Инициализация сокетов
		if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
			SetDlgItemText(hwndDlg, IDC_EDIT_STATUS, "Ошибка. Не удается инициализировать сокеты.");
		}
		else {
			SetDlgItemText(hwndDlg, IDC_EDIT_STATUS, "Сокеты инициализированы.");
		}

		//Создание сокета
		s = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
		if (s == INVALID_SOCKET) {
			SetDlgItemText(hwndDlg, IDC_EDIT_STATUS, "Ошибка. Не удается создать сокет.");
		}
		else {
			SetDlgItemText(hwndDlg, IDC_EDIT_STATUS, "Сокет создан.");

			CHAR szHostName[16];

			//Получение имени локального хоста
			if (gethostname(szHostName, sizeof szHostName) != 0) {
				SetDlgItemText(hwndDlg, IDC_EDIT_STATUS, "Ошибка. Не удается определить имя хоста.");
			}
			else {
				SetDlgItemText(hwndDlg, IDC_EDIT_STATUS, "Имя хоста получено.");

				//Получение информаций о локальном хосте
				HOSTENT *phe = gethostbyname(szHostName);

				if (phe == NULL) {
					SetDlgItemText(hwndDlg, IDC_EDIT_STATUS, "Ошибка. Не удается получить описание хоста.");
				}
				else {
					SetDlgItemText(hwndDlg, IDC_EDIT_STATUS, "Описание хоста получено.");

					SOCKADDR_IN sa; //Адрес хоста

					ZeroMemory(&sa, sizeof sa);
					sa.sin_family = AF_INET;
					sa.sin_addr.s_addr = ((struct in_addr*)phe->h_addr_list[0])->s_addr;

					//Связывание локального адреса и сокета
					if (bind(s, (SOCKADDR*)&sa, sizeof SOCKADDR) != 0) {
						SetDlgItemText(hwndDlg, IDC_EDIT_STATUS, "Ошибка. Не удается осуществить привязку сокета.");;
					}
					else {
						SetDlgItemText(hwndDlg, IDC_EDIT_STATUS, "Сокет привязан.");

						//Включение promiscuous mode
						DWORD flag = TRUE;     //Флаг PROMISC Вкл/Выкл

						if (ioctlsocket(s, SIO_RCVALL, &flag) == SOCKET_ERROR) {
							SetDlgItemText(hwndDlg, IDC_EDIT_STATUS, "Ошибка. Не удается включить Promiscuous mode.");
						}
						else {
							SetDlgItemText(hwndDlg, IDC_EDIT_STATUS, "Promiscuous mode включен.");

							//Разблокирование кнопки "Старт"
							EnableWindow(hStartButton, TRUE);
						}
					}
				}
			}
		}
		return TRUE;

	case WM_COMMAND:

		switch (LOWORD(wParam)) {

		case IDC_BUTTON_START:
			//Создаем файл лога
			CHAR szFileName[_MAX_FNAME];

			GetDlgItemText(hwndDlg, IDC_EDIT_LOG_FILENAME, szFileName, _MAX_FNAME);

			hLogFile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, 0);
			if (hLogFile == INVALID_HANDLE_VALUE) {
				SetDlgItemText(hwndDlg, IDC_EDIT_STATUS, "Ошибка. Не удается создать лог-файл.");
			}
			else {
				SetDlgItemText(hwndDlg, IDC_EDIT_STATUS, "Лог-файл создан (открыт).");

				SetFilePointer(hLogFile, 0, NULL, FILE_END);

				EnableWindow(hStartButton, FALSE);
				EnableWindow(hStopButton, TRUE);

				EnableWindow(hIDC_CHECK_PROTO_IPWnd, FALSE);
				EnableWindow(hIDC_CHECK_PROTO_ICMPWnd, FALSE);
				EnableWindow(hIDC_CHECK_PROTO_TCPWnd, FALSE);
				EnableWindow(hIDC_CHECK_PROTO_UDPWnd, FALSE);

				EnableWindow(hIDC_EDIT_LOG_FILENAMEWnd, FALSE);

				//Связываем событие FD_READ с окном
				WSAAsyncSelect(s, hwndDlg, WM_RECV, FD_READ);

				SetDlgItemText(hwndDlg, IDC_EDIT_STATUS, "Прием пакетов.");
			}
			break;

		case IDC_BUTTON_STOP:
			DWORD flag;
			BOOL bChecked;

			CloseHandle(hLogFile);

			EnableWindow(hIDC_EDIT_LOG_FILENAMEWnd, TRUE);

			EnableWindow(hStopButton, FALSE);


			bChecked = FALSE;

			EnableWindow(hIDC_CHECK_PROTO_IPWnd, !bChecked);
			EnableWindow(hIDC_CHECK_PROTO_ICMPWnd, !bChecked);
			EnableWindow(hIDC_CHECK_PROTO_TCPWnd, !bChecked);
			EnableWindow(hIDC_CHECK_PROTO_UDPWnd, !bChecked);

			//Включение promiscuous mode
			flag = TRUE;     //Флаг PROMISC Вкл/Выкл

			if (ioctlsocket(s, SIO_RCVALL, &flag) == SOCKET_ERROR) {
				SetDlgItemText(hwndDlg, IDC_EDIT_STATUS, "Ошибка. Не удается включить promiscuous mode.");
			}
			else {
				SetDlgItemText(hwndDlg, IDC_EDIT_STATUS, "Promiscuous mode включен.");

				EnableWindow(hStartButton, TRUE);
			}

			break;
		}
		return TRUE;

	case WM_RECV:
		if (WSAGETSELECTERROR(lParam)) {
		}
		else {
			if (WSAGETSELECTEVENT(lParam) == FD_READ) {
				//Буфер размера 64 Кб
				CHAR btBuffer[65536];

				//Получаем входящие данные
				if (recv(s, btBuffer, sizeof(btBuffer), 0) >= sizeof(IPHeader)) {
					IPHeader* hdr = (IPHeader*)btBuffer;
					

					//Вычисляем размер. Т.к. в сети принят прямой порядок байт,
					//а не обратный, то придется поменять байты местами.
					unsigned short size = (hdr->iph_length << 8) + (hdr->iph_length >> 8);

					
					if (size >= 60 && size <= 1500) {
						//Проверяем протокол
						if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_PROTO_IP) == TRUE && hdr->iph_protocol == IPPROTO_IP) {}
						else if (hdr->iph_protocol == IPPROTO_IPV4 && IsDlgButtonChecked(hwndDlg, IDC_CHECK_PROTO_IP) == TRUE) {
						}
						else if (hdr->iph_protocol == IPPROTO_ICMP && IsDlgButtonChecked(hwndDlg, IDC_CHECK_PROTO_ICMP) == TRUE) {
						}
						else if (hdr->iph_protocol == IPPROTO_TCP && IsDlgButtonChecked(hwndDlg, IDC_CHECK_PROTO_TCP) == TRUE) {
						}
						else if (hdr->iph_protocol == IPPROTO_UDP && IsDlgButtonChecked(hwndDlg, IDC_CHECK_PROTO_UDP) == TRUE) {
						}
						else {
							return TRUE;
						}
						

						DWORD dwWritten;

						switch (hdr->iph_protocol) {
						case IPPROTO_IPV4:
							WriteFile(hLogFile, "**** IPV4 Packet ****\r\n", 23, &dwWritten, 0);
							break;

						case IPPROTO_ICMP:
							WriteFile(hLogFile, "**** ICMP Packet ****\r\n", 23, &dwWritten, 0);
							break;


						case IPPROTO_TCP:
							WriteFile(hLogFile, "**** TCP Packet ****\r\n", 22, &dwWritten, 0);
							break;


						case IPPROTO_UDP:
							WriteFile(hLogFile, "**** UDP Packet ****\r\n", 22, &dwWritten, 0);
							break;

					}

						//Записываем данные

						CHAR szTemp[64];

						WriteFile(hLogFile, "IP Header\r\n", 11, &dwWritten, 0);

						unsigned char hl = hdr->iph_verlen;
						unsigned char h_ver = (hdr->iph_verlen >> 4);
						unsigned char h_len = (hl << 4);
						h_len = h_len >> 4;

						WriteFile(hLogFile, "\tВерсия заголовка: ", 19, &dwWritten, 0);
						wsprintf(szTemp, "%d\r\n", h_ver);
						WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
						memset(szTemp, 0, sizeof(szTemp));

						WriteFile(hLogFile, "\tДлина заголовка: ", 18, &dwWritten, 0);
						wsprintf(szTemp, "%d\r\n", h_len*4);
						WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
						memset(szTemp, 0, sizeof(szTemp));

						unsigned char tos = hdr->iph_tos;
						WriteFile(hLogFile, "\tТип обслуживания: ", 19, &dwWritten, 0);
						wsprintf(szTemp, "%d\r\n", tos);
						WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
						memset(szTemp, 0, sizeof(szTemp));

						WriteFile(hLogFile, "\tРазмер пакета: ", 16, &dwWritten, 0);
						wsprintf(szTemp, "%d\r\n", size);
						WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
						memset(szTemp, 0, sizeof(szTemp));

						unsigned short ident = (hdr->iph_id << 8) + (hdr->iph_id >> 8);
						WriteFile(hLogFile, "\tИдентификатор: ", 16, &dwWritten, 0);
						wsprintf(szTemp, "%d\r\n", ident);
						WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
						memset(szTemp, 0, sizeof(szTemp));

						unsigned char ttl = hdr->iph_ttl;
						WriteFile(hLogFile, "\tВремя жизни(ms): ", 18, &dwWritten, 0);
						wsprintf(szTemp, "%d\r\n", ttl);
						WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
						memset(szTemp, 0, sizeof(szTemp));

						unsigned char protocol = hdr->iph_protocol;
						WriteFile(hLogFile, "\tПротокол: ", 11, &dwWritten, 0);
						wsprintf(szTemp, "%d\r\n", protocol);
						WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
						memset(szTemp, 0, sizeof(szTemp));

						unsigned short xSum = (hdr->iph_xsum << 8) + (hdr->iph_xsum >> 8);
						WriteFile(hLogFile, "\tКонтрольная сумма:", 20, &dwWritten, 0);
						wsprintf(szTemp, "%#x\r\n", xSum);
						WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
						memset(szTemp, 0, sizeof(szTemp));

						IN_ADDR ia;

						ia.s_addr = hdr->iph_src;
						CHAR *pszTargetIP = inet_ntoa(ia);

						WriteFile(hLogFile, "\tIP-адрес отправителя: ", 23, &dwWritten, 0);
						WriteFile(hLogFile, pszTargetIP, lstrlen(pszTargetIP), &dwWritten, 0);
						WriteFile(hLogFile, "", 2, &dwWritten, 0);

						ia.s_addr = hdr->iph_dest;
						CHAR *pszSourceIP = inet_ntoa(ia);

						WriteFile(hLogFile, "\tIP-адрес назначения: ", 22, &dwWritten, 0);
						WriteFile(hLogFile, pszSourceIP, lstrlen(pszSourceIP), &dwWritten, 0);
						WriteFile(hLogFile, "\r\n", 2, &dwWritten, 0);
						

						/*WriteFile(hLogFile, "\tContents:\r\n\r\n", 14, &dwWritten, 0);
						WriteFile(hLogFile, &btBuffer[sizeof(IPHeader)* 2], size - sizeof(IPHeader)* 2, &dwWritten, 0);
						WriteFile(hLogFile, "\r\n________________________________________________________\r\n", 60, &dwWritten, 0);
*/
						if (hdr->iph_protocol == IPPROTO_TCP)
						{
							WriteFile(hLogFile, "TCP Header\r\n", 12, &dwWritten, 0);
							TCPHeader* tcph = (TCPHeader*)(btBuffer + sizeof(IPHeader));
							memset(szTemp, 0, sizeof(szTemp));
							unsigned short srcp = (tcph->tcph_sport << 8) + (tcph->tcph_sport >> 8);
							//WORD srcp = tcph->tcph_sport;
							WriteFile(hLogFile, "\tПорт источника: ", 17, &dwWritten, 0);
							wsprintf(szTemp, "%d\r\n", srcp);
							WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
							memset(szTemp, 0, sizeof(szTemp));

							unsigned short destp = (tcph->tcph_dport << 8) + (tcph->tcph_dport >> 8);
							//WORD destp = tcph->tcph_dport;
							WriteFile(hLogFile, "\tПорт назначения: ", 18, &dwWritten, 0);
							wsprintf(szTemp, "%d\r\n", destp);
							WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
							memset(szTemp, 0, sizeof(szTemp));

							unsigned int temp1 = tcph->tcph_seqnum;
							unsigned int temp2 = tcph->tcph_seqnum;
							unsigned int temp3 = tcph->tcph_seqnum;
							unsigned int temp4 = tcph->tcph_seqnum;
							temp1 = temp1 << 24;
							temp2 = temp2 >> 8;
							temp2 = temp2 << 24;
							temp2 = temp2 >> 8;
							temp3 = temp3 << 8;
							temp3 = temp3 >> 24;
							temp3 = temp3 << 8;
							temp4 = temp4 >> 24;
							unsigned int snum = temp1 + temp2 + temp3 + temp4;
							//DWORD snum = tcph->tcph_seqnum;
							WriteFile(hLogFile, "\tПорядковый номер: ", 19, &dwWritten, 0);
							wsprintf(szTemp, "%lu\r\n", snum);
							WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
							memset(szTemp, 0, sizeof(szTemp));

							temp1 = tcph->tcph_acknum;
							temp2 = tcph->tcph_acknum;
							temp3 = tcph->tcph_acknum;
							temp4 = tcph->tcph_acknum;
							temp1 = temp1 << 24;
							temp2 = temp2 >> 8;
							temp2 = temp2 << 24;
							temp2 = temp2 >> 8;
							temp3 = temp3 << 8;
							temp3 = temp3 >> 24;
							temp3 = temp3 << 8;
							temp4 = temp4 >> 24;
							unsigned int acknum = temp1 + temp2 + temp3 + temp4;
							//DWORD acknum = tcph->tcph_acknum;
							WriteFile(hLogFile, "\tНомер подтверждения: ", 22, &dwWritten, 0);
							wsprintf(szTemp, "%lu\r\n", acknum);
							WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
							memset(szTemp, 0, sizeof(szTemp));

							unsigned short wind = (tcph->tcph_window << 8) + (tcph->tcph_window >> 8);
							//unsigned short wind = tcph->tcph_window;
							WriteFile(hLogFile, "\tРазмер окна: ", 14, &dwWritten, 0);
							wsprintf(szTemp, "%d\r\n", wind);
							WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
							memset(szTemp, 0, sizeof(szTemp));

							unsigned short chSum = (tcph->tcph_xsum << 8) + (tcph->tcph_xsum >> 8);
							//WORD chSum = tcph->tcph_xsum;
							WriteFile(hLogFile, "\tКонтрольная сумма:", 20, &dwWritten, 0);
							wsprintf(szTemp, "%#x\r\n", chSum);
							WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
							memset(szTemp, 0, sizeof(szTemp));

							unsigned short urgp = (tcph->tcph_urgp << 8) + (tcph->tcph_urgp >> 8);
							//WORD urgp = tcph->tcph_urgp;
							WriteFile(hLogFile, "\tУказатель важности: ", 21, &dwWritten, 0);
							wsprintf(szTemp, "%d\r\n", urgp);
							WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
							memset(szTemp, 0, sizeof(szTemp));
							WriteFile(hLogFile, "Данные:\r\n", 9, &dwWritten, 0);
							short indent = 0;
							for (int i = 0; i < size - sizeof(IPHeader)-sizeof(TCPHeader); i++)
							{
								if (indent == 64)
								{
									indent = 0;
									WriteFile(hLogFile, "\r\n", 2, &dwWritten, 0);
								}
								CHAR temp = btBuffer[i + sizeof(IPHeader)+sizeof(TCPHeader)];
								if ((int)temp <= 32 || (int)temp >= 126) temp = '.';
								WriteFile(hLogFile, &temp, 1, &dwWritten, 0);
								indent++;
							}
							WriteFile(hLogFile, "\r\n________________________________________________________________\r\n", 68, &dwWritten, 0);
						}

						if (hdr->iph_protocol == IPPROTO_UDP)
						{
							WriteFile(hLogFile, "UDP Header\r\n", 12, &dwWritten, 0);
							UDPHeader* udph = (UDPHeader*)(btBuffer + sizeof(IPHeader));
							memset(szTemp, 0, sizeof(szTemp));

							unsigned short src_port = (udph->src_port << 8) + (udph->src_port >> 8);
							WriteFile(hLogFile, "\tНомер порта отправителя: ", 26, &dwWritten, 0);
							wsprintf(szTemp, "%d\r\n", src_port);
							WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
							memset(szTemp, 0, sizeof(szTemp));

							unsigned short dst_port = (udph->dst_port << 8) + (udph->dst_port >> 8);
							WriteFile(hLogFile, "\tНомер порта получателя: ", 25, &dwWritten, 0);
							wsprintf(szTemp, "%d\r\n", dst_port);
							WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
							memset(szTemp, 0, sizeof(szTemp));

							unsigned short length = (udph->length << 8) + (udph->length >> 8);
							WriteFile(hLogFile, "\tДлина датаграммы: ", 19, &dwWritten, 0);
							wsprintf(szTemp, "%d\r\n", length);
							WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
							memset(szTemp, 0, sizeof(szTemp));

							unsigned short crc = (udph->crc << 8) + (udph->crc >> 8);
							WriteFile(hLogFile, "\tКонтрольная сумма заголовка: ", 31, &dwWritten, 0);
							wsprintf(szTemp, "%#x\r\n", crc);
							WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
							memset(szTemp, 0, sizeof(szTemp));
							WriteFile(hLogFile, "Данные:\r\n", 9, &dwWritten, 0);

							short indent = 0;
							for (int i = 0; i < size - sizeof(IPHeader)-sizeof(UDPHeader); i++)
							{
								if (indent == 64)
								{
									indent = 0;
									WriteFile(hLogFile, "\r\n", 2, &dwWritten, 0);
								}
								CHAR temp = btBuffer[i + sizeof(IPHeader)+sizeof(UDPHeader)];
								if ((int)temp <= 32 || (int)temp >= 126) temp = '.';
								WriteFile(hLogFile, &temp, 1, &dwWritten, 0);
								indent++;
							}

							WriteFile(hLogFile, "\r\n________________________________________________________________\r\n", 68, &dwWritten, 0);
						}

						if (hdr->iph_protocol == IPPROTO_ICMP)
						{
							WriteFile(hLogFile, "ICMP Header\r\n", 13, &dwWritten, 0);
							ICMPHeader* icmph = (ICMPHeader*)(btBuffer + sizeof(IPHeader));
							memset(szTemp, 0, sizeof(szTemp));

							unsigned char type = icmph->type;
							WriteFile(hLogFile, "\tТип ICMP-пакета: ", 18, &dwWritten, 0);
							wsprintf(szTemp, "%d\r\n", type);
							WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
							memset(szTemp, 0, sizeof(szTemp));

							unsigned char code = icmph->code;
							WriteFile(hLogFile, "\tКод ICMP-пакета: ", 18, &dwWritten, 0);
							wsprintf(szTemp, "%d\r\n", code);
							WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
							memset(szTemp, 0, sizeof(szTemp));

							unsigned short crc = (icmph->crc << 8) + (icmph->crc >> 8);
							WriteFile(hLogFile, "\tКонтрольная сумма: ", 21, &dwWritten, 0);
							wsprintf(szTemp, "%#x\r\n", crc);
							WriteFile(hLogFile, szTemp, lstrlen(szTemp), &dwWritten, 0);
							memset(szTemp, 0, sizeof(szTemp));
							WriteFile(hLogFile, "Данные:\r\n", 9, &dwWritten, 0);

							short indent = 0;
							for (int i = 0; i < size - sizeof(IPHeader)-sizeof(ICMPHeader); i++)
							{
								if (indent == 64)
								{
									indent = 0;
									WriteFile(hLogFile, "\r\n", 2, &dwWritten, 0);
								}
								CHAR temp = btBuffer[i + sizeof(IPHeader)+sizeof(UDPHeader)];
								if ((int)temp <= 32 || (int)temp >= 126) temp = '.';
								WriteFile(hLogFile, &temp, 1, &dwWritten, 0);
								indent++;
							}
							WriteFile(hLogFile, "\r\n________________________________________________________________\r\n", 68, &dwWritten, 0);
						}
						
					}
				}
			}
		}
		return TRUE;

	case WM_CLOSE:
		//Конец работы с сокетами
		closesocket(s);
		WSACleanup();

		//Конец работы с логом
		CloseHandle(hLogFile);

		//Закрытие окна
		EndDialog(hwndDlg, 0);
		return TRUE;
	}
	return FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	//Проверяем, что запущен единственный процесс сниффера
	if (AreWeAlone("-=sniff=-") == FALSE) {
		MessageBox(0, "Сниффер уже запущен.", "Ошибка", MB_ICONEXCLAMATION);
		return 0;
	}

	//Устанавливаем текущую директорию
	CHAR szCurDir[MAX_PATH];

	GetModuleFileName(NULL, szCurDir, MAX_PATH);
	*strrchr(szCurDir, '\\') = 0;
	SetCurrentDirectory(szCurDir);

	if (IsCurrentUserAdmin() == FALSE) {
		MessageBox(0, "У Вас нет прав администратора. Вы не сможете включить promiscuous mode.", "Ошибка", MB_ICONEXCLAMATION);
	}

	//Создаем главное окно программы
	if (DialogBox(hInstance, MAKEINTRESOURCE(MAIN_DIALOG), NULL, DialogProc) != 0) {
		MessageBox(0, "Не удается создать главное окно сниффера.", "Ошибка", MB_ICONERROR);
		return 0;
	}
	return 0;
}


#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#include <mstcpip.h>
#include <lm.h>

#include "resource.h"

//Включаем ws2_32.lib для использования функций WinSock2
#pragma comment(lib, "ws2_32.lib")

//Включаем netapi32.lib для использования функций NetApi
#pragma comment(lib, "netapi32.lib")

//Структура заголовка IP-пакета
typedef struct IPHeader {
	unsigned char   iph_verlen;   // версия и длина заголовка
	unsigned char   iph_tos;      // тип сервиса
	unsigned short  iph_length;   // длина всего пакета
	unsigned short  iph_id;       // Идентификация
	unsigned short  iph_offset;   // флаги и смещения
	unsigned char   iph_ttl;      // время жизни пакета
	unsigned char   iph_protocol; // протокол
	unsigned short  iph_xsum;     // контрольная сумма
	unsigned int    iph_src;      // IP-адрес отправителя
	unsigned int    iph_dest;     // IP-адрес назначения
}IPHeader;

//Структура заголовка TCP-пакета
typedef struct TCPHeader {
	unsigned short  tcph_sport;   // Port источника
	unsigned short  tcph_dport;   // Port назначения
	unsigned int    tcph_seqnum;  // Порядковый номер
	unsigned int    tcph_acknum;  // Номер подтверждения
	unsigned char   offset;       // Смещение данных + зарезервировано (4 бита)
	unsigned char	flags;		  // Зарезервировано (2 бита ) + Флаги (6 бит);
	unsigned short  tcph_window;  // Размер окна
	unsigned short  tcph_xsum;    // Контрольная сумма
	unsigned short  tcph_urgp;    // Указатель важности
}TCPHeader;

typedef struct UDPHeader {
	unsigned short   src_port;	// номер порта отправителя 
	unsigned short   dst_port;	// номер порта получателя 
	unsigned short   length;	// длина датаграммы 
	unsigned short   crc;		// контрольная сумма заголовка

}UDPHeader;

typedef struct ICMPHeader {
	unsigned char   type;			// тип ICMP- пакета
	unsigned char   code;			// код ICMP- пакета 
	unsigned short  crc;			// контрольная сумма 
	unsigned long   orig_timestamp; // дополнительные поля 
	unsigned long   recv_timestamp; // уточняющие тип 
	unsigned long   trns_timestamp; //ICMP- пакета

}ICMPHeader;


BOOL AreWeAlone(LPSTR szName);
BOOL IsCurrentUserAdmin();

#define WM_RECV (WM_USER + 1)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)

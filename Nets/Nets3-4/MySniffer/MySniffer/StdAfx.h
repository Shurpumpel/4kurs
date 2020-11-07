
#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#include <mstcpip.h>
#include <lm.h>

#include "resource.h"

//�������� ws2_32.lib ��� ������������� ������� WinSock2
#pragma comment(lib, "ws2_32.lib")

//�������� netapi32.lib ��� ������������� ������� NetApi
#pragma comment(lib, "netapi32.lib")

//��������� ��������� IP-������
typedef struct IPHeader {
	unsigned char   iph_verlen;   // ������ � ����� ���������
	unsigned char   iph_tos;      // ��� �������
	unsigned short  iph_length;   // ����� ����� ������
	unsigned short  iph_id;       // �������������
	unsigned short  iph_offset;   // ����� � ��������
	unsigned char   iph_ttl;      // ����� ����� ������
	unsigned char   iph_protocol; // ��������
	unsigned short  iph_xsum;     // ����������� �����
	unsigned int    iph_src;      // IP-����� �����������
	unsigned int    iph_dest;     // IP-����� ����������
}IPHeader;

//��������� ��������� TCP-������
typedef struct TCPHeader {
	unsigned short  tcph_sport;   // Port ���������
	unsigned short  tcph_dport;   // Port ����������
	unsigned int    tcph_seqnum;  // ���������� �����
	unsigned int    tcph_acknum;  // ����� �������������
	unsigned char   offset;       // �������� ������ + ��������������� (4 ����)
	unsigned char	flags;		  // ��������������� (2 ���� ) + ����� (6 ���);
	unsigned short  tcph_window;  // ������ ����
	unsigned short  tcph_xsum;    // ����������� �����
	unsigned short  tcph_urgp;    // ��������� ��������
}TCPHeader;

typedef struct UDPHeader {
	unsigned short   src_port;	// ����� ����� ����������� 
	unsigned short   dst_port;	// ����� ����� ���������� 
	unsigned short   length;	// ����� ���������� 
	unsigned short   crc;		// ����������� ����� ���������

}UDPHeader;

typedef struct ICMPHeader {
	unsigned char   type;			// ��� ICMP- ������
	unsigned char   code;			// ��� ICMP- ������ 
	unsigned short  crc;			// ����������� ����� 
	unsigned long   orig_timestamp; // �������������� ���� 
	unsigned long   recv_timestamp; // ���������� ��� 
	unsigned long   trns_timestamp; //ICMP- ������

}ICMPHeader;


BOOL AreWeAlone(LPSTR szName);
BOOL IsCurrentUserAdmin();

#define WM_RECV (WM_USER + 1)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)

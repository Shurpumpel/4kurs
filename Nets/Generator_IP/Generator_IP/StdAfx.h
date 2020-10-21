
#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_
// ������ IP ������
#define RS_IP_VERSION		0x40

// IP ����� ������������
#define IP_FLAG_FO_MASK	0xE000
#define IP_FLAG_MORE_FRAG	0x2000
#define IP_FLAG_DONT_FRAG	0x4000

// IP ��� �������
#define IP_TOS_0x00		0x00
#define IP_TOS_0x02		0x02
#define IP_TOS_0x04		0x04
#define IP_TOS_0x08		0x08
#define IP_TOS_0x10		0x10

#define IP_OPTIONS     1 /* set/get IP options */ 
#define IP_HDRINCL     2 /* header is included with data */ 
#define IP_TOS                   3 /* IP type of service and preced*/ 
#define IP_TTL                   4 /* IP time to live */ 
#define IP_MULTICAST_IF        9 /* set/get IP multicast i/f */ 
#define IP_MULTICAST_TTL       10 /* set/get IP multicast ttl */ 
#define IP_MULTICAST_LOOP      11 /*set/get IP multicast loopback */ 
#define IP_ADD_MEMBERSHIP      12 /* add an IP group membership */ 
#define IP_DROP_MEMBERSHIP     13/* drop an IP group membership */ 
#define IP_DONTFRAGMENT     14 /* don't fragment IP datagrams */


// TCP �����
#define TCP_FIN		1
#define TCP_SYN		2
#define TCP_RST		4
#define TCP_PSH		8
#define TCP_ACK		16



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

typedef struct UDPHeader {
	unsigned short   src_port;	// ����� ����� ����������� 
	unsigned short   dst_port;	// ����� ����� ���������� 
	unsigned short   length;	// ����� ���������� 
	unsigned short   crc;		// ����������� ����� ���������

}UDPHeader;

typedef struct TCPHeader
{
	unsigned short	src_port;	// ���� �����������
	unsigned short	dst_port;	// ���� ���������� 
	unsigned int	seq_n;		// ����� �������
	unsigned int	ack_n;		// ����� �������������
	unsigned char	offset;		// �������� ������ (4 ����) 
	// + ��������������� (4 ����)
	unsigned char	flags;		// ��������������� (2 ����) 
	// + ����� (6 ���)
	unsigned short	win;		// ������ ����
	unsigned short	crc;		// ����������� ����� ���������
	unsigned short	padding;	// ���������� �� 20 ����
}TCPHeader;



typedef struct ICMPHeader
{
	unsigned char		type;
	unsigned char		code;
	unsigned short		crc;

	union {
		struct { unsigned char	uc1, uc2, uc3, uc4; } s_uc;
		struct { unsigned short	us1, us2; } s_us;
		unsigned long s_ul;
	} s_icmp;

	unsigned long		orig_timestamp;
	unsigned long		recv_timestamp;
	unsigned long		trns_timestamp;
}ICMPheader;

typedef struct pseudo_header
{
	unsigned int src_addr; // ����� ����������� 
	unsigned int dst_addr; // ����� ���������� 
	unsigned char zero; //��������� ��������� 
	unsigned char proto; // ��������
	unsigned short length; // ����� ��������� 

}pseudo_header;



BOOL AreWeAlone(LPSTR szName);
BOOL IsCurrentUserAdmin();

unsigned short rs_crc(unsigned short * buffer, int length);
unsigned short rs_pseudo_crc(char *data, int data_length,
	unsigned int src_addr,
	unsigned int dst_addr,
	int packet_length,
	unsigned char proto);

int rs_send_ip(SOCKET s, struct IPHeader iph,
	char * data,
	int data_length,
	unsigned short dst_port_raw, unsigned int protoNum);

int rs_send_udp(SOCKET s, struct IPHeader iph, struct UDPHeader udph, unsigned char * data, int data_length);

int rs_send_tcp(SOCKET s, struct IPHeader iph, struct TCPHeader tcph, unsigned char * data, int data_length);



int rs_set_tos(SOCKET s, unsigned char new_tos);
int rs_set_raw(SOCKET s);
int sendICMPPacket(SOCKET& s, IPHeader& iph,
	ICMPheader& icmph,
	unsigned char* /* data */,
	int data_length);
unsigned short getChecksum(unsigned short * buffer, int length);


#define WM_RECV (WM_USER + 1)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)

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

unsigned short rs_crc(unsigned short * buffer, int length)
{
	unsigned long crc = 0;
	// ���������� CRC 
	while (length > 1)
	{
		crc += *buffer++;
		length -= sizeof (unsigned short);
	}
	if (length) crc += *(unsigned char*)buffer;
	// ��������� ���������� 
	crc = (crc >> 16) + (crc & 0xffff);
	crc += (crc >> 16);
	// ���������� ��������������� �������� 
	return (unsigned short)(~crc);
}


unsigned short rs_pseudo_crc(char *data, int data_length, 
							unsigned int src_addr, 
							unsigned int dst_addr,
							int packet_length,
							unsigned char proto)
{
	char * buffer;
	unsigned int full_length;
	unsigned char header_length;
	struct pseudo_header ph;
	unsigned short p_crc = 0;

	// ���������� ��������� ��������������� 
	ph.src_addr = src_addr;
	ph.dst_addr = dst_addr;
	ph.zero = 0;
	ph.proto = proto;
	ph.length = htons(packet_length);
	header_length = sizeof (struct pseudo_header);
	full_length = header_length + data_length;
	buffer = (char *)calloc(full_length, sizeof (char));

	// ��������� ��������������� 
	memcpy(buffer, &ph, header_length);
	memcpy(buffer + header_length, data, data_length);

	// ���������� CRC. 
	p_crc = rs_crc((unsigned short*)buffer, full_length);
	free(buffer);
	return p_crc;
}

int rs_send_ip(SOCKET s, struct IPHeader iph,
	char * data,
	int data_length,
	unsigned short dst_port_raw, unsigned int protoNum)
{
	char * buffer;
	int result;
	sockaddr_in target;
	unsigned char header_length;
	unsigned int packet_length;
	memset(&target, 0, sizeof (target));
	target.sin_family = AF_INET;
	target.sin_addr.s_addr = iph.iph_dest;
	target.sin_port = dst_port_raw;

	// ���������� ����� � ��������� ������ 
	header_length = sizeof (struct IPHeader);
	packet_length = header_length + data_length;

	// ��������� CRC. 
	iph.iph_xsum = 0;

	// ���������� ��������� ����� ��������� IP
	iph.iph_verlen = RS_IP_VERSION;
	iph.iph_length = htons(packet_length);
	iph.iph_protocol = protoNum;
	iph.iph_ttl = 0x80;
	iph.iph_id = htons(1234);

	// ���� ����� ������ �� ������, �� ����� ������ 
	// �������������� � ����� ��������� 
	if (!(iph.iph_verlen & 0x0F))
		iph.iph_verlen |= 0x0F & (header_length / 4);
	buffer = (char *)calloc(packet_length, sizeof (char));

	// ����������� ��������� ������ � ����� ( CRC ����� 0). 
	memcpy(buffer, &iph, sizeof (struct IPHeader));

	// ����������� ������ � ����� 
	if (data) memcpy(buffer + header_length, data, data_length);

	// ���������� CRC. 
	iph.iph_xsum = rs_crc((unsigned short *)buffer, packet_length);

	// ����������� ��������� ������ � ����� (CRC ���������). 
	memcpy(buffer, &iph, sizeof (struct IPHeader));

	// �������� IP ������ � ����.
	result = sendto(s, buffer, packet_length, 0, (struct sockaddr *)&target, sizeof (target));
	free(buffer);
	return result;
}

int sendICMPPacket(SOCKET& s, IPHeader& iph,
	ICMPHeader& icmph,
	unsigned char* /* data */,
	int data_length)
{
	int result;
	data_length = 0;

	// ���������� ���� ������ � ���������.
	const unsigned char header_length = sizeof(ICMPHeader);
	const unsigned int packet_length = header_length + data_length;
	icmph.crc = 0;
	unsigned char* buffer = (unsigned char *)calloc(packet_length, sizeof(unsigned char));

	// ����������� ��������� ������ � ����� ( CRC ����� 0).
	memcpy(buffer, &icmph, sizeof(ICMPHeader));

	// ���������� CRC.
	icmph.crc = getChecksum((unsigned short *)buffer, packet_length);

	// ����������� ��������� ������ � ����� (CRC ���������).
	memcpy(buffer, &icmph, sizeof(ICMPHeader));

	// �������� IP ������ � ��������� ICMP �������.
	result = rs_send_ip(s, iph, (char*)buffer, packet_length, 0, IPPROTO_ICMP);
	free(buffer);
	return result;
}

unsigned short getChecksum(unsigned short * buffer, int length)
{
	unsigned long crc = 0;
	// ���������� CRC 
	while (length > 1)
	{
		crc += *buffer++;
		length -= sizeof(unsigned short);
	}
	if (length) crc += *(unsigned char*)buffer;
	// ��������� ���������� 
	crc = (crc >> 16) + (crc & 0xffff);
	crc += (crc >> 16);
	// ���������� ��������������� �������� 
	return (unsigned short)(~crc);
}

int rs_send_udp(SOCKET s, struct IPHeader iph,struct UDPHeader udph,unsigned char * data,int data_length)
{
	char* buffer;
	int result;
	unsigned char header_length;
	unsigned int packet_length;

	// ���������� ���� ������ � ���������. 
	header_length = sizeof (struct IPHeader);
	packet_length = header_length + data_length;

	// ��������� CRC. 
	udph.crc = 0;

	// ���� ����� ������ �� ������, �� ����� ������ 
	// �������������� � ����� ���������
	if (!udph.length) udph.length = htons(packet_length);
	buffer = (char *)calloc(packet_length, sizeof (char));

	// ����������� ��������� ������ � ����� (CRC ����� 0). 
	memcpy(buffer, &udph, sizeof (struct UDPHeader));

	// ����������� ��������� ����� �������� ������ (������) 
	if (data) memcpy(buffer + header_length, data, data_length);

	// ���������� CRC. 
	udph.crc = rs_pseudo_crc(buffer, packet_length, iph.iph_src, iph.iph_dest, packet_length, IPPROTO_UDP);

	// ����������� ��������� ������ � ����� (CRC ���������). 
	memcpy(buffer, &udph, sizeof (struct UDPHeader));

	// �������� IP ������ � ��������� UDP �������. 
	result = rs_send_ip(s, iph, buffer, packet_length, udph.dst_port, IPPROTO_UDP);
	free(buffer);
	return result;
}


int rs_set_tos(SOCKET s, unsigned char new_tos)
{
	int tos = new_tos;
	int tos_len = sizeof (tos);
	int per = setsockopt(s, IPPROTO_IP, 3, (char *)&tos, tos_len);
	if (per == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}
	return 0;
}

int rs_set_raw(SOCKET s)
{
	unsigned int use_own_header = 1;

	// ��������� ����� RAW ��� ������, ��� ������� � ���
	// ��� �� ������� ����� ����������� ��������� �������
	if (setsockopt(s, IPPROTO_IP, 2, (char*)&use_own_header, sizeof(use_own_header)) == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}

	return 0;
}

int rs_send_tcp(SOCKET s, struct IPHeader iph, struct TCPHeader tcph, unsigned char * data, int data_length)
{
	char * buffer;
	int result;
	unsigned char header_length;
	unsigned int packet_length;

	// ���������� ���� ������ � ���������.
	header_length = sizeof (struct TCPHeader);
	packet_length = header_length + data_length;

	// ��������� CRC. 
	tcph.crc = 0;

	tcph.flags = 0x02;
	tcph.win = 1024;
	tcph.seq_n = 100;

	// ��������� ���� offset .
	tcph.offset = 0xF0 & ((header_length / 4) << 4);
	buffer = (char *)calloc(packet_length, sizeof (char));

	// ����������� ��������� ������ � ����� ( CRC ����� 0). 
	memcpy(buffer, &tcph, sizeof (struct TCPHeader));

	// ����������� ��������� ����� �������� ������ (������) 
	if (data) memcpy(buffer + header_length, data,
		data_length);

	// ���������� CRC. 
	tcph.crc = rs_pseudo_crc(buffer, packet_length,
		iph.iph_src,
		iph.iph_dest,
		packet_length,
		IPPROTO_TCP);

	// ����������� ��������� ������ � ����� (CRC ���������).
	memcpy(buffer, &tcph, sizeof (struct TCPHeader));

	// ������� IP ������ � ��������� TCP ������� 
	result = rs_send_ip(s, iph, buffer, packet_length,
		tcph.dst_port, IPPROTO_TCP);

	free(buffer);
	return result;
}

#include<stdio.h>
#include<Winsock2.h>
#include<ws2tcpip.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>
#pragma comment(lib , "Ws2_32.lib")
#define ICMP_ECHO_REQUEST 8 //定义回显请求类型
#define DEF_ICMP_DATA_SIZE 20 //定义发送数据长度
#define DEF_ICMP_PACK_SIZE 32 //定义数据包长度
#define MAX_ICMP_PACKET_SIZE 1024 //定义最大数据包长度
#define DEF_ICMP_TIMEOUT 3000  //定义超时为3秒
#define ICMP_TIMEOUT 11 //ICMP超时报文
#define ICMP_ECHO_REPLY 0 //定义回显应答类型

typedef struct
{
	byte h_len_ver; 
	byte tos; 
	unsigned short total_len; 
	unsigned short ident; 
	unsigned short frag_and_flags; 
	byte ttl; 
	byte proto; 
	unsigned short cksum; 
	unsigned long sourceIP; 
	unsigned long destIP; 
} IP_HEADER;

typedef struct _ICMP_HEADER
{
	byte type; 
	byte code; 
	unsigned short cksum; 
	unsigned short id; 
	unsigned short seq; 
	unsigned int choose; 
} ICMP_HEADER;
typedef struct
{
	int usSeqNo; 
	DWORD dwRoundTripTime; 
	byte ttl; 
	in_addr dwIPaddr; 
} DECODE_RESULT;
unsigned short GenerateChecksum(unsigned short* pBuf, int iSize)
{
	unsigned long cksum = 0; 
	while (iSize > 1)
	{
		cksum += *pBuf++; 
		iSize -= sizeof(unsigned short); 
	}
	if (iSize)
	{
		cksum += *(unsigned char*)pBuf;
	}
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);
	return (unsigned short)(~cksum);
}
boolean DecodeIcmpResponse_Ping(char* pBuf, int iPacketSize, DECODE_RESULT* stDecodeResult)
{
	IP_HEADER* pIpHrd = (IP_HEADER*)pBuf;
	int iIphedLen = 20;
	if (iPacketSize < (int)(iIphedLen + sizeof(ICMP_HEADER)))
	{
		printf("size error! \n");
		return 0;
	}
	ICMP_HEADER* pIcmpHrd = (ICMP_HEADER*)(pBuf + iIphedLen);
	unsigned short usID, usSeqNo;
	if (pIcmpHrd->type == ICMP_ECHO_REPLY)
	{
		usID = pIcmpHrd->id;
		usSeqNo = ntohs(pIcmpHrd->seq);
	}
	if (usID != GetCurrentProcessId() || usSeqNo != stDecodeResult->usSeqNo)
	{
		printf("usID error!\n");
		return 0;
	}
	if (pIcmpHrd->type == ICMP_ECHO_REPLY)
	{
		stDecodeResult->dwIPaddr.s_addr = pIpHrd->sourceIP;
		stDecodeResult->ttl = pIpHrd->ttl;
		stDecodeResult->dwRoundTripTime = GetTickCount() - stDecodeResult->dwRoundTripTime;
		return 1;
	}
	return 0;
}
void Ping(char* IP)
{
	int respnum=0;
	unsigned long ulDestIP = inet_addr(IP); 
	if (ulDestIP == INADDR_NONE)
	{
		HOSTENT* pHostent = gethostbyname(IP);
		if (pHostent == nullptr) printf("fuckup");
		if (pHostent)
		{
			in_addr addr;
			addr.S_un.S_addr = *(ULONG*)pHostent->h_addr;
			ulDestIP = inet_addr(inet_ntoa(addr)); 
		}
		else
		{
			printf("地址解析失败！\n");
			return;
		}
	}
	SOCKADDR_IN destSockAddr; 
	ZeroMemory(&destSockAddr, sizeof(SOCKADDR_IN)); 
	destSockAddr.sin_family = AF_INET;
	destSockAddr.sin_addr.s_addr = ulDestIP;
	destSockAddr.sin_port = htons(0);
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		printf("初始化WinSock失败！\n");
		return;
	}
	SOCKET sockRaw = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (sockRaw == INVALID_SOCKET)
	{
		printf("创建Socket失败 !\n");
		return;
	}
	int iTimeout = DEF_ICMP_TIMEOUT;
	if (setsockopt(sockRaw, SOL_SOCKET, SO_RCVTIMEO, (char*)&iTimeout, sizeof(iTimeout)) == SOCKET_ERROR)
	{
		printf("设置参数失败！\n");
		return;
	}
	if (setsockopt(sockRaw, SOL_SOCKET, SO_SNDTIMEO, (char*)&iTimeout, sizeof(iTimeout)) == SOCKET_ERROR)
	{
		printf("设置参数失败！\n");
		return;
	}
	char IcmpSendBuf[DEF_ICMP_PACK_SIZE];
	ICMP_HEADER* pIcmpHeader = (ICMP_HEADER*)IcmpSendBuf;
	pIcmpHeader->type = ICMP_ECHO_REQUEST;
	pIcmpHeader->code = 0;
	pIcmpHeader->id = (unsigned short)GetCurrentProcessId();
	memset(IcmpSendBuf + sizeof(ICMP_HEADER), 'E', DEF_ICMP_DATA_SIZE);
	int usSeqNo = 0;
	DECODE_RESULT stDecodeResult;
	while (usSeqNo <= 3)
	{
		pIcmpHeader->seq = htons(usSeqNo);
		pIcmpHeader->cksum = 0;
		pIcmpHeader->cksum = GenerateChecksum((unsigned short*)IcmpSendBuf, DEF_ICMP_PACK_SIZE); //生成校验位
		stDecodeResult.usSeqNo = usSeqNo;
		stDecodeResult.dwRoundTripTime = GetTickCount();
		if (sendto(sockRaw, IcmpSendBuf, DEF_ICMP_PACK_SIZE, 0, (SOCKADDR*)&destSockAddr, sizeof(destSockAddr)) == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEHOSTUNREACH)
			{
				printf("目的主机不可达！\n");
				exit(0);
			}
		}
		SOCKADDR_IN from;
		int iFromLen = sizeof(from);
		int iReadLen;
		char IcmpRecvBuf[MAX_ICMP_PACKET_SIZE];
		while (1)
		{
			iReadLen = recvfrom(sockRaw, IcmpRecvBuf, MAX_ICMP_PACKET_SIZE, 0, (SOCKADDR*)&from, &iFromLen);
			if (iReadLen != SOCKET_ERROR)
			{
				if (DecodeIcmpResponse_Ping(IcmpRecvBuf, sizeof(IcmpRecvBuf), &stDecodeResult))
				{
					printf("来自 %s 的回复: 字节 = %d 时间 = %dms TTL = %d\n", inet_ntoa(stDecodeResult.dwIPaddr),
						iReadLen - 20, stDecodeResult.dwRoundTripTime, stDecodeResult.ttl);
					respnum++;
				}
				break;
			}
			else if (WSAGetLastError() == WSAETIMEDOUT)
			{
				printf("“Request time out\n");
				break;
			}
			else
			{
				printf("发生未知错误！\n");
				break;
			}
		}
		usSeqNo++;
	}
	printf("   数据包: 已发送 = % d，已接收 = % d，丢失 = % d(% d % %丢失)，\n", usSeqNo,respnum,usSeqNo-respnum, (usSeqNo - respnum)*100/usSeqNo);
	closesocket(sockRaw);
	WSACleanup();
}
int main(int argc, char* argv[])
{
	WORD version = MAKEWORD(2, 2);
	WSADATA wd = { 0 };
	WSAStartup(version, &wd);
	char  com[10], IP[20];
	while (1) {
		printf("command>>");
		scanf("%s %s", com, IP);
		if (strcmp(com, "ping") == 0)
		{
			Ping(IP);
		}
		else
		{
			printf("command error ! \n");
		}
	}
	return 0;
}

#pragma comment (lib, "ws2_32.lib")
#include <stdio.h>
#include <WinSock2.h>
#include <process.h>

int main()
{
	//-------���� ���̺귯�� �ҷ�����(?)--------
	WSADATA wsaData;
	int retval = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (retval != 0)
	{
		printf("WSAStartup() Error\n");
		return 0;
	}
	//------------------------------------------

	//---------���ϻ���-------- 
	SOCKET serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	//TCP�� �̿��� ����
																	//-------------------------

																	//---------���� ���� �Է�--------------------
	SOCKADDR_IN serv_addr;
	serv_addr.sin_family = AF_INET;						// IP�ּҸ� �̿��ϰ�
	serv_addr.sin_port = htons(4000);					// ������ 4000����
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");	// ������ ip �ּҴ� 127.0.0.1
														//--------------------------------------------

														//---------���� ����------------
	retval = connect(serv_sock, (SOCKADDR*)&serv_addr, sizeof(SOCKADDR));
	if (retval == SOCKET_ERROR)
	{
		printf("connect() Error\n");
		return 0;
	}
	//-------------------------------

	while (1)
	{
		char buf[255] = { 0 };
		printf(">> ");
		gets(buf);
		//---------������ �޽��� ����---------------
		int sendsize = send(serv_sock, buf, strlen(buf), 0);
		if (sendsize <= 0)
			break;
		//------------------------------------------

		//---------�����κ��� �޽��� ����-----------
		int recvsize = recv(serv_sock, buf, sizeof(buf), 0);
		if (recvsize <= 0)
		{
			printf("��������\n");
			break;
		}
		//-------------------------------------------
		buf[recvsize] = '\0';
		printf("server >> %s\n", buf);
	}

	//----------���� ����------------------
	closesocket(serv_sock);
	//-------------------------------------

	//---------���̺귯�� ����(?)---------
	WSACleanup();
	//-------------------------------------
}
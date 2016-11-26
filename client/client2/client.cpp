#pragma comment (lib, "ws2_32.lib")
#include <stdio.h>
#include <WinSock2.h>
#include <process.h>

int main()
{
	//-------소켓 라이브러리 불러오기(?)--------
	WSADATA wsaData;
	int retval = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (retval != 0)
	{
		printf("WSAStartup() Error\n");
		return 0;
	}
	//------------------------------------------

	//---------소켓생성-------- 
	SOCKET serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	//TCP를 이용한 소켓
																	//-------------------------

																	//---------서버 정보 입력--------------------
	SOCKADDR_IN serv_addr;
	serv_addr.sin_family = AF_INET;						// IP주소를 이용하고
	serv_addr.sin_port = htons(4000);					// 소켓은 4000번에
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");	// 서버의 ip 주소는 127.0.0.1
														//--------------------------------------------

														//---------서버 연결------------
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
		//---------서버에 메시지 전송---------------
		int sendsize = send(serv_sock, buf, strlen(buf), 0);
		if (sendsize <= 0)
			break;
		//------------------------------------------

		//---------서버로부터 메시지 수신-----------
		int recvsize = recv(serv_sock, buf, sizeof(buf), 0);
		if (recvsize <= 0)
		{
			printf("접속종료\n");
			break;
		}
		//-------------------------------------------
		buf[recvsize] = '\0';
		printf("server >> %s\n", buf);
	}

	//----------소켓 닫음------------------
	closesocket(serv_sock);
	//-------------------------------------

	//---------라이브러리 해제(?)---------
	WSACleanup();
	//-------------------------------------
}
#include "CafeConnectionManager.h"
#include "StatusUpdater.h"
#include <iostream>
CafeConnectionManager* CafeConnectionManager::instance = nullptr;
CafeConnectionManager * CafeConnectionManager::GetInstance()
{
	if (instance)
		return instance;
	else
		return instance = new CafeConnectionManager();
}

bool CafeConnectionManager::RequestCardUsage(int card_num)
{
	char message[100];
	char buffer[100];
	int recv_size;
	snprintf(message, 100, "rcard     %d", card_num);
	send(management_sock, message, strlen(message) + 1, 0);
	recv_size = recv(management_sock, buffer, 100, 0);
	return buffer[0] != '0';
}

CafeConnectionManager::CafeConnectionManager()
{
	//-------소켓 라이브러리 불러오기(?)--------
	//WSADATA wsaData; it is declared in header file.
	int retval = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (retval != 0)
	{
		printf("WSAStartup() Error\n");
	}
	//------------------------------------------

	//---------소켓생성-------- 
	// serv_sock is declared in header file as well.
	management_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);   //TCP를 이용한 소켓
																   //-------------------------

																   //---------서버 정보 입력--------------------
   //SOCKADDR_IN serv_addr; it is declared in header file
	serv_addr.sin_family = AF_INET;                  // IP주소를 이용하고
	serv_addr.sin_port = htons(80);               // 소켓은 4000번에
	serv_addr.sin_addr.s_addr = inet_addr(fixed_management_pc_ip);   // 서버의 ip 주소는 127.0.0.1
														  //--------------------------------------------

														  //---------서버 연결------------
	retval = connect(management_sock, (SOCKADDR*)&serv_addr, sizeof(SOCKADDR));
	if (retval == SOCKET_ERROR)
	{
		printf("connect() Error\n");
	}
	//implement listener thread
   //listener = std::thread([]() {});
	listener = std::thread(
		[](SOCKET server)
	{
		// message form is like this
		// "update    %10.0fcard 1....."
		// or
		// "update    %10.0fUser ID : Kim tong tong......"
		char message[100];
		int recv_size;

		while (true)
		{

			recv_size = recv(server, message, 100, 0);
			if (recv_size < 0)
			{
				std::cout << "something ain't right!\n";
				continue;
			}
			if (strncmp(message, "update    ", 10) == 0)
			{
				char *Userinfo;
				float left_time_secs;
				left_time_secs = strtof(message + 10, &Userinfo);
				StatusUpdater::GetInstance()->UpdateStatus(Userinfo, left_time_secs);
			}
		}
	}, management_sock
		);
	
}

CafeConnectionManager::~CafeConnectionManager()
{
	//----------소켓 닫음------------------
	closesocket(management_sock);
	//-------------------------------------

	//---------라이브러리 해제(?)---------
	WSACleanup();
	//-----------------------------------
}

bool CafeConnectionManager::Send_order(int order, const int& PC_number)
{
	char message[14];
	snprintf(message, 14, "order     %1d%2d", order, PC_number);
	send(management_sock, message, 15, 0);
	return true;
}

std::string CafeConnectionManager::Check_Time(int info_number)
{
	std::string program;
	program = "LOL.exe";

	return program;
}

std::string CafeConnectionManager::Check_Status()
{
	char message[100] =  "status    " ;
	char response[100];
	char* user_info;
	char* left_time;
	static char status[100];
	send(management_sock, message, 100, 0);
	recv(management_sock, response, 100, 0);
	/*
	response format is like this.
	User_Info | Left Time
	*/
	user_info = response;
	for (left_time = response; *left_time != '|'; left_time++);
	snprintf(status,100,"User Information : %s\nLeft Time : %sm %ss\n");
	
	return status;
}

bool CafeConnectionManager::Send_program(int count, int number, std::string program)
{
	return true;
}

bool CafeConnectionManager::Quit_program(int Program_Num, int info)
{

	return true;
}

std::string CafeConnectionManager::get_program(int Program_Num, int info)
{
	std::string program;
	program = "LOL.exe";

	return program;
}
bool CafeConnectionManager::StopUsing(int pc_num) {
	char message[100];
	char buffer[100];
	snprintf(message, 100, "s|%d", pc_num);
	send(management_sock, message, (int)strlen(message), 0);
	//발신	return true;
	recv(management_sock, buffer, 100, 0);
	return buffer[0] != '0';
}
bool CafeConnectionManager::Report(bool is_starting, int pc_num)
{
	char message[100];
	char buffer[100];
	int send_ret_val;
	if (is_starting)
		snprintf(message, 100, "report    1%d", pc_num);
	else
		snprintf(message, 100, "report    0%d", pc_num);
	send_ret_val = send(management_sock, message, 100, 0);
	//printf("send : %d\n", send(management_sock, message, (int)strlen(message), 0));//발신	return true;
	//printf("recv : %d\n", recv(management_sock, buffer, 100, 0));
	/*
	send(management_sock, message, strlen(message) + 1, 0);
	recv(management_sock, buffer, 100, 0);
	*/
	return buffer[0] != '0';
}
bool CafeConnectionManager::Register(char * name, char * age, char * phonenum, char * id, char * passwd, char* question, char* psw_answer, char* email_address)
{
	char message[1024];
	char buffer[1024];
	snprintf(message, 1023, "m|%s|%s|%s|%s|%s|%s|%s|%s", name, age, phonenum, id, passwd, question, psw_answer,email_address);
	//printf("[client] : ");
	//scanf("%s", say);

	send(management_sock, message, 1024, 0);//발신
	//send(management_sock, message, (int)strlen(message), 0);//발신

													   /* message : 서버로부터 받아온 값
													   strleng : 서버로부터 받아온 값의 길이 */
	int strleng;
	// it receives '0' or '1'
	strleng = recv(management_sock, buffer, 100, 0);//수신	
	//strleng = recv(management_sock, buffer, sizeof(message) - 1, 0);//수신

	return buffer[0] != '0';
}
bool CafeConnectionManager::Login(const std::string& ID, const std::string& password)
{
	char message[1024];
	char buffer[101];
	strcpy(message, ("login     " + ID + ";" + password + ";").c_str());
	if (send(management_sock, message, 1023, 0) == -1)
		printf("Send error!\n");
	int recvleng = recv(management_sock, buffer, 100, 0);
	//recvleng = recv(management_sock,buffer,100,0);

	if (recvleng == -1)
		printf("Receiving error!\n");
	return buffer[0] != '0';
}

void CafeConnectionManager::Send_chat(std::string nick)
{
	while (1)
	{
		char buf[256] = { 0 };
		char str[256]; // 문자열
		int size = 0;
		printf(">> ");
		gets_s(str); // 받아들임 
		sprintf(buf, "[%s] %s", nick, str);
		size = strlen(buf);
		//---------서버에 메시지 전송---------------
		int sendsize = send(management_sock, (char*)&size, sizeof(int), 0);
		sendsize = send(management_sock, buf, size, 0);
		if (sendsize <= 0)
			break;
		//------------------------------------------
	}
}

void __cdecl CafeConnectionManager::RecvThread(void * p) // 리시브 스레드. 받는 스레드져.
{
	SOCKET sock = (SOCKET)p;
	char buf[256];
	int size;
	while (1)
	{
		//-----------서버로부터 수신------------
		int recvsize = recv(sock, (char*)&size, sizeof(int), 0);
		recvsize = recv(sock, buf, size, 0);
		if (recvsize <= 0)
		{
			printf("접속종료\n");
			break;
		}
		//------------------------------------------------
		buf[recvsize] = '\0';
		printf("\r%s\n>>", buf);
	}
}
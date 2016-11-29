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
	return true;
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
	serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);   //TCP를 이용한 소켓
																   //-------------------------

																   //---------서버 정보 입력--------------------
   //SOCKADDR_IN serv_addr; it is declared in header file
	serv_addr.sin_family = AF_INET;                  // IP주소를 이용하고
	serv_addr.sin_port = htons(4000);               // 소켓은 4000번에
	serv_addr.sin_addr.s_addr = inet_addr(fixed_management_pc_ip);   // 서버의 ip 주소는 127.0.0.1
														  //--------------------------------------------

														  //---------서버 연결------------
	retval = connect(serv_sock, (SOCKADDR*)&serv_addr, sizeof(SOCKADDR));
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
	}, serv_sock
		);

}

CafeConnectionManager::~CafeConnectionManager()
{
	//----------소켓 닫음------------------
	closesocket(serv_sock);
	//-------------------------------------

	//---------라이브러리 해제(?)---------
	WSACleanup();
	//-----------------------------------
}

bool CafeConnectionManager::Send_order(int order, const int& PC_number)
{
	char message[14];
	snprintf(message, 14, "order     %1d%2d", order, PC_number);
	send(serv_sock, message, 15, 0);
	return true;
}

std::string CafeConnectionManager::Check_Time(int info_number)
{
	std::string program;
	program = "LOL.exe";

	return program;
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
	send(serv_sock, message, (int)strlen(message), 0);//발신	return true;
}
bool CafeConnectionManager::Report(bool is_turning_on)
{


	if (is_turning_on)
		send(serv_sock, "report    1", 12, 0);
	else
		send(serv_sock, "report    0", 12, 0);
	return true;
}
bool CafeConnectionManager::Register(char * name, char * age, char * phonenum, char * id, char * passwd, char* question, char* psw_answer)
{
	char message[100];
	char buffer[100];
	snprintf(message, 100, "m|%s|%s|%s|%s|%s|%s|%s", name, age, phonenum, id, passwd, question, psw_answer);
	//printf("[client] : ");
	//scanf("%s", say);
	send(serv_sock, message, (int)strlen(message), 0);//발신

													   /* message : 서버로부터 받아온 값
													   strleng : 서버로부터 받아온 값의 길이 */
	int strleng = recv(serv_sock, buffer, sizeof(message) - 1, 0);//수신

	return buffer[0] != '0';
}
bool CafeConnectionManager::Login(const std::string& ID, const std::string& password)
{
	char message[100];
	strcpy(message, ("login     " + ID + ";" + password + ";").c_str());
	send(serv_sock, message, 99, 0);
	return true;
}

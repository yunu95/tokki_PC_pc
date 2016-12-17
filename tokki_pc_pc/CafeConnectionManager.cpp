#include "CafeConnectionManager.h"
#include "StatusUpdater.h"
#include <iostream>
#include "sha256.h"
CafeConnectionManager* CafeConnectionManager::instance = nullptr;
CafeConnectionManager * CafeConnectionManager::GetInstance()
{
	if (instance)
		return instance;
	else
		return instance = new CafeConnectionManager();
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
	//implement listener 


	//listener = std::thread([Capture](Parameter) {Body});
	// Capture : 람다 함수 밖에 있는 변수를 가져올때 여기다 선언.
	// Parameter : 말 그대로 파라미터. 인자.
	listener = std::thread( // 리스너 스레드는 서버로부터 오는 응답을 계속 받아들이는 스레드.
		[](SOCKET server) // 파라미터로 서버 소켓을 받음 
	{ // body
	  // message form is like this
	  // "update    %10.0fcard 1....."
	  // or
	  // "update    %10.0fUser ID : Kim tong tong......"
		static char message[100];
		int recv_size;

		while (true)
		{

			recv_size = recv(server, message, 100, 0);
			if (recv_size < 0) // 서버(매니저)에서 아무런 연락이 오지 X
			{
				std::cout << "something ain't right!\n";
				exit(0); // 매니저가 아예 안띄워져 있을때
			}
			if (strncmp(message, "update    ", 10) == 0)
			{
				char *Userinfo = message + 10;
				float left_time_secs;
				left_time_secs = strtof(message + 10, &Userinfo);
				StatusUpdater::GetInstance()->UpdateStatus(Userinfo, left_time_secs);
			}
		}
	}, management_sock
	);

	RecvThread = std::thread( // 리시브스레드는 채팅용 스레드
		[](SOCKET server) // 파라미터로 서버 소켓을 받음
	{ // body

		char buf[256];
		int size;
		while (1)
		{
			//-----------서버로부터 수신------------
			int recvsize = recv(server, (char*)&size, sizeof(int), 0);
			recvsize = recv(server, buf, size, 0);
			if (recvsize <= 0)
			{
				break;
			}
			//------------------------------------------------
			buf[recvsize] = '\0';
			printf("\r%s\n>>", buf);
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
{//***음식주문
	char message[14];
	snprintf(message, 14, "o|%d|%d", order, PC_number);
	send(management_sock, message, 15, 0);
	return true;
}

std::string CafeConnectionManager::Check_Status()
{//***현재상태
	char message[100];
	char response[100];
	char* user_info;
	char* left_time;
	char lefttime[10];
	static char status[100];

	time_t curtime;
	struct tm *curtm;
	curtime = time(NULL);
	curtm = localtime(&curtime);//현재시간을 시간구조체에 넣음

								/*status확인 버튼을 눌렀을 당시의 시간*/
	std::string endtime = std::to_string(curtm->tm_hour) + ":" + std::to_string(curtm->tm_min);

	snprintf(message, 100, "cs|%s", endtime.c_str());

	send(management_sock, message, 100, 0);
	recv(management_sock, response, 100, 0);
	/*
	response format is like this.
	User_Info | Left Time
	*/
	user_info = strtok(response, "|");
	left_time = strtok(NULL, "|");//남은시간

	int lt = atoi(left_time);
	int min = lt % 60;
	int hour = lt / 60;
	sprintf(lefttime, "%d:%d", hour, min);
	snprintf(status, 100, "User ID : %s\nLeft Time : %s\n", user_info, lefttime);

	return status;
}

bool CafeConnectionManager::StopUsing(int pc_num)
{//종료
	char message[100];
	char buffer[100];

	system("CLS"); // 화면을 지운다.

	time_t curtime;
	struct tm *curtm;
	curtime = time(NULL);
	curtm = localtime(&curtime);//현재시간을 시간구조체에 삽입

								/*종료했을당시의 시간*/
	std::string endtime = std::to_string(curtm->tm_hour) + ":" + std::to_string(curtm->tm_min);

	snprintf(message, 100, "s|%s", endtime.c_str());
	send(management_sock, message, (int)strlen(message), 0);
	//발신	return true;
	return true;
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

	return buffer[0] != '0';
}
bool CafeConnectionManager::Register(char * name, char * age, char * phonenum, char * id, char * passwd, char* question, char* psw_answer, char* email_address)
{//회원가입
	char message[1024];
	char buffer[1024];
	snprintf(message, 1023, "m|%s|%s|%s|%s|%s|%s|%s|%s", name, age, phonenum, id, passwd, question, psw_answer, email_address);
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
int CafeConnectionManager::Login(const std::string& ID, const std::string& password)
{//로그인
	char message[1024];
	char buffer[100];

	strcpy(message, ("login     " + ID + ";" + password + ";").c_str());

	if (send(management_sock, message, 1023, 0) == -1)
		printf("Send error!\n");

	recv(management_sock, buffer, 100, 0);


	if (strcmp(buffer, "e") == 0 || strcmp(buffer, "false") == 0)
	{
		return 0;//로그인실패
	}
	else
	{
		char m[100];
		char b[100];

		sprintf(m, "lp|%s", (char*)ID.c_str());
		send(management_sock, m, 100, 0);
		recv(management_sock, b, 100, 0);

		if (strcmp(b, "0") == 0)
			return 2;//남은시간이 0
		return 1;//정상로그인
	}
}
void CafeConnectionManager::ChangePsw(char*id, char*psw, char*cpsw, int* check)
{//비밀번호 변경
	char message[1024];
	char buf[100] = { "" };
	std::string p = psw;
	std::string cp = cpsw;

	/*비밀번호와 바꿀비밀번호가 모두 암호화됨*/
	snprintf(message, 1024, "c|%s|%s|%s", id, (char*)sha256(p).c_str(), (char*)sha256(cp).c_str());
	send(management_sock, message, (int)strlen(message), 0);
	int recvleng = recv(management_sock, buf, 100, 0);

	if (recvleng == -1)
		printf("Receiving error!\n");

	if (buf[0] == '0')
		*check = 0;//실패
	else
		*check = 1;//성공
}
char* CafeConnectionManager::Question(char*id)
{//비밀번호확인질문
	char message[1024];
	char buf[100] = { "" };

	snprintf(message, 1024, "fa|%s", id);
	send(management_sock, message, (int)strlen(message), 0);
	int recvleng = recv(management_sock, buf, 100, 0);

	return buf;
}
bool CafeConnectionManager::Answer(char*id, char*answer)
{//비밀번호확인질문 답
	char message[1024];
	char buf[100] = { "" };

	snprintf(message, 1024, "fr|%s|%s", id, answer);
	send(management_sock, message, (int)strlen(message), 0);

	int recvleng = recv(management_sock, buf, 100, 0);

	if (buf[0] == '0')
	{
		printf("답이 틀렸습니다!\n");
		return false;
	}
	else
	{
		printf("임시 비밀번호를 귀하의 이메일로 보냈습니다.\n로그인 하신 뒤, 비밀번호를 변경해주세요!!\n\n");
		return true;
	}
}

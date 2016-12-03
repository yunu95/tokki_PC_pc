/*
this class is where the terminal function is located.
it interprets user's command and does what it's intended to do.
yunu Lee
2016.11.07
-------------------------
현재 로그인까지 구현된 상태. - 회원가입도 구현해야 함.
로그인 뒤 - 명령어를 입력받는 것까지.
그리고 음식메뉴, 요금제 표 작성.
Red.
2016. 11. 18
------------------------

*/
#include <iostream>
#include "StatusUpdater.h"
#include "ServerConnectionManager.h"
#include "CafeConnectionManager.h"
#include <string>
#include <windows.h>
#include "sha256.h"

// 싱글톤 static private instance
StatusUpdater* StatusUpdater::instance = nullptr;
StatusUpdater::StatusUpdater()
{

}

StatusUpdater::~StatusUpdater()
{

}
void StatusUpdater::UpdateStatus(char* user_info, float left_seconds)
{
	strcpy(this->user_info, user_info);
	this->left_time_secs = left_seconds;
}
void StatusUpdater::StartUsing()
{
	//updater_thread.join();
	system("CLS"); // 화면을 지운다.
	// QueryAction returns false when StopUsing command gets typed.
	QueryAction();
	return;
}
void StatusUpdater::StopUsing()
{

}
// 싱글톤 static private instance를 참조하기 위해 쓰이는 정적 메서드
StatusUpdater* StatusUpdater::GetInstance()
{
	if (!instance)
		instance = new StatusUpdater();
	return instance;
}

bool StatusUpdater::Register()
{
	//m | (name) | (age) | (phonenum) | (id) | (password)| (psw_question) | (psw_answer)
	char name[50];
	char age[50];
	char phonenum[50];
	char id[100];
	char password[200];
	char question[300];
	char answer[200];
	char email_address[100];
	cout << "What! is your name?\n";
	cin.getline(name, INT_MAX);
	cout << "How! old are you??\n";
	cin.getline(age, INT_MAX);
	cout << "What! is your cell phone number?\n";
	cin.getline(phonenum, INT_MAX);
	cout << "Type! your id.\n";
	cin.getline(id, INT_MAX);
	cout << "Type! your password.\n";
	cin.getline(password, INT_MAX);
	std::string temp;
	temp = sha256(std::string(password));
	strcpy(password, temp.c_str());
	cout << "Type! the question for your password.\n";
	cin.getline(question, INT_MAX);
	cout << "Type! the answer for your password.\n";
	cin.getline(answer, INT_MAX);
	cout << "Type! the Email address of yours.\n";
	cin.getline(email_address, INT_MAX);
	if (CafeConnectionManager::GetInstance()->Register(name, age, phonenum, id, password, question, answer,email_address))
		if (CafeConnectionManager::GetInstance()->Login(string(id), string(password)))
			return true;
	cout << " what you typed, just don't work.\n";
	return false;
}
bool StatusUpdater::QueryValidation()
{

	while (true) {
		std::string Authorization_type;

		cout << "\nPlease Type card, or id to get authorized.\ntype register if you want to.\n";
		std::getline(std::cin, Authorization_type);
		std::cin.clear();

		if (Authorization_type == "card")
		{
			//if validation succeeds, it breaks out.
			if (validate_card())
				break;
		}
		else if (Authorization_type == "id")
		{
			// same here. it breaks out in case validation succeeds.
			if (validate_ID())
				break;
		}
		else if (Authorization_type == "register")
		{
			// same here. it breaks out in case validation succeeds.
			if (Register())
				break;
		}
		else
		{
			cout << "Wrong input\n";
		}

		//std::cin.ignore(INT_MAX,'\n');

	}
	return true;
}
bool StatusUpdater::QueryAction()
{
	int Program_Num = 0;
	int info = 0;

	while (true)
	{


		printOptions();
		std::string input;
		std::getline(cin, input);
		cin.clear();
		// for the convenience,
		for (string::iterator i = input.begin(); i < input.end(); i++)
		{
			*i = tolower(*i);
		}
		// 0.Check status
		if (input == "c")
		{
			std::cout << CafeConnectionManager::GetInstance()->Check_Status();
			continue;
		}
		// 1.StopUsing 
		if (input == "s") {
			StopUsing();
			continue;
		}
		//2.Orderingfood.
		if (input == "f") {

			int Order_Number = 0;
			int PC_number = 0;

			// 먼저 음식표 출력되고 - 사용자가 선택하도록 요구함.

			cout << "주문할 음식의 번호를 입력하세요" << endl;
			cout << \
				"-------------------\n\
					\n\
1.내가 토끼라면 (2000원).\n\
2.토끼간 순대 (2000원)\n\
3.산토끼 정식 (5000원) \n\
4.콜-라 (1000원)\n\
5.토끼 불고기버거 (1500원)\n\
6.비타민워터 (1200원)\n\
7.토끼에게 맡겨봐 - 랜덤으로 위 음식 중 하나가 주문됩니다. (2000원)\n\
					\n\
-------------------- \n\n"
<< endl;

			cin >> Order_Number;
			cout << "PC의 번호를 입력하세요" << endl;
			cin >> PC_number;
			// PC 번호와 주문 값을 받아서 매니저에 보낸다. (소켓 프로그래밍)

			if (CafeConnectionManager::GetInstance()->Send_order(Order_Number, PC_number))
			{
				// 아래 문구는 매니저에 성공적으로 전달되었을 때, 출력되는 문구.
				cout << "주문이 완료 되었습니다." << endl;
			}
			continue;
		}


		//3.Print Left Time.
		if (input == "t") {

			int number = 0;
			//사용자 정보를 입력받아 남은 시간을 출력하도록 한다.
			cout << "id 나 번호를 입력하세요." << endl;
			cin >> number;
			cout << "남은 사용시간은 :";
			CafeConnectionManager::GetInstance()->Check_Time(number);
			continue;
		}

		//4.Running program.
		if (input == "p") {
			string program;
			info = 0;

			// 프로그램 실행.
			// 대안 1. - 키보드 후킹(키로깅)
			// 대안 2. - 사용자가 직접 입력함. / 이 경우 프로그램명 입력을 받고 실행중 문구를 띄움.

			cout << "실행할 프로그램 이름을 입력하세요 :";
			cin >> program;
			cout << "id 나 번호를 입력하세요." << endl;
			cin >> info;
			// 저장은 스택을 사용하거나 번호를 같이 넘겨서 큰 수부터 삭제.
			if (CafeConnectionManager::GetInstance()->Send_program(Program_Num, info, program)) {
				cout << program << " is running… " << endl;
				Program_Num++;
			}
			continue;
		}
		//5.Quitting program.
		if (input == "q") {
			string dead_program;
			//info의 Program_Num 째 프로그램 (가장 최근의 프로그램)을 종료

			if (CafeConnectionManager::GetInstance()->Quit_program(Program_Num, info) && Program_Num > 0) {
				string dead_program = CafeConnectionManager::GetInstance()->get_program(Program_Num, info);
				cout << dead_program << "is closed" << endl;
				Program_Num--;
			} // 잘되면 프로그램이 닫혔다고 출력하고 넘버를 하나 줄임.
			else
				cout << "Error!" << endl;
			return true;
		}
		//6.Random chatting.
		if (input == "r") {
			// 랜덤 채팅입니다. 클라이언트마다 접속 포트가 다른데, 두 포트(or id?)를 랜덤으로 매칭시킵니다.
			// 먼저 사용할 닉네임을 받아야 하는데요, 이건 자기 아이디를 받아와서 하면 되겠죠. 아님 입력하거나.
			
			char nick[20];
			printf("닉네임 >> "); // 닉네임 -> 아이디로
			gets_s(nick); // 닉네임을 받음
			
			CafeConnectionManager::GetInstance()->Send_chat(nick); 
			// 클라-클라간 랜덤 채팅을 구현하려면, 서버는 당연히 거쳐야 합니다!
			// 따라서 접속 후 - 랜챗 메뉴 선택 - 매니저에 메시지를 보내면 매니저가 그걸 다른 곳에 보내 주면 됩니다.
			// 여기서 둘 다 랜챗 모드 상태여야 하겠죠. 만약 랜챗에 들어와 있는게 자기 혼자라면, 자기 자신과 연결되는 것을 방지하기 위한 것도 있어야죠.
			// 둘 중 하나라도 나간다면, "대화가 끊겼습니다." or xx님이 나가셨습니다. 를 출력.
			// 여기서 접속 종료를 실행하면, 서버에 알려야겠죠.
			// "새로운 상대를 찾으시겠습니까?" 를 출력합니다.
			// 아니라면 랜챗 모드를 나가고, 메뉴가 다시 출력되도록 합니다.
	
			return true;
		}
	}
	return false;
}
void StatusUpdater::AbortUsing()
{

}
bool StatusUpdater::validate_ID()
{
	std::string input_id;
	std::string input_password;
	while (true)
	{
		std::cout << "ID please. Enter Q to quit\n";
		std::getline(std::cin, input_id);
		std::cin.clear();
		if (input_id == "q" || input_id == "Q") {
			return false;
		}
		std::cout << "and password\n";
		std::getline(std::cin, input_password);
		std::string temp;
		temp = sha256(std::string(input_password));
		input_password = temp;
		// function called in if parameter returns true when login function succeeds.
		if (CafeConnectionManager::GetInstance()->Login(input_id, input_password))
			break;
		else
			cout << "Wrong ID or password. try again\n\n";
	}
	return true;
}
bool StatusUpdater::validate_card()
{
	while (true)
	{
		int input_number;
		std::string input;
		std::cout << "enter cardnumber please. Enter Q to quit\n";
		std::getline(cin, input);
		std::cin.clear();
		if (input == "Q" || input == "q")
			return false;
		input_number = atoi(input.c_str());
		if (input_number == 0)
		{
			cout << "Invalid input\n";
			continue;
		}

		// function called in if parameter returns true when login function succeeds.
		if (CafeConnectionManager::GetInstance()->RequestCardUsage(input_number))
		{
			break;
		}
		else
		{
			cout << "Unavailable card number. try again\n\n";
		}
	}
	return true;
}

bool StatusUpdater::printOptions() {
	cout << \
		"\n-------------------\n\
Welcome to Tokki Pc cafe! I hope you enjoy your best time!\n\
if you have anything required using our service, please type one of the commands listed below.\n\
\n\
0.Check Status. (C)\n\
1.StopUsing. (S) \n\
2.Orderingfood. (F)\n\
3.Print Left Time. (T) \n\
4.Running program. (P)\n\
5.Quitting program. (Q)\n\
6.Random chatting. (R)\n\
--------------------- \n\n";
	return true;
}
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
#include<conio.h>
#include <windows.h>
#include "sha256.h"
#include<vector>
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
	printf("\n---Shut Down...---\n");
}
// 싱글톤 static private instance를 참조하기 위해 쓰이는 정적 메서드
StatusUpdater* StatusUpdater::GetInstance()
{
	if (!instance)
		instance = new StatusUpdater();
	return instance;
}

bool StatusUpdater::Register()
{//회원가이입
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
	if (CafeConnectionManager::GetInstance()->Register(name, age, phonenum, id, password, question, answer, email_address))
		if (CafeConnectionManager::GetInstance()->Login(string(id), string(password)))
			return true;
	cout << " what you typed, just don't work.\n";
	return false;
}

bool StatusUpdater::FindPsw()
{//비밀번호찾기

	char id[20];
	printf("---비밀번호 찾기---\n");
	printf("id를 입력해주세요! : ");
	scanf("%s", id);
	std::string answer;
	char question[60];
	strcpy(question, CafeConnectionManager::GetInstance()->Question(id));
	if (question[0] == '0')
	{//찾기 실패
		printf("id가 존재하지 않습니다!!\n");
		return false;
	}

	printf("비밀번호 질문 : %s\n", question);
	printf("답 : ");
	getline(cin, answer);

	if (answer.compare("") == 0)//버퍼가 안비워졌을때(비정상적인 입력)
		std::getline(cin, answer);//다시입력받음

	if (CafeConnectionManager::GetInstance()->Answer(id, (char*)answer.c_str()) == false)
	{
		return false;//실패
	}
	else
		return true;//성공


}

bool StatusUpdater::QueryValidation()
{

	while (true) {
		std::string Authorization_type;

		cout << "\nHELLO! ***TOKKI-PC*** \n\n\n회원 : (U)\n회원가입 : (R)\n비밀번호찾기 : (F)\n";
		std::getline(std::cin, Authorization_type);

		if (Authorization_type.compare("") == 0)
			std::getline(cin, Authorization_type);

		if (Authorization_type == "u" || Authorization_type == "U")
		{
			// same here. it breaks out in case validation succeeds.
			if (validate_ID())
				break;
		}
		else if (Authorization_type == "r" || Authorization_type == "R")
		{
			// same here. it breaks out in case validation succeeds.
			if (Register())
				continue;
		}
		else if (Authorization_type == "f" || Authorization_type == "F")
		{
			// same here. it breaks out in case validation succeeds.
			if (FindPsw())
				continue;
		}
		else
		{
			cout << "Wrong input\n";
		}

	}
	return true;
}
bool StatusUpdater::QueryAction()
{
	int Program_Num = 0;
	int info = 0;
	vector<string> program;//실행중인프로그램 벡터
	while (true)
	{
		printOptions();
		std::string input;

		std::getline(cin, input);

		if (input.compare("") == 0)//정상적인입력이아닐때(공백 등 null문자)
			std::getline(cin, input);//다시입력받음


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

			while (!program.empty())
			{//프로그램벡터에 들어있던 원소들을 차례대로 출력

			 /*최근 프로그램부터 pop함*/
				cout << program.back() << "을 종료하였습니다." << endl;
				program.pop_back();

				/*1초씩 쉼*/
				Sleep(1000);
			}
			break;
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
7.토끼덮밥 (2000원)\n\
					\n\
-------------------- \n\n"
<< endl;

			scanf("%d", &Order_Number);
			cout << "PC의 번호를 입력하세요" << endl;
			scanf("%d", &PC_number);
			// PC 번호와 주문 값을 받아서 매니저에 보낸다. (소켓 프로그래밍)

			if (CafeConnectionManager::GetInstance()->Send_order(Order_Number, PC_number))
			{
				// 아래 문구는 매니저에 성공적으로 전달되었을 때, 출력되는 문구.
				cout << "주문이 완료 되었습니다." << endl;
			}
			cin.clear();
			//cin.ignore('\n');
			continue;
		}


		//3.Change Password
		if (input == "t") {

			int number = 0;
			char id[15];
			char psw[30];
			char cp[30];
			char ccp[30];

			//사용자 정보를 입력받아 남은 시간을 출력하도록 한다.
			cout << "비밀번호 변경!!" << endl;
			cout << "id를 입력해주세요 : ";
			scanf("%s", id);
			cout << "원래 비밀번호를 입력해주세요! : ";
			scanf("%s", psw);
			cout << "바꿀 비밀번호를 입력해주세요! : ";
			scanf("%s", cp);
			cout << "바꿀 비밀번호를 다시한번 입력해주세요! : ";
			scanf("%s", ccp);

			if (strcmp(cp, ccp) == 0)
			{//비밀번호확인 ok
				int tt;
				CafeConnectionManager::GetInstance()->ChangePsw(id, psw, ccp, &tt);
				if (tt == 0)
				{
					cout << "id나 password가 틀렸습니다!" << endl;
				}
				else
					cout << "변경완료!" << endl;
			}

			continue;
		}

		//4.Running program.
		if (input == "p") {
			std::string prog;
			info = 0;

			// 프로그램 실행.

			cout << "실행할 프로그램 이름을 입력하세요 :";
			getline(cin, prog);

			// 저장은 벡터사용

			program.push_back(prog);

			continue;
		}
		//5.Show Program
		if (input == "h") {

			cout << "현재 실행중인 프로그램 : ";

			for (int i = 0; i < program.size(); i++)
				cout << "[" << program[i] << "]  ";
			cout << endl;
			continue;
		}
		//6.Quitting program.
		if (input == "q") {
			string dead_program;
			//가장 최근의 프로그램을 종료

			cout << program.back() << "을 종료하였습니다." << endl;
			program.pop_back();
		}
	}
	return true;
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
		char ps[50];
		char psw;
		std::cout << "ID please. Enter Q to quit\n";
		std::getline(std::cin, input_id);
		std::cin.clear();
		if (input_id == "q" || input_id == "Q") {
			return false;
		}
		std::cout << "and password\n";

		for (int i = 0;;)
		{//password를 *로 표시
			psw = getch();
			if ((psw >= 'a'&&psw <= 'z') || (psw >= 'A'&&psw <= 'Z') || (psw >= '0'&&psw <= '9'))
			{
				ps[i] = psw;
				++i;
				cout << "*";
			}
			if (psw == '\b'&&i >= 1)//backspace눌렀을때
			{
				cout << "\b \b";
				--i;
			}
			if (psw == '\r')//enter눌렀을때(입력종료)
			{
				ps[i] = '\0';
				break;
			}

		}

		input_password = ps;

		std::string temp;
		temp = sha256(std::string(input_password));//비밀번호가 암호화됨
		input_password = temp;
		// function called in if parameter returns true when login function succeeds.
		int l = CafeConnectionManager::GetInstance()->Login(input_id, input_password);
		if (l == 1)//정상로그인
			break;
		else if (l == 2)//시간이 없을때(매니저클라에서 충전하고와야됨)
			cout << "\nNO TIME!!!! PLEASE RECHARGE TIME!\n\n";
		else//아이디나 비번틀림
			cout << "\nWrong ID or password . try again\n\n";
	}
	return true;
}

bool StatusUpdater::printOptions()
{//사용자 메뉴출력
	cout << \
		"\n-------------------\n\
Welcome to Tokki Pc cafe! I hope you enjoy your best time!\n\
if you have anything required using our service, please type one of the commands listed below.\n\
\n\
0.Check Status. (C)\n\
1.StopUsing. (S) \n\
2.Orderingfood. (F)\n\
3.Change Password. (T) \n\
4.Running program. (P)\n\
5.Show program. (H)\n\
6.Quitting program. (Q)\n\
--------------------- \n\n";
	return true;
}

/*
this class is where the terminal function is located.
it interprets user's command and does what it's intended to do.
yunu Lee
2016.11.07
-------------------------
���� �α��α��� ������ ����. - ȸ�����Ե� �����ؾ� ��.
�α��� �� - ��ɾ �Է¹޴� �ͱ���.
�׸��� ���ĸ޴�, ����� ǥ �ۼ�.
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

// �̱��� static private instance
StatusUpdater* StatusUpdater::instance = nullptr;
StatusUpdater::StatusUpdater()
{

}

StatusUpdater::~StatusUpdater()
{

}
void StatusUpdater::UpdateStatus(char* user_info,float left_seconds)
{
}
void StatusUpdater::StartUsing()
{
	//updater_thread.join();
	system("CLS"); // ȭ���� �����.
	// QueryAction returns false when StopUsing command gets typed.
	QueryAction();
	return;
}
void StatusUpdater::StopUsing()
{
}
// �̱��� static private instance�� �����ϱ� ���� ���̴� ���� �޼���
StatusUpdater* StatusUpdater::GetInstance()
{
	if (!instance)
		instance = new StatusUpdater();
	return instance;
}

bool StatusUpdater::QueryValidation()
{

	while (true) {
		std::string Authorization_type;

		cout << "\nPlease Type card, or id to get authorized\n";
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
		// 1.StopUsing 
		if (input == "s") {
			continue;
		}
		//2.Orderingfood.
		if (input == "f") {

			int Order_Number = 0;
			int PC_number = 0;

			// ���� ����ǥ ��µǰ� - ����ڰ� �����ϵ��� �䱸��.

			cout << "�ֹ��� ������ ��ȣ�� �Է��ϼ���" << endl;
			cout << \
				"-------------------\n\
					\n\
1.���� �䳢��� (2000��).\n\
2.�䳢�� ���� (2000��)\n\
3.���䳢 ���� (5000��) \n\
4.��-�� (1000��)\n\
5.�䳢 �Ұ����� (1500��)\n\
6.��Ÿ�ο��� (1200��)\n\
7.�䳢���� �ðܺ� - �������� �� ���� �� �ϳ��� �ֹ��˴ϴ�. (2000��)\n\
					\n\
-------------------- \n\n"
<< endl;

			cin >> Order_Number;
			cout << "PC�� ��ȣ�� �Է��ϼ���" << endl;
			cin >> PC_number;
			// PC ��ȣ�� �ֹ� ���� �޾Ƽ� �Ŵ����� ������. (���� ���α׷���)

			if (CafeConnectionManager::GetInstance()->Send_order(Order_Number, PC_number))
			{
				// �Ʒ� ������ �Ŵ����� ���������� ���޵Ǿ��� ��, ��µǴ� ����.
				cout << "�ֹ��� �Ϸ� �Ǿ����ϴ�." << endl;
			}
			continue;
		}


		//3.Print Left Time.
		if (input == "t") {

			int number = 0;
			//����� ������ �Է¹޾� ���� �ð��� ����ϵ��� �Ѵ�.
			cout << "id �� ��ȣ�� �Է��ϼ���." << endl;
			cin >> number;
			cout << "���� ���ð��� :";
			CafeConnectionManager::GetInstance()->Check_Time(number);
			continue;
		}

		//4.Running program.
		if (input == "p") {
			string program;
			info = 0;

			// ���α׷� ����.
			// ��� 1. - Ű���� ��ŷ(Ű�α�)
			// ��� 2. - ����ڰ� ���� �Է���. / �� ��� ���α׷��� �Է��� �ް� ������ ������ ���.

			cout << "������ ���α׷� �̸��� �Է��ϼ��� :";
			cin >> program;
			cout << "id �� ��ȣ�� �Է��ϼ���." << endl;
			cin >> info;
			// ������ ������ ����ϰų� ��ȣ�� ���� �Ѱܼ� ū ������ ����.
			if (CafeConnectionManager::GetInstance()->Send_program(Program_Num, info, program)) {
				cout << program << " is running�� " << endl;
				Program_Num++;
			}
			continue;
		}
		//5.Quitting program.
		if (input == "q") {
			string dead_program;
			//info�� Program_Num ° ���α׷� (���� �ֱ��� ���α׷�)�� ����

			if (CafeConnectionManager::GetInstance()->Quit_program(Program_Num, info) && Program_Num > 0) {
				string dead_program = CafeConnectionManager::GetInstance()->get_program(Program_Num, info);
				cout << dead_program << "is closed" << endl;
				Program_Num--;
			} // �ߵǸ� ���α׷��� �����ٰ� ����ϰ� �ѹ��� �ϳ� ����.
			else
				cout << "Error!" << endl;
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
		// function called in if parameter returns true when login function succeeds.
		if (ServerConnectionManager::GetInstance()->Login(input_id, input_password))
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
1.StopUsing. (S) \n\
2.Orderingfood. (F)\n\
3.Print Left Time. (T) \n\
4.Running program. (P)\n\
5.Quitting program. (Q)\n\
--------------------- \n\n";
	return true;
}
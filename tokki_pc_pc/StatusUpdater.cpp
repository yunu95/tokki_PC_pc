/*
this class is where the terminal function is located.
it interprets user's command and does what it's intended to do.
yunu Lee
2016.11.07
*/
#include <iostream>
#include "StatusUpdater.h"
#include "ServerConnectionManager.h"
#include "CafeConnectionManager.h"
#include <string>
// 싱글톤 static private instance
StatusUpdater* StatusUpdater::instance = nullptr;
StatusUpdater::StatusUpdater()
{

}

StatusUpdater::~StatusUpdater()
{

}
// 싱글톤 static private instance를 참조하기 위해 쓰이는 정적 메서드
StatusUpdater* StatusUpdater::GetInstance()
{
	if (!instance)
		instance = new StatusUpdater();
	return instance;
}

bool StatusUpdater::QueryValidation() {

	while (true) {
		std::string Authorization_type;

		cout << "Please Type card, or id to get authorized\n";
		std::getline(std::cin, Authorization_type);

		if (Authorization_type == "card")
			//if validation succeeds, it breaks out.
			if(validate_card())
				break;
		else if (Authorization_type == "id")
			// same here. it breaks out in case validation succeeds.
			if(validate_ID())
				break;
		else {
			cout << "Wrong input\n";
		}

	}
	return true;
}
bool StatusUpdater::validate_ID()
{
	std::string input_id;
	std::string input_password;
	while (true)
	{
		std::cout << "ID please. Enter Q to quit\n";
		std::getline(std::cin, input_id);
		if (input_id == "q" || input_id == "Q")
			return false;
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
		std::cout << "type cardnumber please. Enter Q to quit\n";
		if (input_number == 'q' || input_number == 'q')
			return false;
		cin >> input_number;
		// function called in if parameter returns true when login function succeeds.
		if (CafeConnectionManager::GetInstance()->RequestCardUsage(input_number))
			break;
		else
			cout << "Unavailable card number. try again\n\n";
	}
}
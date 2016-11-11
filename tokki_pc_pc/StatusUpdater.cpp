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
// ΩÃ±€≈Ê static private instance
StatusUpdater* StatusUpdater::instance = nullptr;
StatusUpdater::StatusUpdater()
{

}

StatusUpdater::~StatusUpdater()
{

}
void StatusUpdater::UpdateStatus()
{
}
void StatusUpdater::StartUsing()
{
	//updater_thread.join();
	system("CLS");
	cout << \
"-------------------\n\
Welcome to Tokki Pc cafe! I hope you enjoy your best time!\n\
if you have anything required using our service, please type one of the commands listed below.\n\
1.StopUsing. \n\
--------------------- \n\n";
	// QueryAction returns false when StopUsing command gets typed.
	if (QueryAction())
		return;
}
void StatusUpdater::StopUsing()
{
}
// ΩÃ±€≈Ê static private instance∏¶ ¬¸¡∂«œ±‚ ¿ß«ÿ æ≤¿Ã¥¬ ¡§¿˚ ∏ﬁº≠µÂ
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
	while (true)
	{
		std::string input;
		std::getline(cin, input);
		cin.clear();
		// for the convenience,
		for (string::iterator i = input.begin(); i < input.end(); i++)
		{
			*i = tolower(*i);
		}
		if (input == "stopusing")
			return false;
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
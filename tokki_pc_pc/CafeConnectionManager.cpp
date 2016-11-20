#include "CafeConnectionManager.h"

CafeConnectionManager * CafeConnectionManager::GetInstance()
{
	return nullptr;
}

bool CafeConnectionManager::RequestCardUsage(int card_num)
{
	return true;
}

CafeConnectionManager::CafeConnectionManager()
{
}

CafeConnectionManager::~CafeConnectionManager()
{
}

bool CafeConnectionManager::Send_order(int order, int PC_number)
{
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






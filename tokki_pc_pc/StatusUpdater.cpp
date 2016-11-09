/*
this class is where the terminal function is located.
it interprets user's command and does what it's intended to do.
yunu Lee
2016.11.07
*/
#include <iostream>
#include "StatusUpdater.h"
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
	std::string input_id;
	std::string input_password;
	std::cout << "ID please\n";
	std::getline(std::cin,input_id);
	std::cout << "and password\n";
	std::getline(std::cin, input_password);
	return true;
}
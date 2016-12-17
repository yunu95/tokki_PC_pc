/*
this is a member class which represents each account.
it can be initialized during register, or login function.
Yunu Lee
2016.11.07
*/
/*
final modified ( all function ) & Running
GRu Lee
2016.12.17
*/
#pragma once
#include <string>
class Member
{
public:
	std::string ID;
	std::string password;
	Member();
	~Member();
};


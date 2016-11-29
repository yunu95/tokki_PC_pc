/*
this is a member class which represents each account.
it can be initialized during register, or login function.
Yunu Lee
2016.11.07
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


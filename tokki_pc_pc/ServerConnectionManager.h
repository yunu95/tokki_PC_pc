#pragma once
#include<iostream>
#include <string>
using namespace std;
/*
this class manages every connection between main DB server.
any login function or Register function requires certain function declared here to connect with DB server.
Yunu Lee
2016.11.07
*/
class ServerConnectionManager
{
	class Member;
public:
	static ServerConnectionManager* GetInstance();
	ServerConnectionManager();
	~ServerConnectionManager();

	bool Login(const string& ID, const string& password);
	bool Register();
	Member GetUserData(std::string& ID);
	bool RegisterUserData(Member);
	// Member 는 PC managerment 클라이언트에서 받아 옵니다.(?)

private:
	static ServerConnectionManager* instance;
};

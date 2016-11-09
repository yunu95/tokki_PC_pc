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
	// Member �� PC managerment Ŭ���̾�Ʈ���� �޾� �ɴϴ�.(?)

private:
	static ServerConnectionManager* instance;
};

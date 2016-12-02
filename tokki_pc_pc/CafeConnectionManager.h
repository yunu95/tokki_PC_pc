#pragma once
#pragma comment (lib, "ws2_32.lib")
#include <string>
#include <stdio.h>
#include <WinSock2.h>
#include <process.h>
#include <thread>
/*
this class manages every connection between a Main management client program in a single PC cafe.
Yunu Lee
2016.11.07
*/
class CafeConnectionManager
{
private:
	const char* fixed_management_pc_ip = "127.0.0.1";
	// it is the standard length of the request identifier string.
	const int request_bytes_size = 100;
	std::thread listener;
public:
	bool Register(char * name, char * age, char * phonenum, char * id, char * passwd, char* question, char* psw_answer, char* email_address);
	// If you see Get Instance method, just consider the class singleton class.
	static CafeConnectionManager* GetInstance();
	bool Login(const std::string& ID, const std::string& password);
	// it returns true when Report function ends well.
	// it gets fired when you start, or stop using your pc.

	// this just reports, "This computer is turned on. but no one is using."
	// request identifier is Report
	bool Report(bool is_turning_on,int pc_num);
	// it's like telling the cafe magement client like this.
	// "Yo! I'm using one of your cards whose index is... card_num! 
	// if you don't have any problem with that, just return me true."
	bool RequestCardUsage(int card_num);
	bool Send_order(int order, const int& PC_number);
	std::string Check_Time(int info_number);
	std::string Check_Status();
	bool Send_program(int count, int number, std::string program);
	bool Quit_program(int Program_Num, int info);
	std::string get_program(int Program_Num, int info);
	bool StopUsing(int pc_num);

private:
	//std::string GetSpaces(const int& length);
	WSADATA wsaData;
	SOCKET management_sock;
	SOCKADDR_IN serv_addr;
	// if you see private static class pointer of itself, and especially it's named instance,
	// it's 100 % singleton class
	static CafeConnectionManager* instance;
	// if you see constructor is declared in private field
	// it's probably singleton class
	CafeConnectionManager();
	~CafeConnectionManager();

};
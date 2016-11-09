#pragma once
/*
this class manages every connection between a Main management client program in a single PC cafe.
Yunu Lee
2016.11.07
*/
class CafeConnectionManager
{
public:
	// If you see Get Instance method, just consider the class singleton class.
	static CafeConnectionManager* GetInstance();
	// it returns true when Report function ends well.
	// it gets fired when you start, or stop using your pc.
	bool Report();
	// it's like telling the cafe magement client like this.
	// "Yo! I'm using one of your cards whose index is... card_num! 
	// if you don't have any problem with that, just return me true."
	bool RequestCardUsage(int card_num);
private:
	// if you see private static class pointer of itself, and especially it's named instance,
	// it's 100 % singleton class
	static CafeConnectionManager* instance;
	// if you see constructor is declared in private field
	// it's probably singleton class
	CafeConnectionManager();
	~CafeConnectionManager();

};
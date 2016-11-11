#pragma once
#include <thread>
class StatusUpdater
{
public:
	// this class's only instance can be referenced by using this method.
	static StatusUpdater* GetInstance();
	//
	enum PC_Status { Using, NotUsing };
	// QueryValidation is a function for authorizing user to use computer.
	// Card or user account info is needed to authorize.
	bool QueryValidation();
	// It asks player to type command related with pc cafe service.
	// ex) logout, switching seat, ordering food... somethings like these.
	bool QueryAction();
	// It gets called by pc managing client or itself. it turns off the system.
	void AbortUsing();
	// it gets called after successfully acheiving validation.
	void StartUsing();
private:
	// only instance.
	static StatusUpdater* instance;
	// thread to update pc's status and send it's info to pc_cafe manager client periodically.
	std::thread updater_thread;

	StatusUpdater();
	~StatusUpdater();

	PC_Status current_pc_status;
	// it means what it means. update its own status according to the elapsed time.
	void UpdateStatus();
	// it gets called when user wants to stop using ther service.
	void StopUsing();
	// both methods below get called by QueryValidation method.
	bool validate_ID();
	bool validate_card();
};
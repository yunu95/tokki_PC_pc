#include "ServerConnectionManager.h"

ServerConnectionManager* ServerConnectionManager::instance = nullptr;

ServerConnectionManager* ServerConnectionManager::GetInstance()
{
	// if instance is null value, than call the constructor and set instance usable.
	if (!instance)
		instance = new ServerConnectionManager();
	return instance;
}
ServerConnectionManager::ServerConnectionManager()
{
}

ServerConnectionManager::~ServerConnectionManager()
{
}
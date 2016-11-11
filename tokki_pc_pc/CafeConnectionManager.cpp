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
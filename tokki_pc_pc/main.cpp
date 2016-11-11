#include "StatusUpdater.h"
/*
We all know what main is. the very first start point of this whole program.
Yunu Lee
2016.11.07
*/
int main() {
	if (StatusUpdater::GetInstance()->QueryValidation())
		StatusUpdater::GetInstance()->StartUsing();
	return 0;
}
#include "StatusUpdater.h"
/*
We all know what main is. the very first start point of this whole program.
Yunu Lee
2016.11.07
*/
int main() {
	if (StatusUpdater::GetInstance()->QueryValidation())
		// 여기서 확인한 것을 저장해야 합니다. - PC와 멤버/카드를 확인하고 체크아웃/로그아웃 전까지 연결을 유지합니다.
		StatusUpdater::GetInstance()->StartUsing();
	return 0;
}
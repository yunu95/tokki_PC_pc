#include "StatusUpdater.h"
/*
We all know what main is. the very first start point of this whole program.
Yunu Lee
2016.11.07
*/
int main() {
	if (StatusUpdater::GetInstance()->QueryValidation())
		// ���⼭ Ȯ���� ���� �����ؾ� �մϴ�. - PC�� ���/ī�带 Ȯ���ϰ� üũ�ƿ�/�α׾ƿ� ������ ������ �����մϴ�.
		StatusUpdater::GetInstance()->StartUsing();
	return 0;
}
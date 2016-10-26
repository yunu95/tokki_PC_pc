#pragma once

// 개별 PC의 상태를 계속 업그레이드 하는 클래스,
// 매초마다 호출된다.
class StatusUpdater
{
public:
	StatusUpdater();
	~StatusUpdater();

private:
	enum current_pc_status
	{	
		is_active = true,
		is_power_on = true
		
		// 여기에는 무슨 정보가 있는지?
		// PC의 상태니까 PC_ON PC_OFF 같은것?
	};
		// pc의 정보를 담고 있는 열거형
	UpdaterThredad // ? 이
};

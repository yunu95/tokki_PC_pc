#pragma once

// ���� PC�� ���¸� ��� ���׷��̵� �ϴ� Ŭ����,
// ���ʸ��� ȣ��ȴ�.
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
		
		// ���⿡�� ���� ������ �ִ���?
		// PC�� ���´ϱ� PC_ON PC_OFF ������?
	};
		// pc�� ������ ��� �ִ� ������
	UpdaterThredad // ? ��
};

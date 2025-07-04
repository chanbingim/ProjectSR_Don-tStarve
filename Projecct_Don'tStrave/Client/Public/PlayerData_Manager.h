#pragma once
#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Client)

class CPlayerData_Manager : public CBase
{
	DECLARE_SINGLETON(CPlayerData_Manager);

private:
	CPlayerData_Manager();
	virtual ~CPlayerData_Manager() = default;

public:
	const PLAYER_DATA& Get_PlayerData(_uint iItemID) const;
	void	LoadPlayerData(const char* MapFilePath);
private:
	_uint					m_iPlayerDataSize = {};
	vector<PLAYER_DATA>		m_tPlayerDataVec = {};

public:
	virtual void Free() override;
};

NS_END
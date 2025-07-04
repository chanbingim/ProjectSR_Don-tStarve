#pragma once
#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Client)

class CMonsterData_Manager : public CBase
{
	DECLARE_SINGLETON(CMonsterData_Manager);

private:
	CMonsterData_Manager();
	virtual ~CMonsterData_Manager() = default;

public:
	const MONSTER_DATA& Get_MonsterData(_uint iItemID) const;
	void	LoadMonsterData(const char* MapFilePath);
private:
	_uint					m_iMonsterDataSize = {};
	vector<MONSTER_DATA>		m_tMonsterDataVec = {};

public:
	virtual void Free() override;
};

NS_END
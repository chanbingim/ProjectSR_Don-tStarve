#pragma once
#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Client)

class CItem_Manager : public CBase
{
	DECLARE_SINGLETON(CItem_Manager);

private:
	CItem_Manager();
	virtual ~CItem_Manager() = default;

public:
	_float3&			Get_Position(_uint iIndex) { return m_DropDir[iIndex]; }
	const ITEM_DATA&	Get_ItemData(_uint iItemID) const;
	void				LoadItemData(const char* MapFilePath);
	

private:
	_uint				m_ItemDataSize = {};
	vector<ITEM_DATA>	m_ItemDatas = {};
	_float3				m_DropDir[8] = {};

public:
	virtual void Free() override;
};

NS_END
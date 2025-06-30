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
	const ITEM_DATA& Get_ItemData(_uint iItemID) const;
	void	LoadItemData(const char* MapFilePath);
private:
	_uint				m_ItemDataSize = {};
	vector<ITEM_DATA>	m_ItemDatas = {};

public:
	virtual void Free() override;
};

NS_END
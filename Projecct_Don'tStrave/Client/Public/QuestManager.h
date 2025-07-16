#pragma once

#include "Client_Defines.h"
#include "QuestData.h"
#include "Base.h"

NS_BEGIN(Client)
class CInventory;
class CItem_Manager;

class CQuestManager : public CBase
{
	DECLARE_SINGLETON(CQuestManager);

private :
	CQuestManager();
	virtual ~CQuestManager() = default;

public :
	HRESULT		Initialize();

	HRESULT		Accept_Quest(_uint QuestID);
	HRESULT		Cancel_Quest(_uint QuestID);
	HRESULT		Clear_Quest(_uint QuestID);

	_bool		CheckAndApplyCompensation(CQuestData* pQuest, _bool _flag);

	HRESULT		LoadQuestData(const char* FolderName, const char* FileName);
	HRESULT		ReleaseQuestData();

	_bool		IsQuestActive(_uint QuestID);
	_bool		IsQuestClear(_uint QuestID);
	_wstring	GetPercentData(CQuestData* pQuest);

	_int		GetMonstDeathCount(_uint iID);
	void		QuestStartEvent();

	list< CQuestData*>*			GetRunningQuest() { return &m_RunningQuest; }
	list< CQuestData*>*			GetQuestList() { return &m_QuestList; }
	list< CQuestData*>*			GetClearQuest() { return &m_ClearQuest; }

	void						CallMonsterDeath(_uint iID);

private :
	const char*					FrontFilePath = "../Bin/Resources/DataStruct/";
	map<_uint, CQuestData*>		m_QuestMap;
	CInventory*					m_pInven = { nullptr };
	CItem_Manager*				m_pItemManager = { nullptr };

	list<CQuestData*>			m_QuestList;
	list<CQuestData*>			m_RunningQuest;
	list<CQuestData*>			m_ClearQuest;

	list<pair<_uint, _uint>>	m_DeathMonsterCnt;
	_uint						m_CheckDeadthEvent = {};

private :
	_int						UpdateDeathList(_uint iID);

	void						ApplyCompensation(_uint ItemID, _uint ItemCnt);

public :
	virtual		void		Free() override;
	
};

NS_END
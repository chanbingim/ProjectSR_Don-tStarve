#pragma once

#include "Client_Defines.h"
#include "QuestData.h"
#include "Base.h"

NS_BEGIN(Client)
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

	HRESULT		LoadQuestData(const char* FolderName, const char* FileName);
	HRESULT		ReleaseQuestData();

private :
	const char*					FrontFilePath = "../Bin/Resources/DataStruct/";
	map<_uint, CQuestData*>		m_QuestMap;

	list< CQuestData*>			m_RunningQuest;
	list< CQuestData*>			m_ClearQuest;
public :
	virtual		void		Free() override;
	
};

NS_END
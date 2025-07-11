#pragma once

#include "Base.h"

NS_BEGIN(Client)
class CQuestInfo; 

class CQuestManager : public CBase
{
	DECLARE_SINGLETON(CQuestManager);

private :
	CQuestManager();
	virtual ~CQuestManager() = default;

public :
	HRESULT		Initialize();
	HRESULT		Clear_Quest(_uint QuestID);

private :
	map<_uint, CQuestInfo*>		m_QuestMap;

public :
	virtual		void		Free() override;
	
};

NS_END
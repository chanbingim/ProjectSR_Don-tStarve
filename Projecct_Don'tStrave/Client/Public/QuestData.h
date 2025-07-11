#pragma once

#include "Client_Defines.h"

struct CQuestData
{
	_bool		bIsActive;
	_uint		QuestID;
	_wstring	QuestInfo;
	
	_uint		DropItemID;
	_uint		DropItemCnt;

	vector<CQuestData>	ConnectQuest;
};

NS_BEGIN(Client)
//퀘스트 클래스 구현
NS_END
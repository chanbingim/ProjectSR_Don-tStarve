#pragma once

#include "Client_Defines.h"

NS_BEGIN(Client)
enum	CQuestType { COMBAT, COLLECTION, END };

struct CQuestData
{
	_bool							bIsActive;		// 초기상태
	_bool							bIsClear = { false };		// 완료 상태
	_uint							QuestID;		// 퀘스트 ID
	_wstring						QuestTitle;		// 퀘스트 제목
	_wstring						SubTitle;		// 퀘스트 내용
	_wstring						ClearTitle;		// 퀘스트 클리어시 나오는 문구

	vector<pair<_uint, _uint>>		ClearCondition; // 클리어 조건
	vector<pair<_uint, _uint>>		DropItem;		// 보상 내용

	vector<_uint>					ConnectQuest;	//연관 퀘스트
	CQuestType						type;			//퀘스트 타입
};

typedef struct  ClearData
{
	vector<_int>		CurCount;
	CQuestData*			QuestInfo;
}CLEAR_DATA;


NS_END
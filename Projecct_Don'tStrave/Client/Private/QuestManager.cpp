#include "QuestManager.h"

#include "File.h"
#include "CUtility.h"
#include "GameInstance.h"
#include "Inventory.h"

IMPLEMENT_SINGLETON(CQuestManager);

CQuestManager::CQuestManager()
{
}

HRESULT CQuestManager::Initialize()
{

	return S_OK;
}

HRESULT CQuestManager::Accept_Quest(_uint QuestID)
{
	auto  pair = m_QuestMap.find(QuestID);
	if (pair == m_QuestMap.end())
		return E_FAIL;

	m_QuestList.remove(pair->second);
	m_RunningQuest.push_back(pair->second);
	return S_OK;
}

HRESULT CQuestManager::Cancel_Quest(_uint QuestID)
{
	auto iter = find_if(m_RunningQuest.begin(), m_RunningQuest.end(), [&](CQuestData* Quest)
				{
					return Quest->QuestID == QuestID ? true : false;
				});

	if (iter == m_RunningQuest.end())
		return E_FAIL;

	m_RunningQuest.erase(iter);
	return S_OK;
}

HRESULT CQuestManager::Clear_Quest(_uint QuestID)
{
	auto  pair = m_QuestMap.find(QuestID);
	if (pair == m_QuestMap.end())
		return E_FAIL;
	auto CheckQuest = pair->second;

	if (CheckQuest)
	{
		if (!CheckQuest->bIsClear)
		{
			CheckQuest->bIsClear = true;
			CheckAndApplyCompensation(CheckQuest, true);
			
			for (auto& iter : CheckQuest->ConnectQuest)
			{
				auto  pair = m_QuestMap.find(iter);
				if (pair == m_QuestMap.end())
					return E_FAIL;

				pair->second->bIsActive = true;
				m_QuestList.push_back(pair->second);
			}
		}
	}

	m_RunningQuest.remove(CheckQuest);
	m_ClearQuest.push_back(CheckQuest);
	return S_OK;
}

//데이터 파싱 로직 건들지 말것
//건들면 앞으로 평생 대머리 탈모
//만든 사람만 건들기
HRESULT CQuestManager::LoadQuestData(const char* FolderName, const char* FileName)
{
	CFile	file;
	char    filePath[MAX_PATH] = {};
	WCHAR   wfilePath[MAX_PATH] = {};

	_uint	NextQuestIndex = {};
	vector<string> vecQuestData = {};
	sprintf_s(filePath, "%s%s/%s", FrontFilePath, FolderName, FileName);
	file.ReadCSVData(filePath, ',', &vecQuestData);

	for (_uint i = 11;  i < vecQuestData.size();)
	{
		CQuestData* Data = new CQuestData;
		CUtility::ConvertUTFToWide(vecQuestData[i].c_str(), wfilePath);
		Data->QuestTitle =	wfilePath;
		i++;

		CUtility::ConvertUTFToWide(vecQuestData[i].c_str(), wfilePath);
		Data->SubTitle =	wfilePath;
		i++;

		CUtility::ConvertUTFToWide(vecQuestData[i].c_str(), wfilePath);
		Data->ClearTitle =	wfilePath;
		i++;

		_uint type = {};
		stringstream CellBoolStream(vecQuestData[i]);
		i++;

		stringstream CellIdStream(vecQuestData[i]);
		i++;

		stringstream CellTpyeStream(vecQuestData[i]);
		i++;

		if (CellBoolStream >> Data->bIsActive && CellIdStream >> Data->QuestID && CellTpyeStream >> type)
			Data->type = CQuestType(type);

		stringstream	ConnectionData(vecQuestData[i]);
		i++;

		while (ConnectionData >> NextQuestIndex)
		{
			Data->ConnectQuest.emplace_back(NextQuestIndex);
		}

		_uint item, iCnt;
		for (int j = 0; j < 2; ++j)
		{
			stringstream CellItemStream(vecQuestData[i]);
			i++;

			stringstream CellCntStream(vecQuestData[i]);
			i++;
			while (CellItemStream >> item && CellCntStream >> iCnt)
			{
				if(0 == j)
					Data->ClearCondition.emplace_back(item, iCnt);
				else
					Data->DropItem.emplace_back(item, iCnt);
			}
		}

		if (Data->bIsActive)
			m_QuestList.push_back(Data);

		m_QuestMap.insert({Data->QuestID, Data});
	}

	return S_OK;
}

HRESULT CQuestManager::ReleaseQuestData()
{
	for (auto& Pair : m_QuestMap)
	{
		Safe_Delete(Pair.second);
	}
	m_QuestMap.clear();

	return S_OK;
}

_bool CQuestManager::CheckAndApplyCompensation(CQuestData* pQuest, _bool _flag)
{
	if (nullptr == pQuest)
		return false;

	switch (pQuest->type)
	{
	case CQuestType::COLLECTION:
	{
		auto Inven = static_cast<CInventory*>(CGameInstance::GetInstance()->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_UserInterface")));
		for (auto pair : pQuest->ClearCondition)
		{
			if (_flag)
				Inven->Use_Item(pair.first, pair.second);
			else
			{
				_uint OwnCount = Inven->Check_ItemCount(pair.first);
				if (OwnCount < pair.second)
				{
					return false;
				}
			}
		}
	}
	break;
	case CQuestType::COMBAT:
	{
	}
	break;
	}

	return true;
}

void CQuestManager::Free()
{
	__super::Free();

	for (auto& Pair : m_QuestMap)
	{
		Safe_Delete(Pair.second);
	}
	m_QuestMap.clear();
}

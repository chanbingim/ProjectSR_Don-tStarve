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

	if (CQuestType::COMBAT == CQuestType(pair->second->type))
	{
		for (auto& ClearPair : pair->second->ClearCondition)
			UpdateDeathList(ClearPair.first);
		m_CheckDeadthEvent++;
	}

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
			
			if (CQuestType::COMBAT == CheckQuest->type)
			{
				m_CheckDeadthEvent--;
				if (m_CheckDeadthEvent <= 0)
					m_DeathMonsterCnt.clear();
			}

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

_wstring CQuestManager::GetPercentData(CQuestData* pQuest)
{
	if (nullptr == pQuest)
		return L"";

	WCHAR Percent[MAX_PATH] = {};
	_uint OwnCount{}, MaxCount{};
	_uint iID = pQuest->QuestID;
	switch (CQuestType(pQuest->type))
	{
	case CQuestType::COLLECTION:
	{
		auto Inven = static_cast<CInventory*>(CGameInstance::GetInstance()->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_UserInterface")));
		for (auto& pair : pQuest->ClearCondition)
		{
			OwnCount = Inven->Check_ItemCount(pair.first);
			MaxCount = pair.second;
			break;
		}
	}
	break;
	case CQuestType::COMBAT:
	{
		if (0 < m_CheckDeadthEvent)
		{
			for (auto& pair : pQuest->ClearCondition)
			{
				OwnCount = GetMonstDeathCount(pair.first);
				if (OwnCount >= pair.second)
					OwnCount = pair.second;

				MaxCount = pair.second;
				break;
			}
		}
	}
	break;
	}

	if (0 >= OwnCount && 0 >= MaxCount)
		return L"";

	wsprintf(Percent, TEXT("%d / %d"), OwnCount, MaxCount);
	return Percent;
}

_uint CQuestManager::GetMonstDeathCount(_uint iID)
{
	auto iter = find_if(m_DeathMonsterCnt.begin(), m_DeathMonsterCnt.end(), [&](auto pair)
		{
			return pair.first == iID ? true : false;
		});

	if (iter == m_DeathMonsterCnt.end())
		return -1;

	return iter->second;
}

void CQuestManager::QuestStartEvent()
{
	auto pair = m_QuestMap.find(0);
	pair->second->bIsActive = true;
	m_QuestList.push_back(pair->second);
}

void CQuestManager::CallMonsterDeath(_uint iID)
{
	UpdateDeathList(iID);
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
		for (auto& pair : pQuest->ClearCondition)
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
		if (0 < m_CheckDeadthEvent)
		{
			for (auto& pair : pQuest->ClearCondition)
			{
				if (pair.second > GetMonstDeathCount(pair.first))
				{
					return false;
				}
			}
		}
	}
	break;
	}

	return true;
}

_uint CQuestManager::UpdateDeathList(_uint iID)
{
	auto iter = find_if(m_DeathMonsterCnt.begin(), m_DeathMonsterCnt.end(), [&](auto pair)
		{
			return pair.first == iID ? true : false;
		});

	if (iter == m_DeathMonsterCnt.end())
		m_DeathMonsterCnt.emplace_back(iID, 0);
	else
		return iter->second++;

	return 1;
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

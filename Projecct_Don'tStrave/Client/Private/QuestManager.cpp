#include "QuestManager.h"

#include "File.h"
#include "CUtility.h"
#include "GameInstance.h"

#include "Slot.h"
#include "Item.h"
#include "Inventory.h"
#include "Item_Manager.h"
#include "MonsterData_Manager.h"

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

	if (CQuestType::COMBAT == CQuestType(pair->second->type))
	{
		CLEAR_DATA Data;
		Data.CurCount.resize(pair->second->ClearCondition.size(), 0);
		Data.QuestInfo = pair->second;
		m_DeathMonsterCnt.push_back(Data);
	}

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
		if (CheckQuest->DropItem.size() > m_pInven->Get_EmptySlotCnt())
		{
			MSG_BOX("인벤토리를 비워주세요");
			return E_FAIL;
		}

		if (!CheckQuest->bIsClear)
		{
			CheckQuest->bIsClear = true;
			CheckAndApplyCompensation(CheckQuest, true);
			
			if (CQuestType::COMBAT == CheckQuest->type)
			{
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

			for (auto& pair : CheckQuest->DropItem)
			{
				ApplyCompensation(pair.first, pair.second);
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

_bool CQuestManager::IsQuestActive(_uint QuestID)
{
	for (auto& iter : m_RunningQuest)
	{
		if (iter->QuestID == QuestID)
			return true;
	}

	return false;
}

_bool CQuestManager::IsQuestClear(_uint QuestID)
{
	auto iter = m_QuestMap.find(QuestID);

	return iter->second->bIsClear;
}

_wstring CQuestManager::GetPercentData(CQuestData* pQuest)
{
	if (nullptr == pQuest)
		return L"";

	WCHAR Percent[MAX_PATH] = {};
	_uint	TargetID{};
	_int OwnCount{}, MaxCount{};
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
			TargetID = pair.first;
			if (OwnCount < pair.second)
				break;
				
		}
		auto TargetData = CItem_Manager::GetInstance()->Get_ItemData(TargetID);
		wsprintf(Percent, TEXT("%s : %d / %d"), TargetData.strKorName.c_str(), OwnCount, MaxCount);
	}
	break;
	case CQuestType::COMBAT:
	{
		if (!m_DeathMonsterCnt.empty())
		{
			auto iter = find_if(m_DeathMonsterCnt.begin(), m_DeathMonsterCnt.end(), [&](ClearData& ClearData)
						{
							return ClearData.QuestInfo == pQuest ? true : false;
						});

			int i = 0;
			for (auto& pair : pQuest->ClearCondition)
			{
				OwnCount = (*iter).CurCount[i];
				TargetID = pair.first;
				if (OwnCount >= pair.second)
				{
					OwnCount = MaxCount = pair.second;
				}
				else
				{
					MaxCount = pair.second;
					break;
				}
				i++;
			}

			auto TargetData = CMonsterData_Manager::GetInstance()->Get_MonsterData(TargetID);
			wsprintf(Percent, TEXT("%s : %d / %d"), TargetData.strName.c_str(), OwnCount, MaxCount);
		}

	}
	break;
	}

	if (0 >= OwnCount && 0 >= MaxCount)
		return L"";

	return Percent;
}

void CQuestManager::QuestStartEvent()
{
	auto pair = m_QuestMap.find(1);
	pair->second->bIsActive = true;
	m_QuestList.push_back(pair->second);

	if(nullptr == m_pInven)
		m_pInven = static_cast<CInventory*>(CGameInstance::GetInstance()->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_UserInterface")));

	if (nullptr == m_pItemManager)
		m_pItemManager = CItem_Manager::GetInstance();
}

void CQuestManager::CallMonsterDeath(_uint iID)
{
	UpdateDeathList(iID);
}

void CQuestManager::ClearCheatFunc()
{
	for (auto iter : m_RunningQuest)
	{
		if (iter->DropItem.size() > m_pInven->Get_EmptySlotCnt())
		{
			MSG_BOX("인벤토리를 비워주세요");
			return;
		}

		if (!iter->bIsClear)
		{
			iter->bIsClear = true;
			CheckAndApplyCompensation(iter, true);

			if (CQuestType::COMBAT == iter->type)
			{
				m_DeathMonsterCnt.clear();
			}

			for (auto& iter : iter->ConnectQuest)
			{
				auto  pair = m_QuestMap.find(iter);
				if (pair == m_QuestMap.end())
					return;

				pair->second->bIsActive = true;
				m_QuestList.push_back(pair->second);
			}

			for (auto& pair : iter->DropItem)
			{
				ApplyCompensation(pair.first, pair.second);
			}
		}

		m_ClearQuest.push_back(iter);
	}
	m_RunningQuest.clear();
}

_bool CQuestManager::CheckAndApplyCompensation(CQuestData* pQuest, _bool _flag)
{
	if (nullptr == pQuest)
		return false;

	switch (pQuest->type)
	{
	case CQuestType::COLLECTION:
	{
		for (auto& pair : pQuest->ClearCondition)
		{
			if (_flag)
				m_pInven->Use_Item(pair.first, pair.second);
			else
			{
				_uint OwnCount = m_pInven->Check_ItemCount(pair.first);
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
		if (!m_DeathMonsterCnt.empty())
		{
			auto iter = find_if(m_DeathMonsterCnt.begin(), m_DeathMonsterCnt.end(), [&](ClearData& ClearData)
				{
					return ClearData.QuestInfo == pQuest ? true : false;
				});

			int i = 0;
			for (auto& pair : pQuest->ClearCondition)
			{
				if (pair.second > (*iter).CurCount[i])
				{
					return false;
				}
				i++;
			}
		}
	}
	break;
	}

	return true;
}

_int CQuestManager::UpdateDeathList(_uint iID)
{
	_bool flag = false;
	for (auto& iter : m_RunningQuest)
	{
		if (iter->type == CQuestType::COMBAT)
			flag = true;
	}

	if (!flag)
	{
		m_DeathMonsterCnt.clear();
		return -1;
	}

	for (auto& iter : m_DeathMonsterCnt)
	{
		int i = 0;
		for (auto& pair : iter.QuestInfo->ClearCondition)
		{
			if (pair.first == iID)
				iter.CurCount[i]++;

			i++;
		}
	}

	return 1;
}

void CQuestManager::ApplyCompensation(_uint ItemID, _uint ItemCnt)
{
	auto Slot = m_pInven->Find_Item(ItemID);
	auto ItemData = m_pItemManager->Get_ItemData(ItemID);

	ITEM_DESC Item_Desc;
	Item_Desc.iItemID = ItemID;
	Item_Desc.eSlot = ItemData.eSlot;
	Item_Desc.fDurability = 100.f;
	Item_Desc.eItemType = ItemData.eItemType;
	Item_Desc.iNumItem = ItemCnt;

	Slot->Set_Info(Item_Desc);
}

void CQuestManager::Free()
{
	__super::Free();

	for (auto& Pair : m_QuestMap)
	{
		Safe_Delete(Pair.second);
	}
	m_QuestMap.clear();
	m_RunningQuest.clear();
	m_ClearQuest.clear(); 
	m_pInven = nullptr;
	m_pItemManager = nullptr;
}

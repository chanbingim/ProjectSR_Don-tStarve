#include "CharacterManager.h"
#include "CUtility.h"
#include "GameInstance.h"
#include "Player.h"

IMPLEMENT_SINGLETON(CCharacterManager);

CCharacterManager::CCharacterManager()
{
	Initialize();
}

HRESULT CCharacterManager::Initialize()
{
	m_pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(m_pGameInstance);
	m_pCharacterList.clear();
	return S_OK;
}

void CCharacterManager::Update()
{
	_float3 transform = {};
	_float distance = {};
	if (m_pCharacterList.size()) {
		for (auto Src = m_pCharacterList.begin(); Src != m_pCharacterList.end(); Src++) {
			CCharacter* SrcChar = dynamic_cast<CCharacter*>(*Src);
			transform = SrcChar->Get_Char()->fPos - m_pPlayer->Get_Char()->fPos;
			distance = D3DXVec3Length(&transform);
			SrcChar->GetTarget(m_pPlayer, distance);
			for (auto Dst = Src; Dst != m_pCharacterList.end(); Dst++) {
				if (Src != Dst) {
					CCharacter* DstChar = dynamic_cast<CCharacter*>(*Dst);
					if (SrcChar && DstChar) {
						transform = SrcChar->Get_Char()->fPos - DstChar->Get_Char()->fPos;
						distance = D3DXVec3Length(&transform);
						SrcChar->GetTarget(DstChar, distance);
						DstChar->GetTarget(SrcChar, distance);
					}
				}
			}
		}
	}
	auto ItemObjects = m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Item"));
	if (m_pCharacterList.size() && ItemObjects && !ItemObjects->empty()) {
		for (auto Src = m_pCharacterList.begin(); Src != m_pCharacterList.end(); Src++) {
			CCharacter* SrcChar = dynamic_cast<CCharacter*>(*Src);
			if(SrcChar && SrcChar->GetNeedItem() && !SrcChar->GetNearTarget()) {
				for (auto Dst = ItemObjects->begin(); Dst != ItemObjects->end(); Dst++) {
					transform = SrcChar->Get_Char()->fPos - (*Dst)->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION);
					distance = D3DXVec3Length(&transform);
					SrcChar->GetTarget(*Dst, distance);
				}
			}
		}
	}
	m_pCharacterList.clear();
}

void CCharacterManager::SetPlayer(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
}

void CCharacterManager::AddObject(CCharacter* pCharacter)
{
	m_pCharacterList.push_back(pCharacter);
}

void CCharacterManager::Free()
{
	Safe_Release(m_pGameInstance);
}

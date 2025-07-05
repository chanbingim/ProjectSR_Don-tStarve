#include "Character_Manager.h"
#include "GameInstance.h"
#include "Client_Defines.h"
#include "GameObject.h"

CCharacter_Manager* CCharacter_Manager::m_pInstance = { nullptr };

CCharacter_Manager::CCharacter_Manager()
{
	Initialize();
}

HRESULT CCharacter_Manager::Initialize()
{
	m_ObjectList[EnumToInt(FIELDOBJECT::CREATURE)].clear();
	m_ObjectList[EnumToInt(FIELDOBJECT::OBJECT)].clear();
	return S_OK;
}

void CCharacter_Manager::Add_Object(CGameObject* pGameObject, FIELDOBJECT tType)
{
	auto iter = find(m_ObjectList[EnumToInt(tType)].begin(), m_ObjectList[EnumToInt(tType)].end(), pGameObject);

	if (iter == m_ObjectList[EnumToInt(tType)].end())
	{
		m_ObjectList[EnumToInt(tType)].push_back(pGameObject);
	}
}

void CCharacter_Manager::Remove_Object(CGameObject* pGameObject, FIELDOBJECT tType)
{
	auto iter = find(m_ObjectList[EnumToInt(tType)].begin(), m_ObjectList[EnumToInt(tType)].end(), pGameObject);
	if (iter != m_ObjectList[EnumToInt(tType)].end())
		m_ObjectList[EnumToInt(tType)].erase(iter);
}

list<CGameObject*> CCharacter_Manager::Get_NearObject(CGameObject* pGameObject, _float fDistance, FIELDOBJECT tType)
{
	list<CGameObject*> NearObjects;
	for (auto object : m_ObjectList[EnumToInt(tType)]) {
		if (pGameObject != object) {
			_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - pGameObject->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION);
			_float distance = sqrtf(pow(transform.x, 2) + pow(transform.z, 2));
			if (fDistance > distance) {
				NearObjects.push_back(object);
			}
		}
	}

	NearObjects.sort([](CGameObject* pSour, CGameObject* pDest)->_bool    // ZIndex Á¤·Ä
		{
			_float3 transform = pSour->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - pSour->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION);
			_float3 transform2 = pDest->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - pDest->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION);
			_float distance = sqrtf(pow(transform.x, 2) + pow(transform.z, 2));
			_float distance2 = sqrtf(pow(transform2.x, 2) + pow(transform2.z, 2));
			return distance < distance2;
		});
   return NearObjects;
}

void CCharacter_Manager::Free()
{
	__super::Free();
}

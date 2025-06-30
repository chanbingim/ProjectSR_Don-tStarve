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
	m_ObjectList.clear();
	return S_OK;
}

void CCharacter_Manager::Add_Object(CGameObject* pGameObject)
{
	auto iter = find(m_ObjectList.begin(), m_ObjectList.end(), pGameObject);

	if (iter == m_ObjectList.end())
	{
		m_ObjectList.push_back(pGameObject);
	}
}

void CCharacter_Manager::Remove_Object(CGameObject* pGameObject)
{
	auto iter = find(m_ObjectList.begin(), m_ObjectList.end(), pGameObject);
	if (iter != m_ObjectList.end())
		m_ObjectList.erase(iter);
}

list<CGameObject*> CCharacter_Manager::Get_NearObject(CGameObject* pGameObject)
{
	list<CGameObject*> NearObjects;
	for (auto object : m_ObjectList) {
		if (pGameObject != object) {
			_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - pGameObject->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION);
			_float distance = sqrtf(pow(transform.x, 2) + pow(transform.z, 2));
			if (10.f > distance) {
				NearObjects.push_back(object);
			}
		}
	}
   return NearObjects;
}

void CCharacter_Manager::Free()
{
	__super::Free();
}

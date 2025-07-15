#include "Level_Manager.h"
#include "GameInstance.h"
#include "Collision_Manager.h"

#include "Level.h"

CLevel_Manager::CLevel_Manager()
	: m_pGameInstance { CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);	
}

HRESULT CLevel_Manager::Change_Level(CLevel* pNewLevel)
{
	if (nullptr != m_pCurrentLevel)
	{
		m_pGameInstance->Clear_Resources(m_pCurrentLevel->Get_LevelID());
		Safe_Release(m_pCurrentLevel);
	}	

	m_pCurrentLevel = pNewLevel;

	return S_OK;
}

void CLevel_Manager::Priority_Update(_float fTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_pCurrentLevel->Priority_Update(fTimeDelta);
}

void CLevel_Manager::Update(_float fTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_pCurrentLevel->Update(fTimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	if (nullptr == m_pCurrentLevel)
		return E_FAIL;	

	return m_pCurrentLevel->Render();
}

void CLevel_Manager::CurrentLevelReset()
{
	m_pCurrentLevel->ResetLevelData();
}

CLevel_Manager* CLevel_Manager::Create()
{
	return new CLevel_Manager();
}

void CLevel_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pCurrentLevel);
	Safe_Release(m_pGameInstance);
}

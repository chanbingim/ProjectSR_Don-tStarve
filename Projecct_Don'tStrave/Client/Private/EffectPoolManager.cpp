#include "EffectPoolManager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CEffectPoolManager);

CEffectPoolManager::CEffectPoolManager() : m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT CEffectPoolManager::Initailize()
{

    return S_OK;
}

HRESULT CEffectPoolManager::Add_ActiveEffect(_uint ID, CAinimationObject* pOutAnimObject)
{






    return S_OK;
}

HRESULT CEffectPoolManager::Release_ActiveEffect(_uint ID, CAinimationObject* pOutAnimObject)
{

    return S_OK;
}

HRESULT CEffectPoolManager::Emplace_EffectPool()
{
    //여기서 이펙트 생성
    //오브젝트 아이디 별로생성





    return S_OK;
}

void CEffectPoolManager::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
    for (auto pair : m_EffectActiveMap)
    {
        Safe_Release(pair.second);
    }

    for (auto pair : m_EffectUnActiveMap)
    {
        Safe_Release(pair.second);
    }
}

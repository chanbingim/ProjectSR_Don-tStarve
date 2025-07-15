#include "EffectPoolManager.h"

#include "GameInstance.h"
#include "SpriteEffect.h"

IMPLEMENT_SINGLETON(CEffectPoolManager);

CEffectPoolManager::CEffectPoolManager() : m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT CEffectPoolManager::Initailize()
{
    Emplace_EffectPool();
    return S_OK;
}

HRESULT CEffectPoolManager::Add_ActiveEffect(_uint ID, CAinimationObject** pOutAnimObject, void* pArg)
{
    auto UnAnimList = m_EffectUnActiveMap.find(ID)->second;
    auto ActAnimList = m_EffectActiveMap.find(ID)->second;

    if (UnAnimList->size() <= 0)
    {
        for (size_t i = 0; i < ActAnimList->size(); ++i)
        {
            auto newEffect = m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::STATIC),
                                                                TEXT("Prototype_GameObject_EffectAnim"));
            
            UnAnimList->push_back(static_cast<CAinimationObject *>(newEffect));
        }
    }
       
    auto iter = UnAnimList->begin();

    *pOutAnimObject = *iter;
    (*iter)->Initialize(pArg);
    ActAnimList->push_back(*pOutAnimObject);
    UnAnimList->erase(iter);

    return S_OK;
}

HRESULT CEffectPoolManager::Release_ActiveEffect(_uint ID, CAinimationObject* pOutAnimObject)
{
    auto UnAnimList = m_EffectUnActiveMap.find(ID)->second;
    auto ActAnimList = m_EffectActiveMap.find(ID)->second;

    auto iter = find(ActAnimList->begin(), ActAnimList->end(), pOutAnimObject);
    if (iter == ActAnimList->end())
    {
        auto Effect = dynamic_cast<CSpriteEffect*>(pOutAnimObject);
        Effect->ResetObejctaData();

        UnAnimList->push_back(Effect);
        return E_FAIL;
    }

    return S_OK;
}

void CEffectPoolManager::UpdateActvieEffect(_float fTimeDeleta)
{
    for (auto Pair : m_EffectActiveMap)
    {
        for (auto iter = (*Pair.second).begin(); iter != (*Pair.second).end();)
        {
            (*iter)->Priority_Update(fTimeDeleta);

            if (!(*iter)->isDead())
            {
                m_pGameInstance->Add_RenderGroup(RENDER::BLEND, (*iter));
                iter++;
            }
            else
            {
                auto DeadEffect = (*iter);
                iter = (*Pair.second).erase(iter);
                Release_ActiveEffect(Pair.first, DeadEffect);
            }
                
        }
    }
}

HRESULT CEffectPoolManager::Emplace_EffectPool()
{
    //여기서 이펙트 생성
    //오브젝트 아이디 별로생성
    ADD_Effect(1, TEXT("Prototype_Component_Effect_Rock"));
    ADD_Effect(2, TEXT("Prototype_Component_Effect_Leaf"));
    ADD_Effect(3, TEXT("Prototype_Component_Effect_Lightning"));
    ADD_Effect(4, TEXT("Prototype_Component_Effect_Vine"));

    return S_OK;
}

void CEffectPoolManager::ADD_Effect(_uint ID, _wstring PrototypeTag)
{
    m_EffectActiveMap.insert({ ID, new list<CAinimationObject*>() });
    m_EffectUnActiveMap.insert({ ID, new list<CAinimationObject*>() });
    auto  Effectlist = m_EffectUnActiveMap.find(ID)->second;
    for (size_t i = 0; i < 50; ++i)
    {
        auto newEffect = m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::STATIC), PrototypeTag);
        (*Effectlist).push_back(static_cast<CAinimationObject*>(newEffect));
    }
}

void CEffectPoolManager::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
    for (auto& pair : m_EffectActiveMap)
    {
        for (auto& iter : *pair.second)
        {
            Safe_Release(iter);
        }
        Safe_Delete(pair.second);
    }

    for (auto pair : m_EffectUnActiveMap)
    {
        for (auto& iter : *pair.second)
        {
            Safe_Release(iter);
        }
        Safe_Delete(pair.second);
    }
}

#pragma once
#include "Client_Defines.h"

NS_BEGIN(Engine)
class CGameObject;
NS_END

NS_BEGIN(Client)

class CCharacter_Manager : public CBase
{
private:
    CCharacter_Manager();
    virtual ~CCharacter_Manager() = default;

public:
    HRESULT Initialize();
    void Add_Object(CGameObject* pGameObject);
    void Remove_Object(CGameObject* pGameObject);
    list<CGameObject*> Get_NearObject(CGameObject* pGameObject);
    static CCharacter_Manager* GetInstance() {
        if (nullptr == m_pInstance) {

            m_pInstance = new CCharacter_Manager();
        }
        return m_pInstance;
    }
    static void DestroyInstance()
    {
        if (nullptr != m_pInstance && 0 == m_pInstance->Release()) {
            m_pInstance = nullptr;
        }
        int a = m_pInstance->m_ObjectList.size();
        int b = 0;
    }
private:
    list<CGameObject*> m_ObjectList = { nullptr };
    static CCharacter_Manager* m_pInstance;
    virtual void Free() override;
};

NS_END
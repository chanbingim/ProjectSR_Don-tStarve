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
    void Add_Object(CGameObject* pGameObject, FIELDOBJECT tType);
    void Remove_Object(CGameObject* pGameObject, FIELDOBJECT tType);
    list<CGameObject*> Get_NearObject(CGameObject* pGameObject, _float fDistance, FIELDOBJECT tType);
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
    }

private:
    list<CGameObject*> m_ObjectList[EnumToInt(FIELDOBJECT::END)] = {};
    static CCharacter_Manager* m_pInstance;
    virtual void Free() override;
};

NS_END
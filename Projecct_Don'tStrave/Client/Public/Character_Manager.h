//#pragma once
//#include "Base.h"
//#include "GameObject.h"
//
//NS_BEGIN(Engine)
//class CGameObject;
//NS_END
//
//NS_BEGIN(Client)
//
//class CCharacter_Manager final :  public CBase
//{
//private:
//	CCharacter_Manager();
//	virtual ~CCharacter_Manager();
//
//public:
//	HRESULT		Initialize();
//	void			Add_Object(CGameObject* pGameObject);
//	void			Remove_Object(CGameObject* pGameObject);
//	list<CGameObject*>	Get_NearObject(CGameObject* pGameObject);
//	static CCharacter_Manager* GetInstance(void) {
//		if (nullptr == m_pInstance) {
//	
//			m_pInstance = new CCharacter_Manager;
//		}
//		return m_pInstance;
//	}
//	static void DestroyInstance(void) {
//		if (nullptr != m_pInstance) {
//			m_pInstance->Release();
//		}
//	}
//private:
//	list<CGameObject*>		m_ObjectList = { nullptr };
//	static CCharacter_Manager* m_pInstance;
//	virtual void Free() override;
//};
//
//NS_END;
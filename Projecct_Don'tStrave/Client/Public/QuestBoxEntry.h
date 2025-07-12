#pragma once

#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Engine)
class CTransform;
NS_END

NS_BEGIN(Client)

class CEventButton;

class CQuestBoxEntry : public CUserInterface
{
private:
	CQuestBoxEntry(LPDIRECT3DDEVICE9 pGraphic_Device);
	CQuestBoxEntry(const CQuestBoxEntry& rhs);
	virtual ~CQuestBoxEntry() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);
	virtual void					Priority_Update(_float fTimeDelta);
	virtual void					Update(_float fTimeDelta);
	virtual void					Late_Update(_float fTimeDelta);
	virtual HRESULT					Render();

private :
	CTransform*						m_pParentTransform_Com = { nullptr };
	CEventButton*					m_AcceptBut = { nullptr };
	CEventButton*					m_CancelBut = { nullptr };

private:
	HRESULT							ADD_Components();

public:
	static	CQuestBoxEntry*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};

NS_END
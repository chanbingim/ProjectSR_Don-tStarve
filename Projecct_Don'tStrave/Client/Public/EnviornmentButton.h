#pragma once

#include "EventButton.h"

NS_BEGIN(Client)
class CEnviornmentButton : public CEventButton
{
protected:
	CEnviornmentButton(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEnviornmentButton(const CEnviornmentButton& rhs);
	virtual ~CEnviornmentButton() = default;

public:
	virtual HRESULT					Initialize_Prototype()override;
	virtual HRESULT					Initialize(void* pArg)override;
	virtual void					Priority_Update(_float fTimeDelta)override;
	virtual void					Update(_float fTimeDelta)override;
	virtual void					Late_Update(_float fTimeDelta)override;
	virtual HRESULT					Render()override;

private :
	


private:
	HRESULT							ADD_Components();

public:
	static		CEnviornmentButton*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual		CGameObject*			Clone(void* pArg) override;
	virtual		void					Free() override;
};
NS_END
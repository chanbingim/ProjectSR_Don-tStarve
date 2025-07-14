#pragma once
#include "EventButton.h"

NS_BEGIN(Client)
class CQuestNextButton : public CEventButton
{
private :
	CQuestNextButton(LPDIRECT3DDEVICE9 pGraphic_Device);
	CQuestNextButton(const CQuestNextButton& rhs);
	virtual ~CQuestNextButton() = default;

public :
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

private :
	HRESULT							ADD_Components();

public :
	static		CQuestNextButton*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual		CGameObject*			Clone(void* pArg) override;
	virtual		void					Free() override;
};
NS_END
#pragma once
#include "Button.h"

NS_BEGIN(Client)
class CEventButton : public CButton
{
private :
	CEventButton(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEventButton(const CEventButton& rhs);
	virtual ~CEventButton() = default;

public :
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;






private :
	function<_bool()>				m_Onclicked = { nullptr };

public :
	static		CEventButton*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual		CGameObject*		Clone(void* pArg) override;
	virtual		void				Free() override;
};
NS_END
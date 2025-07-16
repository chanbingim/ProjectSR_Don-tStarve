#pragma once
#include "EventButton.h"

NS_BEGIN(Client)
class CSlideButton : public CEventButton
{
private:
	CSlideButton(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSlideButton(const CSlideButton& rhs);
	virtual ~CSlideButton() = default;

public:
	_float Get_PosX() { return m_fX + m_fRelativeX; }
	void    Set_RelativeX(_float fRelativeX) { m_fRelativeX = fRelativeX; }
public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

	void			 SetPressEvent(function<void()> Func);

private:
	function<void()>				m_OnPressedEvent = { nullptr };

private:
	HRESULT							ADD_Components();

public:
	static		CSlideButton* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual		CGameObject* Clone(void* pArg) override;
	virtual		void					Free() override;
};
NS_END
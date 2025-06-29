#pragma once
#include "Client_Defines.h"
#include "Button.h"

NS_BEGIN(Client)

class CCategory_Button final : public CButton
{
private:
	CCategory_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCategory_Button(const CCategory_Button& Prototype);
	virtual ~CCategory_Button() = default;

public:
	void Select_Button() { m_isSelected = !m_isSelected; }

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

	virtual void HoverEevent() override;
	virtual void ClickedEevent() override;

private:
	_bool		m_isSelected = {};
	CTexture*	m_pBackGroundTexture_Com = { nullptr };

private:
	virtual HRESULT ADD_Components();

public:
	static CCategory_Button* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END


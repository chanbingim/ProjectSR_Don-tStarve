#pragma once
#include "Client_Defines.h"
#include "Button.h"

NS_BEGIN(Client)

class CItem_Button final : public CButton
{
public:
	typedef struct ItemBtn_Desc : public CButton::BUTTON_DESC
	{
		_uint iCraftLevel = {};
	}ITEMBTN_DESC;
private:
	CItem_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
	CItem_Button(const CItem_Button& Prototype);
	virtual ~CItem_Button() = default;

public:
	void Select_Button() { m_isSelected = !m_isSelected; }
	void InResearchLap(_bool inRanged);

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
	_bool		m_isActivate = {};
	_uint		m_iCraftLevel = {};

	CTexture*	m_pBackGroundTexture_Com = { nullptr };

private:
	virtual HRESULT ADD_Components();
	

public:
	static CItem_Button* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END


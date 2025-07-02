#pragma once
#include "Client_Defines.h"
#include "Button.h"

NS_BEGIN(Client)

class CBookMark_Button final : public CButton
{
private:
	CBookMark_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBookMark_Button(const CBookMark_Button& Prototype);
	virtual ~CBookMark_Button() = default;

public:
	void	isSelected(_bool isSelected) { m_isSelected = isSelected; }

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

private:
	virtual HRESULT ADD_Components();

public:
	static CBookMark_Button* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END

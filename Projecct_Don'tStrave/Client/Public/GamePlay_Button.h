#pragma once
#include "Client_Defines.h"
#include "Button.h"

NS_BEGIN(Client)

class CGamePlay_Button final : public CButton
{
private:
	CGamePlay_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGamePlay_Button(const CGamePlay_Button& Prototype);
	virtual ~CGamePlay_Button() = default;

public:
	void Set_Button(_uint iTextureIndex) { m_iTextureIndex = iTextureIndex; }

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

	virtual void	HoverEevent() override;
	virtual void	ClickedEevent() override;

private:
	virtual HRESULT ADD_Components();


public:
	static CGamePlay_Button* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END


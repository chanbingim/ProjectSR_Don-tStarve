#pragma once
#include "Client_Defines.h"
#include "Button.h"

NS_BEGIN(Client)

class CCook_Button final : public CButton
{
private:
	CCook_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCook_Button(const CCook_Button& Prototype);
	virtual ~CCook_Button() = default;

public:
	void Set_Button(_uint iTextureIndex) { m_iTextureIndex = iTextureIndex; }

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
	virtual HRESULT ADD_Components();

public:
	static CCook_Button* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END


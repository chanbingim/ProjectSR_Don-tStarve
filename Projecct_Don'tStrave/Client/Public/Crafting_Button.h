#pragma once
#include "Client_Defines.h"
#include "Button.h"

NS_BEGIN(Client)

class CCrafting_Button final : public CButton
{
private:
	CCrafting_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCrafting_Button(const CCrafting_Button& Prototype);
	virtual ~CCrafting_Button() = default;

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
	_uint m_iTextureIndex = {};

private:
	virtual HRESULT ADD_Components();

public:
	static CCrafting_Button* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END


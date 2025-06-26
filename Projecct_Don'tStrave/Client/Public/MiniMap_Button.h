#pragma once
#include "Client_Defines.h"
#include "Button.h"

NS_BEGIN(Client)

class CMiniMap_Button final : public CButton
{
private:
	CMiniMap_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMiniMap_Button(const CMiniMap_Button& Prototype);
	virtual ~CMiniMap_Button() = default;

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

	virtual void ClickedEevent() override;
	virtual void HoverEevent() override;

private:
	virtual HRESULT ADD_Components();

public:
	static CMiniMap_Button* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END
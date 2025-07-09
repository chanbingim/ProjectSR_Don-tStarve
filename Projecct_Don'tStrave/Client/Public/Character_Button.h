#pragma once
#include "Client_Defines.h"
#include "Button.h"

NS_BEGIN(Client)

class CCharacter_Button final : public CButton
{
private:
	CCharacter_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCharacter_Button(const CCharacter_Button& Prototype);
	virtual ~CCharacter_Button() = default;

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
	_uint m_iIndex = {};

private:
	virtual HRESULT ADD_Components();


public:
	static CCharacter_Button* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END


#pragma once
#include "Client_Defines.h"
#include "Button.h"

NS_BEGIN(Client)

class CCharacter_Info final : public CButton
{
private:
	CCharacter_Info(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCharacter_Info(const CCharacter_Info& Prototype);
	virtual ~CCharacter_Info() = default;

public:
	void Set_Button(_uint iTextureIndex) { m_iTextureIndex = iTextureIndex; }

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

private:
	virtual HRESULT ADD_Components();


public:
	static CCharacter_Info* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END


#pragma once
#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)

class CHunger final : public CUserInterface
{
private:
	CHunger(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHunger(const CHunger& Prototype);
	virtual ~CHunger() = default;

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

private:
	_uint	m_iTextureIndex = {};
	_float	m_fTimeAcc = {};

private:
	HRESULT ADD_Components();

public:
	static CHunger* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;

};

NS_END
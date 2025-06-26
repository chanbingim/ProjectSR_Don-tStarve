#pragma once
#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)

class CClock final : public CUserInterface
{
private:
	CClock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CClock(const CClock& Prototype);
	virtual ~CClock() = default;

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

private:
	_uint		m_iDate = {};
	_float		m_fGameTime = {};
	CTransform* m_pTransform_Com_Clock = { nullptr };
	CTexture*	m_pTexture_Com_Clock = { nullptr };

private:
	HRESULT ADD_Components();

public:
	static CClock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END
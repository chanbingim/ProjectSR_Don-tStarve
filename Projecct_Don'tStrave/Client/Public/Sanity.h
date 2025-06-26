#pragma once
#include "Client_Defines.h"
#include "HeadUpDisplay.h"

NS_BEGIN(Client)

class CSanity final : public CHeadUpDisplay
{
private:
	CSanity(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSanity(const CSanity& Prototype);
	virtual ~CSanity() = default;

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

private:
	HRESULT ADD_Components();

public:
	static CSanity* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;

};

NS_END

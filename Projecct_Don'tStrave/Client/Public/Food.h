#pragma once
#include "Client_Defines.h"
#include "Item.h"

NS_BEGIN(Client);

class CFood : public CItem
{
private:
	CFood(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFood(const CFood& Prototype);
	virtual ~CFood() = default;

public:
	virtual HRESULT  Initialize_Prototype();
	virtual HRESULT  Initialize(void* pArg);
	virtual void	 Priority_Update(_float fTimeDelta);
	virtual void	 Update(_float fTimeDelta);
	virtual void	 Late_Update(_float fTimeDelta);
	virtual HRESULT	 Render();

	void HoverEvent();

private:
	HRESULT ADD_Components();
	void Update_Item(_float fTimeDelta);

public:
	static CFood* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END
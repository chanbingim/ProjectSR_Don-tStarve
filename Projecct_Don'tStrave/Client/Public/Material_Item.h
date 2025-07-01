#pragma once
#include "Client_Defines.h"
#include "Item.h"

NS_BEGIN(Client);

class CMaterial_Item : public CItem
{
private:
	CMaterial_Item(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMaterial_Item(const CMaterial_Item& Prototype);
	virtual ~CMaterial_Item() = default;

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
	static CMaterial_Item* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END
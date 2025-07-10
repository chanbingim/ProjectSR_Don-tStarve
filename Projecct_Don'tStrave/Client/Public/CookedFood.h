#pragma once
#include "Client_Defines.h"
#include "Item.h"

NS_BEGIN(Client);

class CCookedFood : public CItem
{
private:
	CCookedFood(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCookedFood(const CCookedFood& Prototype);
	virtual ~CCookedFood() = default;

public:
	virtual HRESULT  Initialize_Prototype();
	virtual HRESULT  Initialize(void* pArg);
	virtual void	 Priority_Update(_float fTimeDelta);
	virtual void	 Update(_float fTimeDelta);
	virtual void	 Late_Update(_float fTimeDelta);
	virtual HRESULT	 Render();

	void HoverEvent();

private:
	CTexture* m_pFood_TextureCom = { nullptr };

private:
	HRESULT ADD_Components();
	void Update_Item(_float fTimeDelta);

public:
	static CCookedFood* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END
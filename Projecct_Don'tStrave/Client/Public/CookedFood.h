#pragma once
#include "Client_Defines.h"
#include "Item.h"

NS_BEGIN(Client);

class CCookedFood : public CItem
{
	enum class STATE { IDLE, MEATBALL, BONESTEW, END };
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
	void ClickedEvent();

	void Set_Food(_uint iID);

private:
	STATE				m_ePreState = {};
	STATE				m_eCurState = {};

	_uint				m_iFoodID = {};

private:
	HRESULT ADD_Components();
	void	Change_State();

public:
	static CCookedFood* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END
#pragma once
#include "Client_Defines.h"
#include "Item.h"

NS_BEGIN(Engine);
class CState;
class CAnimController;
class CLightComponent;
NS_END

NS_BEGIN(Client);

class CFire : public CItem
{

private:
	CFire(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFire(const CFire& Prototype);
	virtual ~CFire() = default;

public:
	virtual HRESULT  Initialize_Prototype();
	virtual HRESULT  Initialize(void* pArg);
	virtual void	 Priority_Update(_float fTimeDelta);
	virtual void	 Update(_float fTimeDelta);
	virtual void	 Late_Update(_float fTimeDelta);
	virtual HRESULT	 Render();

	void Update_Fire(_float fValue);

private:
	_uint m_iFireLevel = {};

	CState* m_pFireState_Com[3] = { nullptr };
	_float3 m_vScale = {};

	CAnimController*	m_pAnimController = { nullptr };
	D3DLIGHT9			m_Light{};
	CLightComponent*	m_pLight_Com = { nullptr };
	D3DXCOLOR			m_Color = {};

private:
	HRESULT ADD_Components();
	void Update_Item(_float fTimeDelta);
	


public:
	static CFire* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END

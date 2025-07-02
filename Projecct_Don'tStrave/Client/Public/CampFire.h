#pragma once
#include "Client_Defines.h"
#include "Item.h"

NS_BEGIN(Engine);
class CState;
class CAnimController;
NS_END
NS_BEGIN(Client);

class CCampFire : public CItem
{
	enum class STATE {IDLE, DEAD, PREVIEW};
private:
	CCampFire(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCampFire(const CCampFire& Prototype);
	virtual ~CCampFire() = default;

public:
	virtual HRESULT  Initialize_Prototype();
	virtual HRESULT  Initialize(void* pArg);
	virtual void	 Priority_Update(_float fTimeDelta);
	virtual void	 Update(_float fTimeDelta);
	virtual void	 Late_Update(_float fTimeDelta);
	virtual HRESULT	 Render();

	void HoverEvent();

private:
	_uint m_iFireLevel = {};
	_float m_fFireOffset = {};

	CCampFire::STATE	m_ePreState = {};
	CCampFire::STATE	m_eCurState = {};

	CTexture*			m_pFireTexture_Com = { nullptr };
	CTransform*			m_pFireTransform_Com = { nullptr };
	CState*				m_pFireState_Com[3] = {nullptr};

	CAnimController*	m_pAnimController = { nullptr };

private:
	HRESULT ADD_Components();
	void Update_Item(_float fTimeDelta);
	void Update_Fire(_float fValue);


public:
	static CCampFire* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END

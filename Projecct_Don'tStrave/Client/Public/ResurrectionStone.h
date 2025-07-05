#pragma once
#include "Enviornment_Object.h"

NS_BEGIN(Client)
class CResurrectionStone final : public CEnviornment_Object
{
private:
	CResurrectionStone(LPDIRECT3DDEVICE9 pGraphic_Device);
	CResurrectionStone(const CResurrectionStone& rhs);
	virtual ~CResurrectionStone() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual	void			Reset_State();
	virtual HRESULT			Render() override;

	virtual void			Damage(void* pArg) override;
	virtual void			Death() override;

private:
	HRESULT				ADD_Components();
	void				Damageed(_uint itype, _uint iDamage);

	void				BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void				OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void				EndHitActor(CGameObject* HitActor, _float3& _Dir);

public:
	static		CResurrectionStone* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject*					Clone(void* pArg) override;
	void							Free() override;
};
NS_END
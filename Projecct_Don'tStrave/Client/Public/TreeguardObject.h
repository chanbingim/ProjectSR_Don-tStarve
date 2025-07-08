#pragma once
#include "DropItemEnviornment.h"

NS_BEGIN(Engine)
class CAnimController;
NS_END

NS_BEGIN(Client)

class CEnv_Animation;

class CTreeguardObject : public CEnviornment_Object
{
private:
	CTreeguardObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTreeguardObject(const CTreeguardObject& rhs);
	virtual ~CTreeguardObject() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual HRESULT		Render() override;

	virtual void		Damage(void* pArg) override;

private:
	HRESULT				ADD_Components();

	void				BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void				OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void				EndHitActor(CGameObject* HitActor, _float3& _Dir);

private:
	_float3					Pos = {};

public:
	static		CTreeguardObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void						Free() override;
};
NS_END
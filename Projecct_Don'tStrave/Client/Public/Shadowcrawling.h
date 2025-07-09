#pragma once
#include "Monster.h"

NS_BEGIN(Client)

class CShadowcrawling : public CMonster
{
	enum MOTION {
		APPEAR,
		IDLE,
		IDLE_TO_RUN,
		RUN,
		RUN_TO_IDLE,
		IDLE_TO_ATTACK,
		ATTACK,
		TAUNT,
		DEATH,
		MOTION_END
	};
private:
	CShadowcrawling(LPDIRECT3DDEVICE9 pGraphic_Device);
	CShadowcrawling(const CShadowcrawling& Prototype);
	virtual ~CShadowcrawling() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT			SetAnimation(DIR dir, MOTION motion);
	virtual void Hit() override;
	virtual void Attack() override;
	virtual void Death() override;
private:
	MOTION					m_tMotion = {};
private:

	void BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void EndHitActor(CGameObject* HitActor, _float3& _Dir);
public:
	static CShadowcrawling* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
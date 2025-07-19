#pragma once
#include "Monster.h"

NS_BEGIN(Client)

class CGoat : public CMonster
{
	enum MOTION {
		IDLE,
		IDLE_TO_RUN,
		RUN,
		RUN_TO_IDLE,
		ATTACK,
		IDLE_TO_SLEEP,
		SLEEP,
		SLEEP_TO_IDLE,
		DAMAGE,
		IDLE_TO_TAUNT,
		TAUNT,
		TAUNT_TO_IDLE,
		SHOCK,
		SHOCK_TO_IDLE,
		DEATH,
		MOTION_END
	};
private:
	CGoat(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGoat(const CGoat& Prototype);
	virtual ~CGoat() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT			SetAnimation(DIR dir, MOTION motion);
	virtual	void		Hit();
	virtual void		Attack() override;
	virtual void		Death() override;
	void				OutHouse();
	virtual void		GetTarget(CGameObject* actor, _float distance) override;
private:
	MOTION					m_tMotion = {};
private:
	void BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void EndHitActor(CGameObject* HitActor, _float3& _Dir);
public:
	static CGoat* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
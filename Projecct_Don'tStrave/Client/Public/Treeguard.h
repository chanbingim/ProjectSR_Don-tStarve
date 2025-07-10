#pragma once
#include "Monster.h"

NS_BEGIN(Client)

class CTreeguard : public CMonster
{
	enum MOTION {
		TRANSFORM,
		TRANSFORM_MAD,
		TRANSFORM_TREE,
		TREE,
		IDLE,
		IDLE_TO_RUN,
		RUN,
		RUN_TO_IDLE,
		IDLE_TO_PANIC,
		PANIC,
		PANIC_TO_IDLE,
		ATTACK,
		DAMAGE,
		DEATH,
		MOTION_END
	};
private:
	CTreeguard(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTreeguard(const CTreeguard& Prototype);
	virtual ~CTreeguard() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT			SetAnimation(DIR dir, MOTION motion);
	virtual void		Hit() override;
	virtual void		Attack() override;
	virtual void		Death() override;
private:
	MOTION					m_tMotion = {};
private:

	void BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void EndHitActor(CGameObject* HitActor, _float3& _Dir);
public:
	static CTreeguard* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
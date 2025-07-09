#pragma once
#include "Monster.h"

NS_BEGIN(Client)

class CPigHouse;
class CPig : public CMonster
{
	enum MOTION {
		IDLE,
		IDLE_TO_WALK,
		WALK,
		WALK_TO_IDLE,
		IDLE_TO_RUN,
		RUN,
		RUN_TO_IDLE,
		ATTACK,
		IDLE_TO_SLEEP,
		SLEEP,
		SLEEP_TO_IDLE,
		EAT,
		DAMAGE,
		ABANDON,
		DEATH,
		MOTION_END
	};
private:
	CPig(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPig(const CPig& Prototype);
	virtual ~CPig() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT			SetAnimation(DIR dir, MOTION motion);
	virtual void		Damage(void* pArg);
	virtual	void		Hit();
	virtual void		Attack() override;
	virtual void		Death() override;
	void				OutHouse();
private:
	MOTION					m_tMotion = {};
	CPigHouse*				m_pHouse = {};
private:
	void BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void EndHitActor(CGameObject* HitActor, _float3& _Dir);
public:
	static CPig* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
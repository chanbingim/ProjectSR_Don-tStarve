#pragma once
#include "Monster.h"

NS_BEGIN(Client)

class CDeerclops : public CMonster
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
		TAUNT,
		DEATH,
		MOTION_END
	};
private:
	CDeerclops(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDeerclops(const CDeerclops& Prototype);
	virtual ~CDeerclops() = default;

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
	virtual void		GetTarget(CGameObject* actor, _float distance) override;
private:
	MOTION					m_tMotion = {};
	_uint					m_iPattern = {};
	_float3					m_fIceSpikeRight = {};
	_float					m_fIceSpikeTime = {};
	_float3					m_fIceSpike = {};
	_uint					m_iIceSpike = {};
private:

	void BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	virtual void OverlapHitActor(CGameObject* HitActor, _float3& _Dir) override;
	void EndHitActor(CGameObject* HitActor, _float3& _Dir);
public:
	static CDeerclops* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
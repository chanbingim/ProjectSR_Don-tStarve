#pragma once
#include "Spider.h"

NS_BEGIN(Engine)
class CTexture;
class CCollision_Component;
NS_END

NS_BEGIN(Client)

class CSpiderWarrior : public CSpider
{
	enum MOTION {
		IDLE,
		IDLE_TO_RUN,
		RUN,
		RUN_TO_IDLE,
		ATTACK,
		DASH_ATTACK,
		IDLE_TO_SLEEP,
		SLEEP,
		SLEEP_TO_IDLE,
		IDLE_TO_EAT,
		EAT,
		EAT_TO_IDLE,
		DAMAGE,
		IDLE_TO_COWER,
		COWER,
		COWER_TO_IDLE,
		TAUNT,
		DEATH,
		MOTION_END
	};
private:
	CSpiderWarrior(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSpiderWarrior(const CSpiderWarrior& Prototype);
	virtual ~CSpiderWarrior() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT			SetAnimation(DIR dir, MOTION motion);
	virtual void Damage() override;
	virtual void Attack() override;
	virtual void Death() override;
	virtual void OutHouse() override;
private:
	MOTION					m_tMotion = {};
	DIR						m_tDir = {};
	_float					m_fAtkCool = {};
	_float3					m_fDash = {};
private:
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

	void BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void EndHitActor(CGameObject* HitActor, _float3& _Dir);
public:
	static CSpiderWarrior* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
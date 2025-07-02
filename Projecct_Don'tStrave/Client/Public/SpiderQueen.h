#pragma once
#include "Monster.h"

NS_BEGIN(Engine)
class CTexture;
class CCollision_Component;
NS_END

NS_BEGIN(Client)

class CSpiderQueen : public CMonster
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
	CSpiderQueen(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSpiderQueen(const CSpiderQueen& Prototype);
	virtual ~CSpiderQueen() = default;

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
private:
	CCollision_Component* m_pCollision_Com = { nullptr };
	MOTION					m_tMotion = {};
	DIR						m_tDir = {};
	_bool					m_bMove = {};
	_float					m_fAtkCool = {};
	_float3					m_fDash = {};
private:
	HRESULT Ready_Components();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

	void BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void EndHitActor(CGameObject* HitActor, _float3& _Dir);
public:
	static CSpiderQueen* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
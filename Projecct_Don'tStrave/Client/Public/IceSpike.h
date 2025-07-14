#pragma once

#include "Monster.h"

class CIceSpike final : public CMonster
{
public:
	typedef struct IceSpike_Desc
	{
		CGameObject*		pAttacker;
		_float			fAngle;
		_uint			iType;
		MONSTER_DESC		tDesc;
	}ICESPIKE_DESC;
private:
	CIceSpike(LPDIRECT3DDEVICE9 pGraphic_Device);
	CIceSpike(const CIceSpike& Prototype);
	virtual ~CIceSpike() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Damage(void* pArg) override;
	virtual void Hit() override;
	virtual void Attack() override;
private:
	_uint		m_iSpike;
private:
	void BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	virtual void OverlapHitActor(CGameObject* HitActor, _float3& _Dir) override;
	void EndHitActor(CGameObject* HitActor, _float3& _Dir);
public:
	static CIceSpike* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};


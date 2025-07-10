#pragma once
#include "House.h"

NS_BEGIN(Client)
class CSpider;
class CSpiderHouse : public CHouse
{
	enum MOTION {
		SMALL,
		SMALL_DAMAGE,
		SMALL_TO_MEDIUM,
		MEDIUM,
		MEDIUM_DAMAGE,
		MEDIUM_TO_LARGE,
		LARGE,
		LARGE_DAMAGE,
		LARGE_TO_QUEEN,
		LARGE_TO_SMALL,
		DEATH,
		MOTION_END
	};
private:
	CSpiderHouse(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSpiderHouse(const CSpiderHouse& Prototype);
	virtual ~CSpiderHouse() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT			SetAnimation(MOTION motion);
	virtual void	Damage(void* pArg) override;
	virtual void	Hit() override;
	virtual void	Attack() override;
	virtual void	Death() override;
	virtual void	EnterSpider(CSpider* pMonster);
	void Emergency();
private:
	MOTION					m_tMotion = {};
	_float					m_fTimeAcc = {};
	vector<CSpider*>		m_pMonsterVec = {};
	_bool					m_bRecon = {};
private:
	void BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void EndHitActor(CGameObject* HitActor, _float3& _Dir);
public:
	static CSpiderHouse* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
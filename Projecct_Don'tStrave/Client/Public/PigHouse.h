#pragma once
#include "House.h"

NS_BEGIN(Client)
class CPig;
class CPigHouse : public CHouse
{
	enum MOTION {
		IDLE,
		LIGHT,
		DAMAGE,
		ADD,
		BURN,
		DEATH,
		MOTION_END
	};
private:
	CPigHouse(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPigHouse(const CPigHouse& Prototype);
	virtual ~CPigHouse() = default;

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
	virtual void	EnterPig(CPig* pMonster);
	void Emergency();
private:
	MOTION					m_tMotion = {};
	CPig*					m_pPig = {};
private:
	void BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void EndHitActor(CGameObject* HitActor, _float3& _Dir);
public:
	static CPigHouse* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
#pragma once
#include "Monster.h"


NS_BEGIN(Client)
class CSpiderHouse : public CMonster
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
	HRESULT			AddAnimation(MOTION motion);
	virtual void Damage() override;
	virtual void Attack() override;
	virtual void Death() override;
private:
	CTexture* m_pTextureCom[MOTION::MOTION_END] = { nullptr };
	CPlayerAnim* m_pSpiderHouseAnim[MOTION::MOTION_END] = { nullptr };
	CCollision_Component* m_pCollision_Com = { nullptr };
	MOTION					m_tMotion = {};
private:
	HRESULT Ready_Components();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

	void BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void EndHitActor(CGameObject* HitActor, _float3& _Dir);
public:
	static CSpiderHouse* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
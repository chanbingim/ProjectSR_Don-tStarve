#pragma once

#include "Client_Defines.h"
#include "SpriteEffect.h"

NS_BEGIN(Engine)
class CCollision_Component;
NS_END

NS_BEGIN(Client)
class CVineEffect : public CSpriteEffect
{
	enum STATE {IDLE, SPAWN, DESPAWN};
private:
	CVineEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVineEffect(const CVineEffect& rhs);
	virtual ~CVineEffect() = default;

public:
	virtual HRESULT		Initialize_Prototype(const char* FilePath, const _wstring FolderName);
	virtual HRESULT		Initialize(void* pArg);

	virtual void		Priority_Update(_float fTimeDelta);
	virtual void		Update(_float fTimeDelta);
	virtual void		Late_Update(_float fTimeDelta);
	virtual HRESULT		Render();

private:
	CVineEffect::STATE		m_ePreState = {};
	CVineEffect::STATE		m_eCurState = {};

	CCollision_Component*	m_pCollisionCom = { nullptr };

private:
	HRESULT ADD_Component();
	void	Change_State();

	void BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void EndHitActor(CGameObject* HitActor, _float3& _Dir);

public:
	static	CVineEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const char* FilePath, const _wstring FolderName);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void			Free() override;
};
NS_END
#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CCollision_Component;
NS_END

NS_BEGIN(Client)

class CPlayer final : public CLandObject
{
private:
	CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void	BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void	OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void	EndHitActor(CGameObject* HitActor, _float3& _Dir);

private:
	CGameObject*			m_pCamera = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };
	CCollision_Component*	m_pCollision_Com = { nullptr };
	
private:
	HRESULT Ready_Components();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();


public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
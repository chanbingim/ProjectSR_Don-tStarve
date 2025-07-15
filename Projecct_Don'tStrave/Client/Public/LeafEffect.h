#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer;
class CCollision_Component;
NS_END

NS_BEGIN(Client);
class CPlayer;
class CCamera;
class CLeafEffect : public CGameObject
{
public:
	typedef struct Leef_Desc
	{
		_uint iIndex = {};
		_float3 vPosition = {};
		_float3 vMovePos = {};
	}LEAF_DESC;
	enum STATE {IDLE, ATTACK, LAND};
private:
	CLeafEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLeafEffect(const CLeafEffect& Prototype);
	virtual ~CLeafEffect() = default;

public:
	virtual HRESULT  Initialize_Prototype();
	virtual HRESULT  Initialize(void* pArg);
	virtual void	 Priority_Update(_float fTimeDelta);
	virtual void	 Update(_float fTimeDelta);
	virtual void	 Late_Update(_float fTimeDelta);
	virtual HRESULT	 Render();

private:
	CLeafEffect::STATE		m_eCurState = {};
	CLeafEffect::STATE		m_ePreState = {};

	_uint					m_fAngel = {};
	_uint					m_iIndex = {};

	_float					m_fReachTime = {};
	_float					m_fTimeAcc = {};
	_float					m_fDistance_Target = {};

	CTexture*				m_pTextureCom = { nullptr };
	CVIBuffer*				m_pVIBufferCom = { nullptr };
	CCollision_Component*	m_pCollisionCom = { nullptr };
	CPlayer*				m_pPlayer = { nullptr };
	CCamera*				m_pCamera = { nullptr };

	_float3					m_vMovePos = {};
	_float3					m_vAttackDir = {};


private:
	HRESULT ADD_Components();
	void Change_State();
	void BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void Ready_Attack();

public:
	static CLeafEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END

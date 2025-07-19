#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer;
class CLightComponent;
NS_END

NS_BEGIN(Client);

class CPlayer;

class CSkillIndicator : public CGameObject
{
private:
	CSkillIndicator(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkillIndicator(const CSkillIndicator& Prototype);
	virtual ~CSkillIndicator() = default;

public:
	virtual HRESULT  Initialize_Prototype();
	virtual HRESULT  Initialize(void* pArg);
	virtual void	 Priority_Update(_float fTimeDelta);
	virtual void	 Update(_float fTimeDelta);
	virtual void	 Late_Update(_float fTimeDelta);
	virtual HRESULT	 Render();

private:
	_bool						m_bIsEffectActive = { false };
	_bool						m_bStartCharge_Sound = {};
	_float						m_Charge = {};
	_float						m_fAngle = {};
	_float						m_fTimeAcc = {};

	CTexture*					m_pTextureCom = { nullptr };
	CVIBuffer*					m_pVIBufferCom = { nullptr };
	CPlayer*					m_pPlayer = { nullptr };
	CLightComponent*			m_pLightCom = { nullptr };
	class CSpriteEffect*		m_pSpirteEffect = { nullptr };

	_float3						m_vDir = {};
	_float3						m_vEffectPos = {};
	_float4x4					WorldMat = {};
	vector<class CTerrain*>*	m_pTerrains = { nullptr };
	
	
	
private:
	HRESULT ADD_Components();
	void Update_Effect(_float fTimeDelta);

public:
	static CSkillIndicator* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END


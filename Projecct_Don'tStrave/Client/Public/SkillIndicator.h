#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer;
NS_END

NS_BEGIN(Client);

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
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTransform* m_pPlayerTranformCom = { nullptr };

	vector<class CTerrain*>* m_pTerrains = { nullptr };
	_float4x4 WorldMat = {};
	_float m_Charge = {};
private:
	HRESULT ADD_Components();

public:
	static CSkillIndicator* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END


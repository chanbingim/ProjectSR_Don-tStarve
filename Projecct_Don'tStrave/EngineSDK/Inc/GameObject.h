#pragma once

#include "Base.h"

NS_BEGIN(Engine)
class CTransform;

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct GameObject_Desc
	{
		_float3			vScale = {};
		_float3			vRotation = {};
		_float3			vPosition = {};

		_wstring		TextruePath = {};

	}GAMEOBJECT_DESC;

protected:
	CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual HRESULT			Initialize_Late();
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

	virtual void			Damage(void* pArg);
	virtual void			Death();

public:
	_bool isDead() const {
		return m_isDead;
	}
	void  SetDead()  {
		m_isDead = true;
	}

	_float				Get_CameraDistance();


	class CComponent*	Find_Component(const _wstring& strComponentTag);
	CTransform*			GetTransfrom() { return m_pTransformCom; }


protected:
	_bool						m_bEnableBillboard = { false };
	int							m_iData;
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };
	_bool						m_isDead = { false };

	CTransform*					m_pTransformCom = {nullptr};
	map<const _wstring, class CComponent*>		m_Components;

	LPD3DXEFFECT					m_pEffect = { nullptr };
	D3DXHANDLE						m_hTechnique = { nullptr };
	LPDIRECT3DVERTEXDECLARATION9	m_pDecl;
	D3DVERTEXELEMENT9				decl[MAX_FVF_DECL_SIZE];

protected:
	HRESULT Add_Component(	_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, 
							const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);

	HRESULT	Setting_Shader(const WCHAR* ShaderName);

	void				Excute_Billboard(const _matrix& _InvWorldMat, LPDIRECT3DBASETEXTURE9 pTex);
	void				End_Billboard();

private :
	_matrix				m_BillboardMat = {};
	_matrix				m_ViewMat = {};
	_matrix				m_ProMat = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END
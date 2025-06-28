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
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	_bool isDead() const {
		return m_isDead;
	}

	class CComponent*	Find_Component(const _wstring& strComponentTag);
	CTransform*			GetTransfrom() { return m_pTransformCom; }

protected:
	int							m_iData;
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };
	_bool						m_isDead = { false };

	CTransform* m_pTransformCom = {nullptr};
	map<const _wstring, class CComponent*>		m_Components;

protected:
	HRESULT Add_Component(	_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, 
							const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END
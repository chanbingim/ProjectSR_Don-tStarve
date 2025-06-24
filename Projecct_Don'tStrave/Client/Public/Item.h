#pragma once
#include "Client_Defines.h"
#include "LandObject.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer;
NS_END

NS_BEGIN(Client)

class CItem : public CLandObject
{
public:
	typedef struct Item_Desc
	{
		ITEM_TYPE		eItemType{};
		_uint			iItemID{};
		_uint			iNumItem{};
		_float			fDurability{};
		_float3			vPosition{};
	}ITEM_DESC;

private:
	CItem(LPDIRECT3DDEVICE9 pGraphic_Device);
	CItem(const CItem& Prototype);
	virtual ~CItem() = default;

public:
	virtual HRESULT  Initialize_Prototype();
	virtual HRESULT  Initialize(void* pArg);
	virtual void	 Priority_Update(_float fTimeDelta);
	virtual void	 Update(_float fTimeDelta);
	virtual void	 Late_Update(_float fTimeDelta);
	virtual HRESULT	 Render();

private:
	ITEM_TYPE		m_eItemType{};
	_uint			m_iItemID{};
	_uint			m_iNumItem{};
	_float			m_fDurability{};

	CTexture*	m_pTexture_Com = { nullptr };
	CTransform*	m_pTransform_Com = { nullptr };
	CVIBuffer*	m_pVIBuffer_Com = { nullptr };

private:
	HRESULT ADD_Components();

public:
	static CItem* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END

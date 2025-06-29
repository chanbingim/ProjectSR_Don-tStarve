#pragma once
#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)

class CMaterialSlot : public CUserInterface
{
private:
	CMaterialSlot(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMaterialSlot(const CMaterialSlot& Prototype);
	virtual ~CMaterialSlot() = default;

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

private:
	_bool		m_isCreatable = {};
	_uint		m_iItemID = {};
	_uint		m_iNumNeeded = {};
	_uint		m_iMaterials[3] = {};
	_uint		m_iNeededs[3] = {};
	_uint		m_itemCount[3] = {};

	class CInventory* m_pInventory = { nullptr };

	CTexture*	m_pItemTexture_Com = {};

private:
	HRESULT			ADD_Components();

public:
	static CMaterialSlot* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END
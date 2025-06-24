#pragma once
#include "Client_Defines.h"
#include "MouseSlotUI.h"

NS_BEGIN(Client)
class CSlot final : public CMouseSlotUI
{
private:
	CSlot(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSlot(const CSlot& Prototype);
	virtual ~CSlot() = default;

public:
	void Set_Info(_uint	iItemID, _uint iNumItem, _float	fDurability) {
		m_iItemID = iItemID;
		m_iNumItem = iNumItem;
		m_fDurability = fDurability;
		if (5 == m_iItemID)
			m_eItemType = ITEM_TYPE::FOOD;
	}
	_uint	Get_ItemID() { return m_iItemID; }

	void	Merge_Item(CSlot* pSlot);
	void	Clear();

	void	Add_Item(_uint iNum = 1) { m_iNumItem += iNum; }
	void	Use_Item(_uint iNum = 1) { m_iNumItem -= iNum; }
public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

	void			 Render_ItemState();

private:
	_bool			m_bKey = {};

	ITEM_TYPE		m_eItemType = {};
	_uint			m_iItemID = {};
	_uint			m_iNumItem = {};
	_float			m_fDurability = {};

	CTexture*		m_pTexture_Com_ItemState = { nullptr };
	CTexture*		m_pTexture_Com_NumItem = { nullptr };

private:
	HRESULT ADD_Components();
	void	Key_Input();
	void	Update_Item(_float fTimeDelta);
	void	Render_Item();

public:
	static CSlot* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END


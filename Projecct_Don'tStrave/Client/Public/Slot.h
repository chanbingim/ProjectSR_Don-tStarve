#pragma once
#include "Client_Defines.h"
#include "MouseSlotUI.h"

NS_BEGIN(Client)
class CItem;
class CSlot final : public CMouseSlotUI
{
private:
	CSlot(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSlot(const CSlot& Prototype);
	virtual ~CSlot() = default;

public:
	void Set_Info(ITEM_DESC& Item_Desc);
	ITEM_DESC& Get_Info();
	_uint	Get_ItemID() { return m_Item_Desc.iItemID; }

	void	Merge_Item(CSlot* pSlot);
	void	Merge_Item(ITEM_DESC& Item_Desc);
	void	Clear();

	void	Add_Item(_uint iNum = 1) { m_Item_Desc.iNumItem += iNum; }
	void	Use_Item(_uint iNum = 1) { m_Item_Desc.iNumItem -= iNum; }

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	HRESULT			 Render(RECT& rcText);

	void			 Render_ItemState();

private:
	_bool		m_bKey = {};
	Item_Desc	m_Item_Desc = {};

	CTexture*	m_pTexture_Com_ItemState = { nullptr };
	CTexture*	m_pTexture_Com_NumItem = { nullptr };

private:
	HRESULT ADD_Components();
	void	Key_Input();
	void	Update_Item(_float fTimeDelta);
	void	Render_Item(RECT& rcText);

public:
	static CSlot* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END


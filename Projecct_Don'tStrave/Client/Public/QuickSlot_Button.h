#pragma once
#include "Client_Defines.h"
#include "Button.h"

NS_BEGIN(Client)

class CQuickSlot_Button final : public CButton
{
private:
	CQuickSlot_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
	CQuickSlot_Button(const CQuickSlot_Button& Prototype);
	virtual ~CQuickSlot_Button() = default;

public:
	void Set_Item(_uint iItemID) { m_iTextureIndex = iItemID; }

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

	virtual void HoverEevent() override;
	virtual void ClickedEevent() override;

private:
	_uint m_iPreID = {};
	CTexture* m_pItemTexture_Com = { nullptr };

	_uint				m_iNumNeeded = {};
	_uint				m_iMaterials[3] = {};
	_uint				m_iNeededs[3] = {};
	_uint				m_itemCount[3] = {};

	class CInventory* m_pInventory = { nullptr };

private:
	virtual HRESULT ADD_Components();

	void Set_Data(ITEM_DATA& Data);
	void Check(ITEM_DATA& Data);

public:
	static CQuickSlot_Button* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END


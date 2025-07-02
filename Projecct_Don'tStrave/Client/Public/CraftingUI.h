#pragma once
#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)

class CCraftingUI : public CUserInterface
{
private:
	CCraftingUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCraftingUI(const CCraftingUI& Prototype);
	virtual ~CCraftingUI() = default;

public:
	virtual HRESULT  Initialize_Prototype()	override;
	virtual HRESULT  Initialize(void* pArg) override;
	virtual void	 Priority_Update(_float fTimeDelta) override;
	virtual void	 Update(_float fTimeDelta) override;
	virtual void	 Late_Update(_float fTimeDelta) override;
	virtual HRESULT	 Render() override;

	virtual void HoverEevent();
	virtual void ClickedEevent();

	_bool Add_QuickSlot(_uint iItemID);
	void Remove_QuickSlot(_uint iItemID);
	
private:
	_bool					m_bHide = {};

	_uint m_iCategoryIndex = {};
	_uint m_iItemBtnIndex[EnumToInt(CATEGORY::END)] = {};

	class CButton*					m_pOpenButton = {};
	
	class CItem_Info*				m_pItem_Info = { nullptr };
	class CMaterialSlot*			m_pMaterialSlot = { nullptr };
	vector<class CCategory_Button*> m_pCategorys = {};
	vector<class CItem_Button*>		m_pItem_Buttons[EnumToInt(CATEGORY::END)];

	vector<class CQuickSlot_Button*>			m_pQuickSlots = {};

private:
	HRESULT ADD_Components();

public:
	static CCraftingUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
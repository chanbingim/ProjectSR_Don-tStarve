#pragma once
#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)

class CItem_Info final : public CUserInterface
{
private:
	CItem_Info(LPDIRECT3DDEVICE9 pGraphic_Device);
	CItem_Info(const CItem_Info& Prototype);
	virtual ~CItem_Info() = default;

public:
	void Set_Item(_uint iItemID) { m_SelectedItemID = iItemID; }
	
public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	HRESULT			 Render(CTransform* pTransform);

	void			Update_Rect(_float fX, _float fY);

private:
	_uint	m_SelectedItemID = {};

	RECT	m_rcItemName = {};
	RECT	m_rcItemInfo = {};

	_float3		m_vTexture_Position = {};
	CTexture*	m_pBackTexture_Com = {};
	
	vector<class CSlotFrame*> m_pMetarials = {};

	class CButton* m_pBtnCreate = { nullptr };
	class CButton* m_pBtnAddToQuickSlot = { nullptr };

private:
	HRESULT ADD_Components();
	

public:
	static CItem_Info* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END
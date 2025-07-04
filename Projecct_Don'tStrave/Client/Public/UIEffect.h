#pragma once
#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)

class CUIEffect final : public CUserInterface
{
public:
	typedef struct UIEffect_Desc
	{
		_uint			iItemID = {};
		ITEM_DESC       Item_Desc = {};
		class CSlot*	pSlot = {nullptr};
	}UIEFFECT_DESC;

private:
	CUIEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUIEffect(const CUIEffect& Prototype);
	virtual ~CUIEffect() = default;

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

private:
	_uint			m_iTextureIndex = {};
	_float			m_fTimeAcc = {};
	ITEM_DESC		m_Item_Desc = {};
	class CSlot*	m_pSlot = {};
	_float3			m_TargetDir = {};

private:
	virtual HRESULT Add_Components();

public:
	static CUIEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

NS_END


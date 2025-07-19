#pragma once
#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)

class CFoodEffect final : public CUserInterface
{
public:
	typedef struct FoodEffect_Desc
	{
		_uint			iItemID = {};
		CTransform*		pTransform = {};
		
	}FOODEFFECT_DESC;

private:
	CFoodEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFoodEffect(const CFoodEffect& Prototype);
	virtual ~CFoodEffect() = default;

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

	CTransform* m_pTargetTransform = { nullptr };

private:
	virtual HRESULT Add_Components();

public:
	static CFoodEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

NS_END


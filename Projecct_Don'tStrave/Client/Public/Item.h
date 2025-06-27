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
private:
	CItem(LPDIRECT3DDEVICE9 pGraphic_Device);
	CItem(const CItem& Prototype);
	virtual ~CItem() = default;

public:
	ITEM_DESC& Get_Info(){ return m_Item_Desc; }

public:
	virtual HRESULT  Initialize_Prototype();
	virtual HRESULT  Initialize(void* pArg);
	virtual void	 Priority_Update(_float fTimeDelta);
	virtual void	 Update(_float fTimeDelta);
	virtual void	 Late_Update(_float fTimeDelta);
	virtual HRESULT	 Render();

	void HoverEvent();
	void ClickedEvent();

private:
	ITEM_DESC	m_Item_Desc = {};

	CTexture*	m_pTexture_Com = { nullptr };
	CTransform*	m_pTransform_Com = { nullptr };
	CVIBuffer*	m_pVIBuffer_Com = { nullptr };

private:
	HRESULT ADD_Components();
	void Update_Item(_float fTimeDelta);

public:
	static CItem* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END

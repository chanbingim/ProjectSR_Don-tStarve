#pragma once
#include "Client_Defines.h"
#include "Item.h"

NS_BEGIN(Engine);
class CState;
class CAnimController;
NS_END

NS_BEGIN(Client);

class CChest : public CItem
{
	enum class STATE { IDLE, PLACE, OPEN, CLOSE ,END};
private:
	CChest(LPDIRECT3DDEVICE9 pGraphic_Device);
	CChest(const CChest& Prototype);
	virtual ~CChest() = default;

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
	CChest::STATE		m_eCurState = {};
	CChest::STATE		m_ePreState = {};

	CTexture*			m_Texture_Com[EnumToInt(CChest::STATE::END)] = { nullptr };
	CState*				m_State_Com[EnumToInt(CChest::STATE::END)] = {nullptr};

	CAnimController*	m_pAnimController = { nullptr };

	class CChestUI*		m_pChestUI = { nullptr };

private:
	HRESULT ADD_Components();
	void Change_State();

public:
	static CChest* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END

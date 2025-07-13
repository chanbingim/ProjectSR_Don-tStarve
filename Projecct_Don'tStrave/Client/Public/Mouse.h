#pragma once
#include"Client_Defines.h"

#include "UserInterface.h"

NS_BEGIN(Client)

class CMouse final : public CUserInterface
{
private:
	CMouse(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMouse(const CMouse& Prototype);
	virtual ~CMouse() = default;

public:
	class CSlot* Get_Slot() { return m_pSlot; }
	
public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

	void ClickedEevent();

	void Update_HoverItem(_uint itemID);
	void Update_HoverSlot(_uint itemID);
	void Update_Hover(_uint itemID);
	void Update_Hover(const wstring strMessage,const _uint iMouseState);
	void Update_HoverEnv(_uint iObjectID);

private:
	_uint					m_iMouseState = {};
	_bool					m_bPutDown = {};
	_float					m_fTimeAcc = {};
	ITEM_TYPE				m_eType = {};

	wstring					m_strInfoMessage = {};
	wstring					m_strInteraction = {};

	class CSlot*			m_pSlot = { nullptr };
	class CGrid*			m_pGrid = { nullptr };
	class CSkillIndicator*	m_pSkillIndicator = { nullptr };

	CTransform*				m_pPlayerTransform_Com = {};
	CTexture*				m_pBlend_Texture_Com = { nullptr };
	PLAYER_DATA*			m_pPlayer_Data = { nullptr };

private:
	HRESULT ADD_Components();
	HRESULT Add_Slot();
	

public:
	static CMouse* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END
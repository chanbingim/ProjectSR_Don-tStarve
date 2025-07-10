#pragma once
#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)
class CSelect_Character final : public CUserInterface
{
private:
	CSelect_Character(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSelect_Character(const CSelect_Character& Prototype);
	virtual ~CSelect_Character() = default;

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;


private:
	class CCharacter_Button*	m_pButton_Wilson = { nullptr };
	class CCharacter_Button*	m_pButton_Wigfrid = { nullptr };
	class CCharacter_Info*		m_pCharater_Info = { nullptr };

private:
	HRESULT ADD_Components();

public:
	static CSelect_Character* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END
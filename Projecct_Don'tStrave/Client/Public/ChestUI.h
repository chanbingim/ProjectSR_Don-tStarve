#pragma once
#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Engine)
class CAnimController;
class CState;
NS_END

NS_BEGIN(Client)
class CChestUI final : public CUserInterface
{
public:
	enum class STATE {OPEN, CLOSE};
private:
	CChestUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CChestUI(const CChestUI& Prototype);
	virtual ~CChestUI() = default;

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

	void Change_State(CChestUI::STATE eState);
	void Update_State();
	void RePosition_SlotFrame();
	void Add_Render();

private:
	_bool			 m_bRended = {};

	CChestUI::STATE  m_eCurState = {};
	CChestUI::STATE  m_ePreState = {};

	vector<class CSlotFrame*>	m_SlotFrames = {};
	CAnimController*			m_pAnimController = { nullptr };
	CState*						m_pState_Open = nullptr;
	CState*						m_pState_Close = nullptr;


private:
	HRESULT ADD_Components();

public:
	static CChestUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END

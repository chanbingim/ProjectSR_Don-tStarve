#pragma once
#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Engine)
class CAnimController;
class CState;
NS_END

NS_BEGIN(Client)
class CCookUI final : public CUserInterface
{
public:
	enum class STATE { OPEN, CLOSE };
private:
	CCookUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCookUI(const CCookUI& Prototype);
	virtual ~CCookUI() = default;

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

	void Change_State(CCookUI::STATE eState);
	void Update_State();
	void RePosition_SlotFrame();
	void Add_Render();

private:
	_bool						m_bRended = {};

	CCookUI::STATE				m_eCurState = {};
	CCookUI::STATE				m_ePreState = {};

	vector<class CSlotFrame*>	m_SlotFrames = {};
	CAnimController*			m_pAnimController = { nullptr };
	CState*						m_pState_Open = nullptr;
	CState*						m_pState_Close = nullptr;

	CTexture*					m_pOpenTexture_Com = { nullptr };
	class CCookpot*				m_pCookpot = { nullptr };
	class CCook_Button*			m_CookBtn = { nullptr };
private:
	HRESULT ADD_Components();
	_uint Check_Recipe(_uint Items[]);

public:
	static CCookUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END


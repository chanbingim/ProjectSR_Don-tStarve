#pragma once

#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)

struct CQuestData;
class CQuestNextButton;

class CQuestPreView : public CUserInterface
{
private :
	CQuestPreView(LPDIRECT3DDEVICE9 pGraphic_Device);
	CQuestPreView(const CQuestPreView& rhs);
	virtual ~CQuestPreView() = default;

public :
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

	void						ClickedButtonEvent(_uint index);
	void						UpdateViewer();

private :
	_uint						m_ViewIndex = {};
	RECT						m_FontRect = {};

	list<CQuestData*>*			m_pRunningList = { nullptr };
	CQuestNextButton*			m_pNextButton[2] = {nullptr};
	CQuestData*					m_pViewData = { nullptr };

private :
	HRESULT						ADD_Components();
	HRESULT						ADD_NextButtons();

public :
	static	CQuestPreView*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual	CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;

};
NS_END
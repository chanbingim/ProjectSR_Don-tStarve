#pragma once

#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)
class CQuestFrameUI : public CUserInterface
{
private :
	CQuestFrameUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CQuestFrameUI(const CQuestFrameUI& rhs);
	virtual ~CQuestFrameUI() = default;

public :
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);
	virtual void					Priority_Update(_float fTimeDelta);
	virtual void					Update(_float fTimeDelta);
	virtual void					Late_Update(_float fTimeDelta);
	virtual HRESULT					Render();

private :
	HRESULT							ADD_Components();

public :
	static	CQuestFrameUI*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;

};
NS_END
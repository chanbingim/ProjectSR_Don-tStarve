#pragma once

#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Engine)
class CTransform;
NS_END

NS_BEGIN(Client)

class CQuestBoxEntry;

class CListBoxUI : public CUserInterface
{
public:
	typedef struct ListBoxUI_desc
	{
		_uint					m_ipreviewCnt = { 0 };
		CTransform*				pParentTransform_Com;

	} LISTBOXUI_DESC;


private :
	CListBoxUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CListBoxUI(const CListBoxUI& rhs);
	virtual ~CListBoxUI() = default;
	
public :
	virtual HRESULT						Initialize_Prototype();
	virtual HRESULT						Initialize(void* pArg);
	virtual void						Priority_Update(_float fTimeDelta);
	virtual void						Update(_float fTimeDelta);
	virtual HRESULT						Render();
	virtual void						Late_Update(_float fTimeDelta);

public :
	CTransform*							m_pParentTransform_Com = { nullptr };
	vector<CQuestBoxEntry*>				m_QuestBoxVec = {};
	_uint								m_ipreviewCnt = { 0 };
	_uint								m_iCurViewIndex = { 0 };
	_float2								m_EntryBoxSize = {};

private:
	HRESULT								ADD_Components();
	HRESULT								ADD_EntryBox(_uint Index);

public :
	static	CListBoxUI*					Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*				Clone(void* pArg) override;
	virtual void						Free() override;

};
NS_END